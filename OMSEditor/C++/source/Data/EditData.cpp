#include "TDNLIB.h"
#include "EditData.h"
#include "../MIDI_Loader/MIDI_Loader.h"
#include "../Music/PlayCursor.h"
#include "../Music/OMS_Loader.h"

#define MODE_OMT

void EditDataManager::Initialize()
{
	// メモリと値を初期化(落ちた)
	//ZeroMemory(&m_tagMusicInfoEP, sizeof(MusicInfoEP));
	//ZeroMemory(&m_tagMusicInfoVRShoot, sizeof(MusicInfoVRShoot));

	// 曲ファイル名リスト初期化
	m_sProjectDirectory = m_sProjectName = "";

	m_eOMSLoadType = OMS_TYPE::NO_LOAD;
	m_bOGGOpend = false;
	m_bSaveFlag = false;
	m_sShift = 0;
	//m_iEditNoteIndex = EDIT_NO_SELECT_NOTE;

	//m_vNoteList.clear();
	//m_vSoflanList.clear();
	//m_vBeatList.clear();
}

EditDataManager::~EditDataManager()
{
	Clear();
}

void EditDataManager::EditUpdateVRShoot()
{
	// C#側からいじることにした(リストビューの書き換えの問題で)
	/*
	if (m_iEditNoteIndex == NO_EDIT_NOTE) return;

	const float fAngleSpeed = 1.0f;
	float fAddAngle = 0;
	if (GetKeyState('D') & 0x80) fAddAngle += fAngleSpeed;
	if (GetKeyState('A') & 0x80) fAddAngle -= fAngleSpeed;

	float fAddY = 0;
	if (GetKeyState('W') & 0x80) fAddY += 0.01f;
	if (GetKeyState('S') & 0x80) fAddY -= 0.01f;

	m_tagMusicInfoVRShoot.NoteDatas[m_iEditNoteIndex].fDegree += fAddAngle;
	//if (m_tagMusicInfoVRShoot.NoteDatas[m_iEditNoteIndex].fDegree > 180) m_tagMusicInfoVRShoot.NoteDatas[m_iEditNoteIndex].fDegree -= 360;
	m_tagMusicInfoVRShoot.NoteDatas[m_iEditNoteIndex].fRateY = Math::Clamp(m_tagMusicInfoVRShoot.NoteDatas[m_iEditNoteIndex].fRateY + fAddY, 0, 1);
	*/
}

void EditDataManager::Clear()
{
	m_tagMusicInfoEP.Release();
	m_tagMusicInfoVRShoot.Release();

	//for (auto it : m_vNoteList) delete it;
	//m_vNoteList.clear();
	//for (auto it : m_vSoflanList) delete it;
	//m_vSoflanList.clear();
	//for (auto it : m_vBeatList) delete it;
	//m_vBeatList.clear();
}

int EditDataManager::LoadProject(LPCSTR lpcProjectDirectory)
{
	m_sProjectDirectory = lpcProjectDirectory;
	m_sProjectName =
#ifdef MODE_OMT
		c_lpcPROJECT_OMT_NAME;
#else
		c_lpcPROJECT_OMS_NAME;
#endif

	// oggファイルの存在確認
	std::string l_sPath = lpcProjectDirectory;
	l_sPath += c_lpcPROJECT_OGG_NAME;
	m_bOGGOpend = tdnFile::isFileExist(l_sPath.c_str());

	// OMSファイルへのパスを作成
	l_sPath = lpcProjectDirectory;

	OMS_RESULT l_eResult;

	l_eResult =
#ifdef MODE_OMT
		LoadOMT((l_sPath + m_sProjectName).c_str());
#else
		LoadOMS((l_sPath + m_sProjectName).c_str());
#endif

	// OMS読み込み&もしOMSファイルが無かったら読み込みステート変更
	if (l_eResult != OMS_RESULT::OK)
	{
		m_eOMSLoadType = OMS_TYPE::NO_LOAD;
	}

	return (int)l_eResult;
}

int EditDataManager::LoadProjectByOMS(LPCSTR lpcFileName)
{
	// パス分割
	{
		// OMS名(保存用に覚えておく)
		m_sProjectDirectory = m_sProjectName = "";


		// 文字の最後から/までの間
		int i;
		for (i = strlen(lpcFileName) - 1; i > 0; i--)
		{
			if (IsDBCSLeadByte(lpcFileName[i - 1]))
			{
				i--;
				continue;
			}

			if (lpcFileName[i] == '/' || lpcFileName[i] == '\\') break;
		}


		// ディレクトリ
		for (int j = 0; j < i; j++)
		{
			m_sProjectDirectory += lpcFileName[j];
		}


		// ファイル名
		for (int j = i; lpcFileName[j] != '\0' < i; j++)
		{
			m_sProjectName += lpcFileName[j];
		}

	}

	//tdnDebug::OutPutLog("%s\n%s\n%s\n%s\n", lpcFileName, m_sProjectDirectory.c_str(), m_sProjectName.c_str(), (m_sProjectDirectory + c_lpcPROJECT_OGG_NAME).c_str());

	// OGG読み込み
	m_bOGGOpend = tdnFile::isFileExist((m_sProjectDirectory + c_lpcPROJECT_OGG_NAME).c_str());

	// OMS読み込み&もしOMSファイルが無かったら読み込みステート変更
	std::string l_sExt = tdnFile::GetFileExtention(lpcFileName);
	OMS_RESULT l_eResult;

	if (l_sExt == ".oms")
	{
		l_eResult = LoadOMS(lpcFileName);
	}
	else if (l_sExt == ".omt")
	{
		l_eResult = LoadOMT(lpcFileName);
	}

	if(l_eResult != OMS_RESULT::OK)
	{
		m_eOMSLoadType = OMS_TYPE::NO_LOAD;
	}

	return (int)l_eResult;
}

void EditDataManager::SaveProject()
{
	// プロジェクト開いてなかったら抜ける
	if (m_sProjectDirectory == "") return;

#ifdef MODE_OMT
	WriteOMT((m_sProjectDirectory + m_sProjectName).c_str());
#else
	WriteOMS((m_sProjectDirectory + m_sProjectName).c_str());
#endif
}

void EditDataManager::InportOGG(LPCSTR lpcFileName)
{
	tdnFile::Copy(lpcFileName, (m_sProjectDirectory + c_lpcPROJECT_OGG_NAME).c_str());

	// OGG開いた
	m_bOGGOpend = true;
}

void EditDataManager::BaseToVRShooting()
{
	// 申し訳程度の例外処理(C#側で制限してるから、ここでおかしなことにはならないと信じたい)
	if (m_tagMusicInfoEP.OMSInfo.wNumNotes == 0) return;

	// 譜面タイプをVRシューティングに
	m_eOMSLoadType = OMS_TYPE::VR_SHOOT;

	// VR譜面用の構造体を一旦空にする
	m_tagMusicInfoVRShoot.Release();

	// OMSデータ移植
	memcpy_s(&m_tagMusicInfoVRShoot.OMSInfo, sizeof(BaseOMSInfo), &m_tagMusicInfoEP.OMSInfo, sizeof(BaseOMSInfo));

	// バッファ確保
	m_tagMusicInfoVRShoot.NoteDatas = new NoteDataVRShoot[m_tagMusicInfoVRShoot.OMSInfo.wNumNotes];
	m_tagMusicInfoVRShoot.SoflanDatas = new SoflanData[m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans];
	m_tagMusicInfoVRShoot.BeatDatas = new BeatData[m_tagMusicInfoVRShoot.OMSInfo.wNumBeats];

	//tdnDebug::OutPutLog("%d, %d, %d\n", m_tagMusicInfoVRShoot.OMSInfo.wNumNotes, m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans, m_tagMusicInfoVRShoot.OMSInfo.wNumBeats);

	// ノーツ移植
	for (int i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumNotes; i++)
	{
		m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime	= m_tagMusicInfoEP.NoteDatas[i].iEventTime;
		m_tagMusicInfoVRShoot.NoteDatas[i].wSoflanNo	= m_tagMusicInfoEP.NoteDatas[i].wSoflanNo;
		m_tagMusicInfoVRShoot.NoteDatas[i].byNoteType	= m_tagMusicInfoEP.NoteDatas[i].byNoteType;
		m_tagMusicInfoVRShoot.NoteDatas[i].fDegree = 0;
		m_tagMusicInfoVRShoot.NoteDatas[i].fRateY = 0.5f;
		
		// CN関係
		if ((m_tagMusicInfoEP.NoteDatas[i].byNoteType & (BYTE)NOTE_TYPE::CN_START))
		{
			// CN終了ノーツ検索
			for (int j = i + 1; j < m_tagMusicInfoVRShoot.OMSInfo.wNumNotes; j++)
			{
				// CN開始と同じレーンでCN終了属性だったら、そのノーツがCNの終了ノーツなのでその番号
				if (m_tagMusicInfoEP.NoteDatas[j].byLaneNo == m_tagMusicInfoEP.NoteDatas[i].byLaneNo && (m_tagMusicInfoEP.NoteDatas[j].byNoteType & (BYTE)NOTE_TYPE::CN_END))
				{
					m_tagMusicInfoVRShoot.NoteDatas[i].wCNEndIndex = j;
					break;
				}
			}
		}
		else m_tagMusicInfoVRShoot.NoteDatas[i].wCNEndIndex = 0;


		//tdnDebug::OutPutLog("[%d]%d, %d\n",i, m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime, m_tagMusicInfoEP.NoteDatas[i].iEventTime);
	}

	// ソフラン移植
	for (int i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans; i++)
	{
		// 構造体変わったら注意
		memcpy_s(&m_tagMusicInfoVRShoot.SoflanDatas[i], sizeof(SoflanData), &m_tagMusicInfoEP.SoflanDatas[i], sizeof(SoflanData));
	}

	// 拍子移植
	for (int i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumBeats; i++)
	{
		// 構造体変わったら注意
		memcpy_s(&m_tagMusicInfoVRShoot.BeatDatas[i], sizeof(BeatData), &m_tagMusicInfoEP.BeatDatas[i], sizeof(BeatData));
	}

	// 移植元を解放
	m_tagMusicInfoEP.Release();
}

//void EditDataManager::EditStart()
//{
//	//m_iEditNoteIndex = EDIT_NO_SELECT_NOTE;
//	m_SelectNoteIndices.clear();
//}

//UINT EditDataManager::SetEditNoteVRShoot(std::vector<WORD> *list)
//{
//	//tdnDebug::OutPutLog("%d\n", index);
//	//m_iEditNoteIndex = index;
//
//	m_SelectNoteIndices.clear();
//	for (auto it : *list)
//	{
//		m_SelectNoteIndices.push_back(it);
//	}
//
//	return m_tagMusicInfoVRShoot.NoteDatas[m_SelectNoteIndices[0]].iEventTime;
//}

//UINT EditDataManager::SetEditNoteVRShootString(LPCSTR indicesString)
//{
//	m_SelectNoteIndices.clear();
//
//	// 要約すると、「,」で区切った文字列の数字達を格納していってる
//	for (int i = 0; indicesString[i] != '\0'; i++)
//	{
//		std::string sIndexString = "";
//		for (int j = i;; j++)
//		{
//			if (indicesString[j] == ',')
//			{
//				i = j;
//				break;
//			}
//			sIndexString += indicesString[j];
//		}
//		m_SelectNoteIndices.push_back((WORD)std::stoi(sIndexString));
//	}
//	//for each(WORD i in m_SelectNoteIndices) tdnDebug::OutPutLog("%d\n", i);
//
//	return m_tagMusicInfoVRShoot.NoteDatas[m_SelectNoteIndices[0]].iEventTime;
//}
//
//void EditDataManager::SetScreenPositionVRShoot(Vector2 * position)
//{
//	// 選んでなかったら出ていけぇ！！
//	if (m_SelectNoteIndices.empty()) return;
//
//	m_tagMusicInfoVRShoot.NoteDatas[m_SelectNoteIndices[0]].SetScreenPosition(position);
//}

OMS_RESULT EditDataManager::LoadMIDI_MakeMusicScore(LPCSTR lpcFileName)
{
	OMS_RESULT l_eResult;

	// 既存バッファの開放
	m_tagMusicInfoEP.Release();

	/* MIDI読み込む */
	MIDILoadData l_tagMIDIFormat;
	l_eResult = MIDI_Loader::LoadMIDI(&l_tagMIDIFormat, lpcFileName);

	// MIDIファイルオープンに失敗している
	if (l_eResult != OMS_RESULT::OK) return l_eResult;

	// 情報初期化
	//ZeroMemory(&m_tagMusicInfoEP, sizeof(MusicInfoEP));
	// ★Music1に固定する
	m_tagMusicInfoEP.OMSInfo.byPlayMusicNo = 1;

	/* 読み込んだMIDI情報から必要な情報を抜き取っていく */
	m_tagMusicInfoEP.OMSInfo.sDivision = l_tagMIDIFormat.sDivision;

	BYTE temp(0);	 // イベント解析の際に使用する一時保存用変数
	BYTE status(0);   // ステータスバイト用の一時変数
	int cnt(0);
	UINT l_iEventTime(0);	// カーソルバーみたいな感じ
	int delta(0);	// デルタタイム
	bool bCNList[127]{ false };

	// プッシュ用可変配列
	std::vector<NoteDataEP*>	l_vNoteList;
	std::vector<SoflanData*>	l_vSoflanList;
	std::vector<BeatData*>		l_vBeatList;

	for (int no = 0; no < l_tagMIDIFormat.sNumTrack; no++)
	{
		// ★現在位置の初期化
		l_iEventTime = 0;

		for (int j = 0; j < l_tagMIDIFormat.tagTrackDatas[no].iSize; j++)   // データ分だけ繰返し
		{
			delta = 0;

			// デルタタイム計測(0なら多分同時押し、もしくはノートオフとオンが重なったとか)
			while (true)
			{
				//デルタタイムの長さ
				delta = ((delta) << 7) | (l_tagMIDIFormat.tagTrackDatas[no].cData[j] & 0x7F);

				// 7ビット目が0ならデルタタイム終了(デルタタイムは4バイト)
				if ((l_tagMIDIFormat.tagTrackDatas[no].cData[j++] & 0x80) == 0x00) break;
			}

			// 先頭からの位置
			l_iEventTime += delta;
			printf("%d:: \n", delta);   // デルタタイム出力

			// ランニングステータスルールに対する処理
			if ((l_tagMIDIFormat.tagTrackDatas[no].cData[j] & 0x80) == 0x80)
			{
				// ランニングステータスルールが適用されない場合は、ステータスバイト用変数を更新。
				status = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];   // ステータスバイトを保持しておく
			}
			else
			{
				printf("@\n");   // ランニングステータスルール適用のしるし
				j--;   // データバイトの直前のバイト（デルタタイムかな？）を指すようにしておく。
				// 次の処理でj++するはずなので、そうすればデータバイトにアクセスできる。
			}

			// データ判別
			if ((status & 0xf0) == 0x80)
			{
				// ノート・オフ【ボイスメッセージ】
				printf("Note Off [%02dch] : \n", (status & 0x0f));
				j++;
				
				char lane = l_tagMIDIFormat.tagTrackDatas[no].cData[j];
				printf("Note%d\n", (BYTE)lane);

				// 前のレーンがCNなら
				if (bCNList[lane])
				{
					NoteDataEP *note = new NoteDataEP;
					note->byLaneNo = (BYTE)lane;
					note->iEventTime = l_iEventTime;
					note->byNoteType = (BYTE)NOTE_TYPE::CN_END;	// CN終了ノーツ
					/* ★PushZone */
					l_vNoteList.push_back(note);
					m_tagMusicInfoEP.OMSInfo.wNumNotes++;

					// フラグリスト解除
					bCNList[lane] = false;
				}

				printf("[0x%02x] \n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
				printf("Velocity=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
			}
			else if ((status & 0xf0) == 0x90)
			{
				NoteDataEP *note = new NoteDataEP;
				note->iEventTime = l_iEventTime;
				//note.tone = tone;

				// ノート・オン【ボイスメッセージ】
				printf("Note On  [%02dch] : \n", (status & 0x0f));
				j++;

				// ノート番号読み込み
				note->byLaneNo = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
				printf("Note%d\n", note->byLaneNo);
				j++;

				printf("Velocity=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);

				// ★★★velocity値が127ならCNフラグ立つ(なんでvelocityにしたかというと、ノートオンイベントと同時に判定をとりたかったから唯一、一緒に取ってこれるvelocityを使おうという事にした)
				bool bCN = (l_tagMIDIFormat.tagTrackDatas[no].cData[j] == 127);
				if (bCN)
				{
					MyAssert(!bCNList[note->byLaneNo], "2重CNが発生してる");
					bCNList[note->byLaneNo] = true;
					note->byNoteType = (BYTE)NOTE_TYPE::CN_START;	// CNフラグをスタートにする
				}
				else note->byNoteType = (BYTE)NOTE_TYPE::NORMAL;	// 通常ノーツ

				/* ★PushZone */
				l_vNoteList.push_back(note);
				m_tagMusicInfoEP.OMSInfo.wNumNotes++;
			}
			else if ((status & 0xf0) == 0xa0)
			{
				// ポリフォニック・キー・プレッシャー【ボイスメッセージ】
				printf("Polyphonic Key Pressure [%02dch] : \n", (status & 0x0f));
				j++;
				printf("Note%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
				printf("[0x%02x] \n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
				printf("Pressure=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
			}
			else if ((status & 0xf0) == 0xb0)
			{
				// コントロールチェンジ【ボイスメッセージ】、【モードメッセージ】
				printf("Control Change [%02dch] : \n", (status & 0x0f));
				j++;
				temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
				if (temp <= 63){   // 連続可変タイプのエフェクトに関するコントロール情報（MSBもLSBも）
					// （ホントは「0<=c && c<=63」と書きたいけど、warningが出るので「c<=63」にする）
					printf("VariableEffect(\n");
					switch (temp){
					case 0:    // 上位バイト[MSB]
					case 32:   // 下位バイト[LSB]
						printf("BankSelect[%s]\n", (temp == 0) ? "MSB\n" : "LSB\n");   // バンク・セレクト
						break;
					case 1:
					case 33:
						printf("ModulationDepth[%s]\n", (temp == 1) ? "MSB\n" : "LSB\n");   // モジュレーション・デプス
						break;
					case 2:
					case 34:
						printf("BreathType[%s]\n", (temp == 2) ? "MSB\n" : "LSB\n");   // ブレス・タイプ
						break;
					case 4:
					case 36:
						printf("FootType[%s]\n", (temp == 4) ? "MSB\n" : "LSB\n");   // フット・タイプ
						break;
					case 5:
					case 37:
						printf("PortamentoTime[%s]\n", (temp == 5) ? "MSB\n" : "LSB\n");   // ポルタメント・タイム
						break;
					case 6:
					case 38:
						printf("DataEntry[%s]\n", (temp == 6) ? "MSB\n" : "LSB\n");   // データ・エントリー
						break;
					case 7:
					case 39:
						printf("MainVolume[%s]\n", (temp == 7) ? "MSB\n" : "LSB\n");   // メイン・ボリューム
						break;
					case 8:
					case 40:
						printf("BalanceControl[%s]\n", (temp == 8) ? "MSB\n" : "LSB\n");   // バランス・コントロール
						break;
					case 10:
					case 42:
						printf("Panpot[%s]\n", (temp == 10) ? "MSB\n" : "LSB\n");   // パンポット
						break;
					case 11:
					case 43:
						printf("Expression[%s]\n", (temp == 11) ? "MSB\n" : "LSB\n");   // エクスプレッション
						break;
					case 16:
					case 17:
					case 18:
					case 19:
					case 48:
					case 49:
					case 50:
					case 51:
						printf("SomethingElse_No_%d[%s]\n", temp, (temp == 16) ? "MSB\n" : "LSB\n");   // 汎用操作子
						break;
					default:
						printf("##UndefinedType_No_%d[%s]", (temp < 32) ? temp : temp - 32, (temp < 32) ? "MSB\n" : "LSB\n");   // よくわからないコントロール
					}

					printf(")=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
				}
				else if (120 <= temp && temp <= 127)   // モード・メッセージ
				{
					printf("ModeMessage(\n");
					switch (temp){
					case 120:
						printf("AllSoundOff\n");   // オール・サウンド・オフ
						break;
					case 121:
						printf("ResetAllController\n");   // リセット・オール・コントローラー
						break;
					case 122:
						printf("LocalControl\n");   // ローカル・コントロール
						break;
					case 123:
						printf("AllNoteOff\n");   // オール・ノート・オフ
						break;
					case 124:
						printf("OmniOn\n");   // オムニ・オン
						break;
					case 125:
						printf("OmniOff\n");   // オムニ・オフ
						break;
					case 126:
						printf("MonoModeOn\n");   // モノモード・オン（ポリモード・オフ）
						break;
					case 127:
						printf("PolyModeOn\n");   // ポリモード・オン（モノモード・オフ）
						break;
					default:
						printf("##UndefinedType_No_%d\n", temp);   // よくわからないコントロール
					}
					printf(")=%d\n", temp);
				}
				printf("\n");
			}
			else if ((status & 0xf0) == 0xc0)
			{
				// プログラム・チェンジ【ボイスメッセージ】
				printf("Program Change [%02dch] : \n", (status & 0x0f));
				j++;
				printf("ToneNo=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
				//tone = l_tagMIDIFormat.tagTrackDatas[no].cData[j];
			}
			else if ((status & 0xf0) == 0xd0)
			{
				// チャンネル・プレッシャー【ボイスメッセージ】
				printf("Channel Pressure [%02dch] : \n", (status & 0x0f));
				j++;
				printf("Pressure=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
			}
			else if ((status & 0xf0) == 0xe0)
			{
				// ピッチ・ベンド・チェンジ【ボイスメッセージ】
				printf("Pitch Bend Change [%02dch] : \n", (status & 0x0f));
				j++;
				printf(" (LSB:%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
				printf(", MSB:%d)\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
			}
			else if ((status & 0xf0) == 0xf0)
			{
				// 【システム・メッセージ】
				switch (status & 0x0f){
				case 0x00:   // エクスクルーシブ・メッセージ
					printf("F0 Exclusive Message : \n");
					j++;

					// SysExのデータ長を取得
					cnt = 0;   // 初期化
					while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80){   // フラグビットが1の間はループ
						cnt = cnt | temp;   // 合成
						cnt <<= 7;   // 7bit左シフト
					}
					cnt = cnt | temp;   // 合成
					printf(" Length=%u\n", (unsigned int)cnt);   // SysExのデータ長を取得

					for (int k = 1; k <= cnt; k++){   // 長さの分だけデータ取得
						printf("[%02x]\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
					}
					j--;   // 行き過ぎた分をデクリメント

					break;
				case 0x01:   // MIDIタイムコード
					j++;
					break;
				case 0x02:   // ソング・ポジション・ポインタ
					j++;
					break;
				case 0x03:   // ソング・セレクト
					j++;
					break;
				case 0x07:   // エンド・オブ・エクスクルーシブでもあるけども...
					// F7ステータスの場合のエクスクルーシブ・メッセージ
					//printf("@End of Exclusive");
					printf("F7 Exclusive Message : \n");
					j++;

					// SysExのデータ長を取得
					cnt = 0;   // 初期化
					while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80){   // フラグビットが1の間はループ
						cnt = cnt | temp;   // 合成
						cnt <<= 7;   // 7bit左シフト
					}
					cnt = cnt | temp;   // 合成
					printf(" Length=%u\n", (unsigned int)cnt);   // SysExのデータ長を取得

					for (int k = 1; k <= cnt; k++){   // 長さの分だけデータ取得
						printf("[%02x]\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
					}
					j--;   // 行き過ぎた分をデクリメント

					break;
				case 0x0F:   // メタイベント

					printf("Meta Ivent : \n");
					j++;
					switch ((BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]){
					case 0x00:   // シーケンス番号
						printf("Sequence Number=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
						cnt <<= 8;   // 8bit左シフト
						cnt = cnt | (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
						printf("%d\n", cnt);
						break;
					case 0x01:   // テキスト[可変長]
						printf("Text=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x02:   // 著作権表示[可変長]
						printf("Copyright=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x03:   // シーケンス名（曲タイトル）・トラック名[可変長]
						printf("Title=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x04:   // 楽器名[可変長]
						printf("InstName=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x05:   // 歌詞[可変長]
						printf("Words=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x06:   // マーカー[可変長]
						printf("Marker=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x07:   // キューポイント[可変長]
						printf("CuePoint=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x08:   // プログラム名（音色名）[可変長]
						printf("ProgramName=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x09:   // デバイス名（音源名）[可変長]
						printf("DeviceName=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("%c", l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					case 0x20:   // MIDIチャンネルプリフィックス[1byte]
						printf("MidiChannelPrefix=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
						printf("%d\n", cnt);
						break;
					case 0x21:   // ポート指定[1byte]
						printf("Port=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
						printf("%d\n", cnt);
						break;
					case 0x2F:   // トラック終端[0byte]
						printf("End of Track\n");
						j += 1;   // データ長の分を通り越す

						// ★よくわからんけど、これで直ることもある
						//l_iEventTime -= delta;

						break;
					case 0x51:   // テンポ設定[3byte]

						printf("Temp=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
						cnt <<= 8;   // 8bit左シフト
						cnt = cnt | (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
						cnt <<= 8;   // 8bit左シフト
						cnt = cnt | (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
						printf("%d\n", cnt);

						{
							SoflanData *soflan = new SoflanData;
							soflan->iEventTime = l_iEventTime;

							// BPM割り出し
							soflan->fBPM = 60000000 / (float)cnt;

							// 小数点第1で切り捨て処理(10にすると第一位、100にすると第2位まで切り捨てられる)
							soflan->fBPM = floorf(soflan->fBPM * 10) / 10;

							// tick値割り出し
							soflan->fTick = (60 / soflan->fBPM / m_tagMusicInfoEP.OMSInfo.sDivision * 1000);

							// リストにぶちこむ/*PushZone*/
							l_vSoflanList.push_back(soflan);
							m_tagMusicInfoEP.OMSInfo.wNumSoflans++;
						}

						// ★よくわからんけど、これで直ることもある
						//l_iEventTime -= delta;

						break;
					case 0x54:   // SMPTEオフセット[5byte]
						printf("SMPTE_Offset=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
						switch (cnt & 0xC0){   // フレーム速度
						case 0x00:
							printf("24fps\n");
							break;
						case 0x01:
							printf("25fps\n");
							break;
						case 0x10:
							printf("30fps(DropFrame)\n");
							break;
						case 0x11:
							printf("30fps\n");
							break;
						}
						printf(",Hour:%d\n", (cnt & 0x3F));   // 時間
						printf(",Min:%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);   // 分
						printf(",Sec:%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);   // 秒
						printf(",Frame:%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);   // フレーム
						printf(",SubFrame:%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);   // サブフレーム
						break;
					case 0x58:   // 拍子設定[4byte]
						j += 2;   // データ長の分を通り越す

						{
							BeatData *l_tagBeat=new BeatData;
							l_tagBeat->iEventTime = l_iEventTime;

							// 分子
							l_tagBeat->byNumerator = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];

							// 拍子の分母を算出する
							l_tagBeat->byDenominator = 1;
							for (int k = 0; k < (int)l_tagMIDIFormat.tagTrackDatas[no].cData[j]; k++)
								l_tagBeat->byDenominator *= 2;   // 2の累乗

							// リストにぶちこむ/*PushZone*/
							l_vBeatList.push_back(l_tagBeat);
							m_tagMusicInfoEP.OMSInfo.wNumBeats++;
						}

						j++;
						printf(" ClockPerBeat=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						printf(" 32NotePer4Note=%d\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j]);
						break;
					case 0x59:   // 調設定[2byte]
						printf("Key=\n");
						j += 2;   // データ長の分を通り越す
						cnt = (int)l_tagMIDIFormat.tagTrackDatas[no].cData[j++];
						if (temp > 0)
						{
							printf("Sharp[%d]\n", temp);
						}
						else if (temp == 0)
						{
							printf("C\n");
						}
						else
						{
							printf("Flat[%d]\n", temp);
						}
						cnt = (int)l_tagMIDIFormat.tagTrackDatas[no].cData[j];
						if (temp == 0)
						{
							printf("_Major\n");
						}
						else
						{
							printf("_Minor\n");
						}

						break;
					case 0x7F:   // シーケンサ特定メタイベント
						printf("SpecialIvent=\n");
						cnt = 0;
						j += 1;
						while ((temp = (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]) & 0x80)
						{
							cnt = cnt | (temp & 0x7F);   // 合成
							cnt <<= 7;   // 7bit左シフト
						}
						cnt = cnt | (temp & 0x7F);   // 合成
						for (int k = 1; k <= cnt; k++)
						{
							printf("[%02x]\n", (BYTE)l_tagMIDIFormat.tagTrackDatas[no].cData[j++]);
						}
						j--;   // 行き過ぎた分をデクリメント
						break;
					}
					break;
				}
			}
		}
	}

	//tdnDebug::OutPutLog("キテルグマ\n");

	// データ解放
	for (int i = 0; i < l_tagMIDIFormat.sNumTrack; i++)
	{
		delete l_tagMIDIFormat.tagTrackDatas[i].cData;
	}
	delete l_tagMIDIFormat.tagTrackDatas;

	//tdnDebug::OutPutLog("キテルグマ\n");

	//tdnDebug::OutPutLog("キテルグマ\n");

	// バッファ確保
	m_tagMusicInfoEP.NoteDatas = new NoteDataEP[m_tagMusicInfoEP.OMSInfo.wNumNotes];
	m_tagMusicInfoEP.SoflanDatas = new SoflanData[m_tagMusicInfoEP.OMSInfo.wNumSoflans];
	m_tagMusicInfoEP.BeatDatas = new BeatData[m_tagMusicInfoEP.OMSInfo.wNumBeats];

	//tdnDebug::OutPutLog("キテルグマ\n");

	// ★ソフラン座標調整
	for (UINT i=0;i<l_vSoflanList.size();i++)
	{
		// 情報コピー
		memcpy_s(&m_tagMusicInfoEP.SoflanDatas[i], sizeof(SoflanData), l_vSoflanList[i], sizeof(SoflanData));

		if (i > 0)
		{
			int val = l_vSoflanList[i]->iEventTime - l_vSoflanList[i - 1]->iEventTime;
			m_tagMusicInfoEP.SoflanDatas[i].iEventTime = (int)(val * l_vSoflanList[i - 1]->fTick) + m_tagMusicInfoEP.SoflanDatas[i - 1].iEventTime;

			//tdnDebug::OutPutLog("%d, %d, %.1f\n", val, m_tagBaseMusicInfo.SoflanDatas[i].iEventTime, m_tagBaseMusicInfo.SoflanDatas[i].fTick);
		}
	}

	//tdnDebug::OutPutLog("キテルグマ\n");

	// ★ノーツ座標調整
	for (UINT i = 0; i<l_vNoteList.size(); i++)
	{
		// ★同時押し情報追加(CN終了でなければ)
		if ((l_vNoteList[i]->byNoteType & (BYTE)NOTE_TYPE::CN_END) == 0)
		{
			for (UINT j = i + 1; j < l_vNoteList.size(); j++)
			{
				// イベント時間が同じなら同時押し
				if (l_vNoteList[j]->iEventTime == l_vNoteList[i]->iEventTime)
				{
					l_vNoteList[i]->byNoteType |= (BYTE)NOTE_TYPE::CHORDS;
					l_vNoteList[j]->byNoteType |= (BYTE)NOTE_TYPE::CHORDS;
				}

				// 過ぎたら抜ける
				else if (l_vNoteList[j]->iEventTime > l_vNoteList[i]->iEventTime)
					break;
			}
		}

		//tdnDebug::OutPutLog("キテルグマ\n");
		// 情報コピー
		memcpy_s(&m_tagMusicInfoEP.NoteDatas[i], sizeof(NoteDataEP), l_vNoteList[i], sizeof(NoteDataEP));
		//tdnDebug::OutPutLog("キテルグマ\n");
		for (UINT j = m_tagMusicInfoEP.OMSInfo.wNumSoflans - 1; j >= 0; j--)
		{
			//tdnDebug::OutPutLog("キテルグマ %d, %d, %d, %d\n", m_tagMusicInfoEP.OMSInfo.wNumNotes, m_tagMusicInfoEP.OMSInfo.wNumSoflans, l_vNoteList.size(), l_vSoflanList.size());
			if (m_tagMusicInfoEP.NoteDatas[i].iEventTime >= l_vSoflanList[j]->iEventTime)	// >ではなく、>=にしないとソフランがずれる
			{
				//tdnDebug::OutPutLog("キテルグマ\n");
				const int val(m_tagMusicInfoEP.NoteDatas[i].iEventTime - l_vSoflanList[j]->iEventTime);
				m_tagMusicInfoEP.NoteDatas[i].iEventTime = (int)(val * l_vSoflanList[j]->fTick) + m_tagMusicInfoEP.SoflanDatas[j].iEventTime;	// ソフランのイベント時間+そっからの自分の時間的な何か
				m_tagMusicInfoEP.NoteDatas[i].wSoflanNo = j;
				break;
			}
		}
	}


	// ★拍子座標調整
	for (UINT i = 0; i<l_vBeatList.size(); i++)
	{
		// 情報コピー
		memcpy_s(&m_tagMusicInfoEP.BeatDatas[i], sizeof(BeatData), l_vBeatList[i], sizeof(BeatData));
		for (UINT j = m_tagMusicInfoEP.OMSInfo.wNumSoflans - 1; j >= 0; j--)
		{
			if (m_tagMusicInfoEP.BeatDatas[i].iEventTime >= l_vSoflanList[j]->iEventTime)	// >ではなく、>=にしないとソフランがずれる
			{
				const int val(m_tagMusicInfoEP.BeatDatas[i].iEventTime - l_vSoflanList[j]->iEventTime);
				m_tagMusicInfoEP.BeatDatas[i].iEventTime = (int)(val * l_vSoflanList[j]->fTick) + m_tagMusicInfoEP.SoflanDatas[j].iEventTime;	// ソフランのイベント時間+そっからの自分の時間的な何か
				break;
			}
		}
	}

	// リスト解放
	for (auto it : l_vNoteList) delete it;
	for (auto it : l_vSoflanList) delete it;
	for (auto it : l_vBeatList) delete it;

	// 譜面タイプ

	// ★★★VRモードだったら、VRを極力残しつつMIDIのデータに合わせる
	if (m_eOMSLoadType == OMS_TYPE::VR_SHOOT)
	{
		//tdnDebug::OutPutLog("%d, ", m_tagMusicInfoVRShoot.OMSInfo.wNumNotes);

		MusicInfoVRShoot BackUp;
		memcpy_s(&BackUp.OMSInfo, sizeof(BaseOMSInfo), &m_tagMusicInfoVRShoot.OMSInfo, sizeof(BaseOMSInfo));

		// ノーツコピー
		BackUp.NoteDatas = new NoteDataVRShoot[BackUp.OMSInfo.wNumNotes];
		for (UINT i = 0; i < BackUp.OMSInfo.wNumNotes; i++)
		{
			memcpy_s(&BackUp.NoteDatas[i], sizeof(NoteDataVRShoot), &m_tagMusicInfoVRShoot.NoteDatas[i], sizeof(NoteDataVRShoot));
		}

		// コピー終わったので読み込んだEP情報をVR用に変換
		BaseToVRShooting();

		//tdnDebug::OutPutLog("%d\n", m_tagMusicInfoVRShoot.OMSInfo.wNumNotes);

		// さっきのバックアップ
		UINT cursor = 0;
		for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumNotes; i++)
		{
			for (UINT j = cursor; j < BackUp.OMSInfo.wNumNotes; j++)
			{
				// イベント時間が一致していたら、その情報をもらう
				if (BackUp.NoteDatas[j].iEventTime == m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime)
				{
					//memcpy_s(&m_tagMusicInfoVRShoot.NoteDatas[i], sizeof(NoteDataVRShoot), &BackUp.NoteDatas[j], sizeof(NoteDataVRShoot));
					m_tagMusicInfoVRShoot.NoteDatas[i].fDegree = BackUp.NoteDatas[j].fDegree;
					m_tagMusicInfoVRShoot.NoteDatas[i].fRateY = BackUp.NoteDatas[j].fRateY;
					cursor = j + 1;
					break;
				}

				// 杉田智一ので、抜ける
				else if (BackUp.NoteDatas[j].iEventTime > m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime)
					break;
			}
		}

		delete[] BackUp.NoteDatas;
	}
	else m_eOMSLoadType = OMS_TYPE::BASE;

	return OMS_RESULT::OK;
}


void EditDataManager::Sort()
{
	//// ラムダ式
	//auto Ascending = [](EditData::Base *val1, EditData::Base *val2){
	//	return (val1->GetEventTime() < val2->GetEventTime());
	//};
	//
	//for (int i = 0; i < (int)LIST_TYPE::MAX; i++)
	//{
	//	if (Lists[i].size() <= 1)return;
	//	Lists[i].sort(Ascending); // 昇順に並べる
	//}
}

OMS_RESULT EditDataManager::LoadOMS(LPCSTR lpcFileName)
{
	// 既存バッファの開放
	Clear();

	//tdnDebug::OutPutLog("%d", sizeof(BaseOMSInfo));

	// OMSからデータ読み込み
	// 読み出しくん
	std::ifstream ifs(lpcFileName, std::ios::binary);
	if (!ifs) return OMS_RESULT::FAILED_OPEN;

	/* ヘッダチャンク侵入 */
	char work[5];
	ifs.read(work, 4);										// ヘッダチャンクであることを示す"OMhd"という4文字のアスキーコード
	if (
		work[0] != 'O' ||
		work[1] != 'M' ||
		work[2] != 'h' ||
		work[3] != 'd'
		)
		return OMS_RESULT::FAILED_READ_OMS;

	// ★データのバージョン
	BYTE l_byOMSType;
	ifs.read((char*)&l_byOMSType, sizeof(BYTE));
	m_eOMSLoadType = (OMS_TYPE)l_byOMSType;

	OMS_RESULT result;

	switch (m_eOMSLoadType)
	{
	case OMS_TYPE::BASE:
		result =  EditOMSLoader::LoadBaseOMS(&m_tagMusicInfoEP, &ifs);
		if (result == OMS_RESULT::OK)
		{
			m_sShift = m_tagMusicInfoEP.OMSInfo.sShift;
		}
		break;

	case OMS_TYPE::VR_SHOOT:
		result = EditOMSLoader::LoadVRShootOMS(&m_tagMusicInfoVRShoot, &ifs);
		if (result == OMS_RESULT::OK)
		{
			m_sShift = m_tagMusicInfoVRShoot.OMSInfo.sShift;
		}
		break;

	default:
		assert(0);
		result =  OMS_RESULT::FAILED_OPEN;
		break;
	}

	// ★選曲情報構造体に書き込む
	//SelectMusicMgr->Set(&m_tagBaseMusicInfo);
	return result;
}

OMS_RESULT EditDataManager::LoadOMT(LPCSTR lpcFileName)
{
	// 既存バッファの開放
	Clear();

	// OMTからデータ読み込み
	// 読み出しくん
	std::ifstream ifs(lpcFileName);
	if (!ifs) return OMS_RESULT::FAILED_OPEN;

	/* ヘッダチャンク侵入 */
	char l_cChunkID[16];
	ifs >> l_cChunkID;	// ヘッダチャンクであることを示す"OMhd"という4文字のアスキーコード									
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'h' ||
		l_cChunkID[3] != 'd'
		)
		return OMS_RESULT::FAILED_READ_OMS;

	// ★データのバージョン
	ifs >> l_cChunkID;

	// 読み込んだ文字列でenumの値を設定
	if (strcmp("BASE", l_cChunkID) == 0)			m_eOMSLoadType = OMS_TYPE::BASE;
	else if (strcmp("VR_SHOOT", l_cChunkID) == 0)	m_eOMSLoadType = OMS_TYPE::VR_SHOOT;

	OMS_RESULT result;
	switch (m_eOMSLoadType)
	{
	case OMS_TYPE::BASE:
		result = EditOMSLoader::LoadBaseOMT(&m_tagMusicInfoEP, &ifs);
		if (result == OMS_RESULT::OK)
		{
			m_sShift = m_tagMusicInfoEP.OMSInfo.sShift;
		}
		break;

	case OMS_TYPE::VR_SHOOT:
		result = EditOMSLoader::LoadVRShootOMT(&m_tagMusicInfoVRShoot, &ifs);
		if (result == OMS_RESULT::OK)
		{
			m_sShift = m_tagMusicInfoVRShoot.OMSInfo.sShift;
		}
		break;

	default:
		assert(0);
		result = OMS_RESULT::FAILED_OPEN;
		break;
	}

	// ★選曲情報構造体に書き込む
	//SelectMusicMgr->Set(&m_tagBaseMusicInfo);
	return result;
}

void EditDataManager::WriteOMS(LPCSTR lpcFileName)
{
	if (m_eOMSLoadType == OMS_TYPE::NO_LOAD) return;


	// 一旦ソート(イベント時間順に)
	//Sort();

	/*
	out 書き込み専用に開く。デフォルト。
	app 追記用に開く。
	ate ファイルを開くと、自動的に末尾に移動する。
	trunc ファイルを開くと、以前の内容を全て削除する。
	binary 特殊な文字を置換しない。
	*/

	// 書き出しくん
	std::ofstream ofs(lpcFileName, std::ios::binary);

	MyAssert(ofs, "OMS書き出しに失敗！");	// 何かしらの例外が生じて書き込めない(パス間違えてるとか)

	/* ヘッダチャンク作成 */
	ofs.write("OMhd", sizeof(char[4]));

	// ★データのバージョン
	BYTE l_byOMSType((BYTE)m_eOMSLoadType);
	ofs.write((char*)&l_byOMSType, sizeof(BYTE));

	switch (m_eOMSLoadType)
	{
	case OMS_TYPE::BASE:
		// シフト設定
		m_tagMusicInfoEP.OMSInfo.sShift = m_sShift;

		// 書き出し
		WriteBaseOMS(&ofs);
		break;

	case OMS_TYPE::VR_SHOOT:
		// シフト設定
		m_tagMusicInfoVRShoot.OMSInfo.sShift = m_sShift;

		// 書き出し
		WriteVRShootOMS(&ofs);
		break;
	}
}

void EditDataManager::WriteBaseOMS(std::ofstream *ofs)
{
	// OMSフォーマット出力
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.wNumNotes, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.wNumSoflans, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.wNumBeats, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.sDivision, sizeof(short));
	ofs->write((char*)&m_tagMusicInfoEP.OMSInfo.sShift, sizeof(short));

	/* データチャンク作成 */
	//ofs << "OMdt";
	ofs->write("OMdt", sizeof(char[4]));


	//for (auto it : m_vNoteList)
	//{
	//	switch (m_eOMSLoadType)
	//	{
	//	case OMS_TYPE::BASE:
	//	ofs.write((char*)it, sizeof(BaseNoteData));
	//		break;
	//	default:
	//		break;
	//	}
	//}
	// ノーツ情報
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumNotes; i++)
	{
		ofs->write((char*)&m_tagMusicInfoEP.NoteDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoEP.NoteDatas[i].wSoflanNo, sizeof(WORD));
		ofs->write((char*)&m_tagMusicInfoEP.NoteDatas[i].byNoteType, sizeof(BYTE));
		ofs->write((char*)&m_tagMusicInfoEP.NoteDatas[i].byLaneNo, sizeof(BYTE));
	}

	// BPM変更情報
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumSoflans; i++)
	{
		ofs->write((char*)&m_tagMusicInfoEP.SoflanDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoEP.SoflanDatas[i].fBPM, sizeof(float));
		ofs->write((char*)&m_tagMusicInfoEP.SoflanDatas[i].fTick, sizeof(float));
	}

	// 拍子変更情報
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumBeats; i++)
	{
		ofs->write((char*)&m_tagMusicInfoEP.BeatDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoEP.BeatDatas[i].byNumerator, sizeof(BYTE));
		ofs->write((char*)&m_tagMusicInfoEP.BeatDatas[i].byDenominator, sizeof(BYTE));
	}
}

void EditDataManager::WriteVRShootOMS(std::ofstream *ofs)
{
	// OMSフォーマット出力
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.wNumNotes, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.wNumBeats, sizeof(WORD));
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.sDivision, sizeof(short));
	ofs->write((char*)&m_tagMusicInfoVRShoot.OMSInfo.sShift, sizeof(short));


	/* データチャンク作成 */
	ofs->write("OMdt", sizeof(char[4]));

	// ノーツ情報
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumNotes; i++)
	{
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].wSoflanNo, sizeof(WORD));
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].byNoteType, sizeof(BYTE));
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].fDegree, sizeof(float));
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].fRateY, sizeof(float));
		ofs->write((char*)&m_tagMusicInfoVRShoot.NoteDatas[i].wCNEndIndex, sizeof(WORD));
	}

	// BPM変更情報
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans; i++)
	{
		ofs->write((char*)&m_tagMusicInfoVRShoot.SoflanDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoVRShoot.SoflanDatas[i].fBPM, sizeof(float));
		ofs->write((char*)&m_tagMusicInfoVRShoot.SoflanDatas[i].fTick, sizeof(float));
	}

	// 拍子変更情報
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumBeats; i++)
	{
		ofs->write((char*)&m_tagMusicInfoVRShoot.BeatDatas[i].iEventTime, sizeof(int));
		ofs->write((char*)&m_tagMusicInfoVRShoot.BeatDatas[i].byNumerator, sizeof(BYTE));
		ofs->write((char*)&m_tagMusicInfoVRShoot.BeatDatas[i].byDenominator, sizeof(BYTE));
	}
}

void EditDataManager::WriteOMT(LPCSTR lpcFileName)
{
	if (m_eOMSLoadType == OMS_TYPE::NO_LOAD) return;


	// 一旦ソート(イベント時間順に)
	//Sort();

	/*
	out 書き込み専用に開く。デフォルト。
	app 追記用に開く。
	ate ファイルを開くと、自動的に末尾に移動する。
	trunc ファイルを開くと、以前の内容を全て削除する。
	binary 特殊な文字を置換しない。
	*/

	//tdnDebug::OutPutLog("%d, %d\n", m_tagMusicInfoVRShoot.OMSInfo.wNumNotes, (int)m_eOMSLoadType);

	// 書き出しくん
	std::ofstream ofs(lpcFileName);

	MyAssert(ofs, "OMT書き出しに失敗！");	// 何かしらの例外が生じて書き込めない(パス間違えてるとか)

	/* ヘッダチャンク作成 */
	ofs << "OMhd\n";

	// ★データのバージョンで分岐
	switch (m_eOMSLoadType)
	{
	case OMS_TYPE::BASE:
		// シフト設定
		m_tagMusicInfoEP.OMSInfo.sShift = m_sShift;

		// 書き出し
		WriteBaseOMT(&ofs);
		break;

	case OMS_TYPE::VR_SHOOT:
		// シフト設定
		m_tagMusicInfoVRShoot.OMSInfo.sShift = m_sShift;

		// 書き出し
		WriteVRShootOMT(&ofs);
		break;
	}
}

void EditDataManager::WriteBaseOMT(std::ofstream * ofs)
{
	// タイプ名
	(*ofs) << "BASE\n";

	// OMSフォーマット出力
	char str[256];
	sprintf_s(str, 256, "%d %d %d %d %d\n",
		m_tagMusicInfoEP.OMSInfo.byPlayMusicNo,
		m_tagMusicInfoEP.OMSInfo.wNumSoflans,
		m_tagMusicInfoEP.OMSInfo.wNumBeats,
		m_tagMusicInfoEP.OMSInfo.sDivision,
		m_tagMusicInfoEP.OMSInfo.sShift);
	(*ofs) << str;

	/* データチャンク作成 */
	(*ofs) << "OMdt\n";

	// ノーツ情報
	(*ofs) << "NOTE:\n";
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumNotes; i++)
	{
		sprintf_s(str, 256, "%d %d %d %d\n",
			m_tagMusicInfoEP.NoteDatas[i].iEventTime,
			m_tagMusicInfoEP.NoteDatas[i].wSoflanNo,
			m_tagMusicInfoEP.NoteDatas[i].byNoteType,
			m_tagMusicInfoEP.NoteDatas[i].byLaneNo);
		(*ofs) << str;
	}
	// ノーツ終端コード
	(*ofs) << END_OF_NOTE << "\n";

	// BPM変更情報
	(*ofs) << "SOFLAN:\n";
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumSoflans; i++)
	{
		sprintf_s(str, 256, "%d %.2f %.2f\n",
			m_tagMusicInfoEP.SoflanDatas[i].iEventTime,
			m_tagMusicInfoEP.SoflanDatas[i].fBPM,
			m_tagMusicInfoEP.SoflanDatas[i].fTick);
		(*ofs) << str;
	}

	// 拍子変更情報
	(*ofs) << "BEAT:\n";
	for (UINT i = 0; i < m_tagMusicInfoEP.OMSInfo.wNumBeats; i++)
	{
		sprintf_s(str, 256, "%d %d %d\n",
			m_tagMusicInfoEP.BeatDatas[i].iEventTime,
			m_tagMusicInfoEP.BeatDatas[i].byNumerator,
			m_tagMusicInfoEP.BeatDatas[i].byDenominator);
		(*ofs) << str;
	}
}

void EditDataManager::WriteVRShootOMT(std::ofstream * ofs)
{
	// タイプ名
	(*ofs) << "VR_SHOOT\n";

	char str[256];

	// 最大角度出力
	//sprintf_s(str, 256, "%d\n", m_tagMusicInfoVRShoot.iMaxDegree);
	//(*ofs) << str;

	// OMSフォーマット出力
	sprintf_s(str, 256, "%d %d %d %d %d\n",
		m_tagMusicInfoVRShoot.OMSInfo.byPlayMusicNo,
		m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans,
		m_tagMusicInfoVRShoot.OMSInfo.wNumBeats,
		m_tagMusicInfoVRShoot.OMSInfo.sDivision,
		m_tagMusicInfoVRShoot.OMSInfo.sShift);
	(*ofs) << str;

	/* データチャンク作成 */
	(*ofs) << "OMdt\n";

	// ノーツ情報
	(*ofs) << "NOTE:\n";
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumNotes; i++)
	{
		sprintf_s(str, 256, "%d %d %d %.2f %.2f %d\n",
			m_tagMusicInfoVRShoot.NoteDatas[i].iEventTime,
			m_tagMusicInfoVRShoot.NoteDatas[i].wSoflanNo,
			m_tagMusicInfoVRShoot.NoteDatas[i].byNoteType,
			m_tagMusicInfoVRShoot.NoteDatas[i].fDegree,
			m_tagMusicInfoVRShoot.NoteDatas[i].fRateY,
			m_tagMusicInfoVRShoot.NoteDatas[i].wCNEndIndex);
		(*ofs) << str;
	}
	// ノーツ終端コード
	(*ofs) << END_OF_NOTE << "\n";

	// BPM変更情報
	(*ofs) << "SOFLAN:\n";
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumSoflans; i++)
	{
		sprintf_s(str, 256, "%d %.2f %.2f\n",
			m_tagMusicInfoVRShoot.SoflanDatas[i].iEventTime,
			m_tagMusicInfoVRShoot.SoflanDatas[i].fBPM,
			m_tagMusicInfoVRShoot.SoflanDatas[i].fTick);
		(*ofs) << str;
	}

	// 拍子変更情報
	(*ofs) << "BEAT:\n";
	for (UINT i = 0; i < m_tagMusicInfoVRShoot.OMSInfo.wNumBeats; i++)
	{
		sprintf_s(str, 256, "%d %d %d\n",
			m_tagMusicInfoVRShoot.BeatDatas[i].iEventTime,
			m_tagMusicInfoVRShoot.BeatDatas[i].byNumerator,
			m_tagMusicInfoVRShoot.BeatDatas[i].byDenominator);
		(*ofs) << str;
	}
}