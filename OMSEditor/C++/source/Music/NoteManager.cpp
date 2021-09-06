#include "tdnlib.h"
#include "../Sound/SoundManager.h"
#include "MusicInfo.h"
#include "NoteManager.h"
#include "PlayCursor.h"
#include "SoflanManager.h"
#include "../Data/PlayerData.h"
#include "../Judge/JudgeManager.h"

//*****************************************************************************************************************************
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
NoteManager::NoteManager(): m_iNumNorts(0), m_pJudgeMgr(nullptr), m_pSoflanMgr(nullptr)
{}

void NoteManager::Initialize(SoflanManager *pSoflanMgr, JudgeManager *pJudgeMgr)
{
	m_pSoflanMgr = pSoflanMgr;
	m_pJudgeMgr = pJudgeMgr;

	/*
	分能値は、４分音符が何ティックかを設定します。例えば、タイムベースが120なら８分音符は60ティック、三連８分音符は40ティック、全音符は480ティックになります。
	分能値は、通常120、196、240、480などが使われます。他の値を設定しても構いませんが、シーケンサによってはサポートしていない場合があります。
	１ティックの時間(秒) = 60 / テンポ / タイムベース

	例えば、テンポ120でタイムベースが480ならば1ティックは1 / 960秒、約１ミリ秒になります。
	*/

	// リスト配列の動的確保
	m_NoteList.clear();
}
//------------------------------------------------------
//	解放
//------------------------------------------------------
NoteManager::~NoteManager()
{
	Clear();
}

void NoteManager::Clear()
{
	for (auto it = m_NoteList.begin(); it != m_NoteList.end();)
	{
		delete (*it);
		it = m_NoteList.erase(it);
	}
	m_NoteList.clear();
}

//*****************************************************************************************************************************gwq
//		更新
//*****************************************************************************************************************************
//------------------------------------------------------
//		更新
//------------------------------------------------------
void NoteManager::Update(DWORD dwCurrentPosition)
{
	// 更新分岐
	isVR ? VRShootUpdate(dwCurrentPosition) : EPUpdate(dwCurrentPosition);
}

void NoteManager::EditUpdate(DWORD dwCurrentPosition)
{
	// 座標更新するだけ
	for (auto it : m_NoteList)
	{
		it->UpdatePos(dwCurrentPosition);
	}
}

void NoteManager::EPUpdate(DWORD dwCurrentPosition)
{
	for (auto it = m_NoteList.begin(); it != m_NoteList.end();)
	{
		(*it)->UpdatePos(dwCurrentPosition);

		// 最適化
		if ((*it)->iPosition < -5000) break;

		// 消去チェック
		if ((*it)->bErase)
		{
			//InputMIDIMgr->SetTone((*it)->note.tone);
			delete (*it);
			it = m_NoteList.erase(it);
		}

		// DJAUTO
		else if ((*it)->iPosition > -16)
		{
			// 判定強制SUPER
			m_pJudgeMgr->JudgeNote((*it)->pNoteEP->byLaneNo);
			//delete (*it);
			//it = m_NoteList.erase(it);
			break;
		}
		else
		{
			// ノーツ座標計算
			it++;
		}
	}
}

void NoteManager::VRShootUpdate(DWORD dwCurrentPosition)
{
	for (auto it = m_NoteList.begin(); it != m_NoteList.end();)
	{
		(*it)->UpdatePos(dwCurrentPosition);

		// 最適化
		if ((*it)->iPosition < -5000) break;

		// 消去チェック
		if ((*it)->bErase)
		{
			//InputMIDIMgr->SetTone((*it)->note.tone);
			delete (*it);
			it = m_NoteList.erase(it);
		}

		// DJAUTO
		else if ((*it)->iPosition > -16)
		{
			// 判定強制SUPER
			m_pJudgeMgr->JudgeNote((*it));
			//delete (*it);
			//it = m_NoteList.erase(it);
			break;
		}
		else
		{
			// ノーツ座標計算
			it++;
		}
	}
}

//*****************************************************************************************************************************
//		セットゲット
//*****************************************************************************************************************************
std::list<PlayNoteInfo*> *NoteManager::GetList()
{
	return &m_NoteList;
}
void NoteManager::GetNoteAbsolutePos(const PlayNoteInfo &note, int *pos_y)
{
	*pos_y = note.iPosition;
}

void NoteManager::GetNoteHiSpeedPos(const PlayNoteInfo &note, int *pos_y)
{
	*pos_y = note.iPosition;

	// BPMによる影響
	//if (!PlayerDataMgr->m_PlayerOption.bRegulSpeed)
		*pos_y = (int)(*pos_y / m_pSoflanMgr->GetSoflans()[note.pNoteEP->wSoflanNo].fTick);
	// ハイスピードによる影響
	const float speed = /*PlayerDataMgr->m_PlayerOption.HiSpeed * */HI_SPEED_WIDTH;

	//static int type(0);
	//if (KeyBoardTRG(KB_1)) type = 0;
	//if (KeyBoardTRG(KB_2)) type = 1;
	//if (KeyBoardTRG(KB_3)) type = 2;
	//if (KeyBoardTRG(KB_4)) type = 3;

	//switch (type)
	//{
	//case 0:
		*pos_y = (int)(*pos_y * (.1f + (speed / .1f * .01f)));
	//	break;
	//case 1:
	//	*pos_y = (int)(*pos_y * (.1f + (speed / .75f * .1f)));
	//	break;
	//case 2:
	//	*pos_y = (int)(*pos_y * (.1f + (speed / .5f * .05f)));
	//	break;
	//case 3:
	//	*pos_y = (int)(*pos_y * (.1f + (speed / .75f * .1f)));
	//	break;
	//}
}

void NoteManager::Set(MusicInfoEP *info)
{
	// 例外処理(SoflanManagerのSetを先に呼んでください)
	assert(m_pSoflanMgr->GetSoflans());

	// ベースオクターブ保存
	//m_cBaseOctave = info.omsInfo.BaseOctave;

	// 既存のバッファ開放
 	Clear();

	m_iNumNorts = info->OMSInfo.wNumNotes;

	isVR = false;

	// ノーツ情報をリストに入れていく
	for (UINT i = 0; i < m_iNumNorts; i++)
	{
		PlayNoteInfo *set = new PlayNoteInfo;
		set->bErase = false;
		//set->pNote = std::make_shared<NoteDataEP>();
		//memcpy_s(set->pNote.get(), sizeof(NoteDataEP), &info->NoteDatas[i], sizeof(NoteDataEP));
		set->pNoteEP = &info->NoteDatas[i];
		set->iPosition = -114514;
		set->wIndex = i;

		// ソフラン対応用のイベントタイム配列作成
		//set->EventTimeArray = new UINT[m_info->omsInfo.NumSoflans];
		//memcpy_s(set->EventTimeArray, sizeof(UINT)*m_info->omsInfo.NumSoflans, &set->note->iEventTime, sizeof(UINT)*m_info->omsInfo.NumSoflans);

		//float tick = (60 / m_SoflanMgr->m_soflans[0].fBPM / m_info->omsInfo.division) * 1000;
		//set->EventTimeArray[0] = (int)(note->iEventTime * tick);
		//UINT debug[300];
		
		// エディットの方で計算するようにした
		//for (UINT j = info->OMSInfo.wNumSoflans - 1; j >= 0; j--)
		//{
		//	if (info->NoteDatas[i].iEventTime >= info->SoflanDatas[j].iEventTime)	// >ではなく、>=にしないとソフランがずれる
		//	{
		//		const float tick((60 / m_pSoflanMgr->GetSoflans()[j].fBPM / info->OMSInfo.sDivision) * 1000);
		//		const int val(info->NoteDatas[i].iEventTime - info->SoflanDatas[j].iEventTime);
		//		set->pNote->iEventTime = (int)(val * tick) + m_pSoflanMgr->GetSoflans()[j].iEventTime;	// ソフランのイベント時間+そっからの自分の時間的な何か
		//		set->iSoflanNo = j;

		//		//tdnDebug::OutPutLog("ノート%d: %d\n j: %d\n Soflan[j]: %d\n tick: %.1f\n BPM: %.1f\n", i, info->NoteDatas[i].iEventTime, j, m_pSoflanMgr->GetSoflans()[j].iEventTime, tick, m_pSoflanMgr->GetSoflans()[j].fBPM);
		//		break;
		//	}
		//}

		//int lane = info->notes[i].cLaneNo - m_cBaseOctave;	// ノーツのレーン

		m_NoteList.push_back(set);
	}

	//int i = 1;
	//for(auto it : m_NoteList)
	//{
	//	tdnDebug::OutPutLog("ノート%d: %d\n", i, it->pNote->iEventTime);
	//}


	// 変化後の差(このまま放っておくとBPMが変わった際ずれる。)
	//int *shift = new int[m_info->omsInfo.NumSoflans];
	//
	//// 変化前の差(BPMが変わる前の差を保存)
	//int *shift2 = new int[m_info->omsInfo.NumSoflans];
	//
	//// 0は使わない
	//shift[0] = shift2[0] = 0;
	//
	//for (UINT i = 1; i < m_info->omsInfo.NumSoflans; i++)
	//{
	//	// 変化後の差倍率
	//	float tick2 = (60 / m_SoflanMgr->m_soflans[i].fBPM / m_info->omsInfo.division) * 1000;
	//
	//	// 変化前の差倍率
	//	float tick = (60 / m_SoflanMgr->m_soflans[i - 1].fBPM / m_info->omsInfo.division) * 1000;
	//
	//	// ソフラン開始から終わりまでの差を保存
	//	int val = m_SoflanMgr->m_soflans[i].iEventTime - m_SoflanMgr->m_soflans[i - 1].iEventTime;
	//
	//	// 変わる前と変わった後の倍率それぞれを掛ける
	//	shift[i] = (int)(val * tick2);
	//	shift2[i] = (int)(val * tick);
	//}
	//for (UINT i = 1; i < m_info->omsInfo.NumSoflans; i++)
	//{
	//	for (auto it : nList)
	//	{
	//		if (it->note->iEventTime <= m_SoflanMgr->m_soflans[i].iEventTime)
	//			continue;
	//
	//		//it->EventTimeArray[i] = it->note->iEventTime;
	//		for (int j = i; j >= 0; j--)
	//		{
	//			it->EventTimeArray[i] = it->EventTimeArray[i] - shift[j] + shift2[j];
	//			if (j == 2)
	//			{
	//				int val = it->EventTimeArray[i] - m_SoflanMgr->m_soflans[j].iEventTime;
	//				float tick2 = (60 / m_SoflanMgr->m_soflans[j].fBPM / m_info->omsInfo.division) * 1000;
	//				it->EventTimeArray[i] = it->EventTimeArray[i] - val + (int)(val * tick2);
	//			}
	//		}
	//	}
	//}
	//
	//delete shift;
	//delete shift2;
}

void NoteManager::Set(MusicInfoVRShoot * info, DWORD dwOffset)
{
	// 例外処理(SoflanManagerのSetを先に呼んでください)
	assert(m_pSoflanMgr->GetSoflans());

	// 既存のバッファ開放
	Clear();

	m_iNumNorts = info->OMSInfo.wNumNotes;
	std::vector<NoteDataVRShoot> list;
	UINT iOffsetIndex = 0;
	for (UINT i = 0; i < m_iNumNorts; i++)
	{
		if (info->NoteDatas[i].iEventTime >= (int)dwOffset)
		{
			iOffsetIndex = i;
			break;
		}
	}

	isVR = true;

	// ノーツ情報をリストに入れていく
	for (UINT i = iOffsetIndex; i < m_iNumNorts; i++)
	{
		PlayNoteInfo *set = new PlayNoteInfo;
		set->bErase = false;
		set->pNoteVRShoot = &info->NoteDatas[i];
		set->iPosition = -114514;
		set->wIndex = i;
		m_NoteList.push_back(set);
	}
}

void NoteManager::GetStartNotesLane(std::vector<int> *pOutLanes)
{
	//UINT iMinEventTime(INT_MAX);

	//// 全ノーツから、最初に押されるノーツを探す
	//	// このレーンには何も降ってこない場合の処理
	//if (m_NoteList.empty())

	//// レーンの一番最初のノーツ(最初のイベント時間の計測だけでいいので、2個目以降のレーンのノーツはいらない)
	//auto LaneFirstNote = m_NoteList.begin();

	//const UINT iNoteEventTime((*LaneFirstNote)->Note.iEventTime);
	//// 最短時間より短かったら
	//if (iNoteEventTime < iMinEventTime)
	//{
	//	// ノーツ開始最短時間更新
	//	iMinEventTime = iNoteEventTime;

	//	pOutLanes->clear();
	//	pOutLanes->push_back(i);
	//}

	//// 同時押しにも対応
	//else if (iNoteEventTime == iMinEventTime) pOutLanes->push_back(i);
}

void PlayNoteInfo::UpdatePos(DWORD dwCurrentPosition)
{
	if (pNoteEP) iPosition = dwCurrentPosition - pNoteEP->iEventTime;
	else if (pNoteVRShoot)iPosition = dwCurrentPosition - pNoteVRShoot->iEventTime;
}
