#include "TDNLIB.h"
#include "OMS_Loader.h"
#include "../Music/MusicInfo.h"
#include "PlayCursor.h"

//*****************************************************************************************************************************
//	OMSファイル読み込み
//*****************************************************************************************************************************


//OMS_RESULT OMSLoader::LoadOMS(MusicInfo *pOut, LPCSTR lpcFileName)
//{
//	// 読み出しくん
//	std::ifstream ifs(lpcFileName, std::ios::binary);
//	//MyAssert(ifs,"OMSファイルオープンに失敗");
//	if (!ifs) return OMS_RESULT::FAILED_OPEN;
//
//	/* ヘッダチャンク侵入 */
//	char l_cChunkID[5];
//	ifs.read(l_cChunkID, 4);										// ヘッダチャンクであることを示す"OMhd"という4文字のアスキーコード
//	if (
//		l_cChunkID[0] != 'O' ||
//		l_cChunkID[1] != 'M' ||
//		l_cChunkID[2] != 'h' ||
//		l_cChunkID[3] != 'd'
//		)
//		return OMS_RESULT::FAILED_READ_OMS;
//
//	// ★データのバージョン
//	BYTE l_byOMSType;
//	ifs.read((char*)&l_byOMSType, sizeof(BYTE));
//
//	//switch ((OMS_TYPE)l_byOMSType)
//	//{
//	//case OMS_TYPE::BASE:
//	//	ifs.read((char*)&out->tagOMSInfo, sizeof(BaseOMSInfo));
//	//	break;
//	//default:
//	//	break;
//	//}
//
//	/* データチャンク侵入 */
//	//ifs.read(work, 4);										// データチャンクであることを示す"OMdt"という4文字のアスキーコード
//	//assert(
//	//	work[0] == 'O'&&
//	//	work[1] == 'M'&&
//	//	work[2] == 'd'&&
//	//	work[3] == 't'
//	//	);
//
//	// まずノート
//	//out->tagNotes = new NoteInfo[out->tagOMSInfo.wNumNotes];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumNotes; i++)
//	//{
//	//	ifs.read((char*)&out->tagNotes, sizeof(NoteInfo));
//	//}
//
//	//// BPM変更情報
//	//out->tagSoflans = new SoflanInfo[out->tagOMSInfo.wNumSoflans];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumSoflans; i++)
//	//{
//	//	ifs.read((char*)&out->tagSoflans, sizeof(SoflanInfo));
//	//}
//
//	//// 拍子情報
//	//out->tagBeats = new BeatInfo[out->tagOMSInfo.wNumBeats];
//	//for (UINT i = 0; i < out->tagOMSInfo.wNumBeats; i++)
//	//{
//	//	ifs.read((char*)&out->tagBeats, sizeof(BeatInfo));
//	//}
//
//	return OMS_RESULT::OK;
//}


OMS_RESULT EditOMSLoader::LoadBaseOMS(MusicInfoEP *pOut, std::ifstream *ifs)
{

	// OMSフォーマット読み込み
	ifs->read((char*)&pOut->OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ifs->read((char*)&pOut->OMSInfo.wNumNotes, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumSoflans, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumBeats, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.sDivision, sizeof(short));
	ifs->read((char*)&pOut->OMSInfo.sShift, sizeof(short));

	char l_cChunkID[5];

	/* データチャンク侵入 */
	ifs->read(l_cChunkID, 4);										// データチャンクであることを示す"OMdt"という4文字のアスキーコード
	if(
		l_cChunkID[0] != 'O'||
		l_cChunkID[1] != 'M'||
		l_cChunkID[2] != 'd'||
		l_cChunkID[3] != 't'
	)return OMS_RESULT::FAILED_READ_OMS;

	// まずノート
	pOut->NoteDatas = new NoteDataEP[pOut->OMSInfo.wNumNotes];
	for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
	{
		ifs->read((char*)&pOut->NoteDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->NoteDatas[i].wSoflanNo, sizeof(WORD));
		ifs->read((char*)&pOut->NoteDatas[i].byNoteType, sizeof(BYTE));
		ifs->read((char*)&pOut->NoteDatas[i].byLaneNo, sizeof(BYTE));
	}

	// BPM変更情報
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		ifs->read((char*)&pOut->SoflanDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->SoflanDatas[i].fBPM, sizeof(float));
		ifs->read((char*)&pOut->SoflanDatas[i].fTick, sizeof(float));
	}

	// 拍子情報
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		ifs->read((char*)&pOut->BeatDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->BeatDatas[i].byNumerator, sizeof(BYTE));
		ifs->read((char*)&pOut->BeatDatas[i].byDenominator, sizeof(BYTE));
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadVRShootOMS(MusicInfoVRShoot * pOut, std::ifstream * ifs)
{
	// OMSフォーマット読み込み
	ifs->read((char*)&pOut->OMSInfo.byPlayMusicNo, sizeof(BYTE));
	ifs->read((char*)&pOut->OMSInfo.wNumNotes, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumSoflans, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.wNumBeats, sizeof(WORD));
	ifs->read((char*)&pOut->OMSInfo.sDivision, sizeof(short));
	ifs->read((char*)&pOut->OMSInfo.sShift, sizeof(short));

	char l_cChunkID[5];

	/* データチャンク侵入 */
	ifs->read(l_cChunkID, 4);										// データチャンクであることを示す"OMdt"という4文字のアスキーコード
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// まずノート
	pOut->NoteDatas = new NoteDataVRShoot[pOut->OMSInfo.wNumNotes];
	for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
	{
		ifs->read((char*)&pOut->NoteDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->NoteDatas[i].wSoflanNo, sizeof(WORD));
		ifs->read((char*)&pOut->NoteDatas[i].byNoteType, sizeof(BYTE));
		ifs->read((char*)&pOut->NoteDatas[i].fDegree, sizeof(float));
		ifs->read((char*)&pOut->NoteDatas[i].fRateY, sizeof(float));
		ifs->read((char*)&pOut->NoteDatas[i].wCNEndIndex, sizeof(WORD));
	}

	// BPM変更情報
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		ifs->read((char*)&pOut->SoflanDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->SoflanDatas[i].fBPM, sizeof(float));
		ifs->read((char*)&pOut->SoflanDatas[i].fTick, sizeof(float));
	}

	// 拍子情報
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		ifs->read((char*)&pOut->BeatDatas[i].iEventTime, sizeof(int));
		ifs->read((char*)&pOut->BeatDatas[i].byNumerator, sizeof(BYTE));
		ifs->read((char*)&pOut->BeatDatas[i].byDenominator, sizeof(BYTE));
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadBaseOMT(MusicInfoEP * pOut, std::ifstream * ifs)
{
	// int以外の整数型(BYTE)とかの読み込みに使う(これがないと値がおかしくなる)
	int l_iValue;

	// OMSフォーマット読み込み
	(*ifs) >> l_iValue;
	pOut->OMSInfo.byPlayMusicNo = (BYTE)l_iValue;

	// ノーツは可変長にする
	//(*ifs) >> pOut->OMSInfo.wNumNotes;

	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumSoflans = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumBeats = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sDivision = (short)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sShift = (short)l_iValue;

	char l_cChunkID[16];

	/* データチャンク侵入 */

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'N' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'T' ||
		l_cChunkID[3] != 'E' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// まずノート(書き換えやすいように可変長にする)
	{
		std::vector<NoteDataEP> notes;

		// 終端コードまで無限ループ
		while(true)
		{
			NoteDataEP l_append;

			(*ifs) >> l_append.iEventTime;

			// ★ノーツ終端コード(-1)
			if (l_append.iEventTime == END_OF_NOTE)
			{
				// 領域確保
				pOut->OMSInfo.wNumNotes = (WORD)notes.size();
				pOut->NoteDatas = new NoteDataEP[pOut->OMSInfo.wNumNotes];

				// データコピー
				for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
				{
					memcpy_s(&pOut->NoteDatas[i], sizeof(NoteDataEP), &notes[i], sizeof(NoteDataEP));
				}
				notes.clear();

				// ループ抜ける
				break;
			}

			(*ifs) >> l_iValue;
			l_append.wSoflanNo = (WORD)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byNoteType = (BYTE)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byLaneNo = (BYTE)l_iValue;

			// リスト追加
			notes.push_back(l_append);
		}
	}

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'S' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'F' ||
		l_cChunkID[3] != 'L' ||
		l_cChunkID[4] != 'A' ||
		l_cChunkID[5] != 'N' ||
		l_cChunkID[6] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// BPM変更情報
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		(*ifs) >> pOut->SoflanDatas[i].iEventTime;
		(*ifs) >> pOut->SoflanDatas[i].fBPM;
		(*ifs) >> pOut->SoflanDatas[i].fTick;
	}

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'B' ||
		l_cChunkID[1] != 'E' ||
		l_cChunkID[2] != 'A' ||
		l_cChunkID[3] != 'T' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// 拍子情報
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		(*ifs) >> pOut->BeatDatas[i].iEventTime;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byNumerator = (BYTE)l_iValue;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byDenominator = (BYTE)l_iValue;
	}

	return OMS_RESULT::OK;
}

OMS_RESULT EditOMSLoader::LoadVRShootOMT(MusicInfoVRShoot * pOut, std::ifstream * ifs)
{
	// int以外の整数型(BYTE)とかの読み込みに使う(これがないと値がおかしくなる)
	int l_iValue;

	// 最大角度読み込み
	//(*ifs) >> l_iValue;
	//pOut->iMaxDegree = l_iValue;

	// OMSフォーマット読み込み
	(*ifs) >> l_iValue;
	pOut->OMSInfo.byPlayMusicNo = (BYTE)l_iValue;

	// ノーツは可変長にする
	//(*ifs) >> pOut->OMSInfo.wNumNotes;

	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumSoflans = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.wNumBeats = (WORD)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sDivision = (short)l_iValue;
	(*ifs) >> l_iValue;
	pOut->OMSInfo.sShift = (short)l_iValue;

	char l_cChunkID[16];

	/* データチャンク侵入 */

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'O' ||
		l_cChunkID[1] != 'M' ||
		l_cChunkID[2] != 'd' ||
		l_cChunkID[3] != 't'
		)return OMS_RESULT::FAILED_READ_OMS;

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'N' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'T' ||
		l_cChunkID[3] != 'E' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// まずノート(書き換えやすいように可変長にする)
	{
		std::vector<NoteDataVRShoot> notes;

		// 終端コードまで無限ループ
		while (true)
		{
			NoteDataVRShoot l_append;

			(*ifs) >> l_append.iEventTime;

			// ★ノーツ終端コード(-1)
			if (l_append.iEventTime == END_OF_NOTE)
			{
				// 領域確保
				pOut->OMSInfo.wNumNotes = (WORD)notes.size();
				pOut->NoteDatas = new NoteDataVRShoot[pOut->OMSInfo.wNumNotes];

				// データコピー
				for (WORD i = 0; i < pOut->OMSInfo.wNumNotes; i++)
				{
					memcpy_s(&pOut->NoteDatas[i], sizeof(NoteDataVRShoot), &notes[i], sizeof(NoteDataVRShoot));
				}
				notes.clear();

				// ループ抜ける
				break;
			}

			(*ifs) >> l_iValue;
			l_append.wSoflanNo = (WORD)l_iValue;
			(*ifs) >> l_iValue;
			l_append.byNoteType = (BYTE)l_iValue;
			(*ifs) >> l_append.fDegree;
			(*ifs) >> l_append.fRateY;
			(*ifs) >> l_iValue;
			l_append.wCNEndIndex = (WORD)l_iValue;

			// リスト追加
			notes.push_back(l_append);
		}
	}

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'S' ||
		l_cChunkID[1] != 'O' ||
		l_cChunkID[2] != 'F' ||
		l_cChunkID[3] != 'L' ||
		l_cChunkID[4] != 'A' ||
		l_cChunkID[5] != 'N' ||
		l_cChunkID[6] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// BPM変更情報
	pOut->SoflanDatas = new SoflanData[pOut->OMSInfo.wNumSoflans];
	for (WORD i = 0; i < pOut->OMSInfo.wNumSoflans; i++)
	{
		(*ifs) >> pOut->SoflanDatas[i].iEventTime;
		(*ifs) >> pOut->SoflanDatas[i].fBPM;
		(*ifs) >> pOut->SoflanDatas[i].fTick;
	}

	// チャンクチェック
	(*ifs) >> l_cChunkID;
	if (
		l_cChunkID[0] != 'B' ||
		l_cChunkID[1] != 'E' ||
		l_cChunkID[2] != 'A' ||
		l_cChunkID[3] != 'T' ||
		l_cChunkID[4] != ':'
		)return OMS_RESULT::FAILED_READ_OMS;

	// 拍子情報
	pOut->BeatDatas = new BeatData[pOut->OMSInfo.wNumBeats];
	for (WORD i = 0; i < pOut->OMSInfo.wNumBeats; i++)
	{
		(*ifs) >> pOut->BeatDatas[i].iEventTime;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byNumerator = (BYTE)l_iValue;
		(*ifs) >> l_iValue;
		pOut->BeatDatas[i].byDenominator = (BYTE)l_iValue;
	}

	return OMS_RESULT::OK;
}
