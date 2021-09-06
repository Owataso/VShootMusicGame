#include "TDNLIB.h"
#include "MusicInfo.h"
#include "BeatManager.h"
#include "SoflanManager.h"

//sList *sList::roots = nullptr;

//*****************************************************************************************************************************
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
BeatManager::BeatManager() :m_Beats(nullptr), m_iBeatStep(0), m_iNumBeats(0), m_pSoflanMgr(nullptr){}

void BeatManager::Initialize(SoflanManager *pSoflanMgr)
{
	m_pSoflanMgr = pSoflanMgr;
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
void BeatManager::Clear()
{
	if (m_Beats)
	{
		delete[] m_Beats;
		m_Beats = nullptr;
	}
}

BeatManager::~BeatManager()
{
	Clear();
}

//*****************************************************************************************************************************
//		更新
//*****************************************************************************************************************************
//------------------------------------------------------
//		更新
//------------------------------------------------------
bool BeatManager::Update(DWORD dwCurrentPosition)
{
	bool bChangedBeat(false);

	if (m_iBeatStep < m_iNumBeats - 1)
	{
		int position = (int)((dwCurrentPosition - m_Beats[m_iBeatStep + 1].iEventTime));
		if (position >= 0)
		{
			// カーソルを次に
			m_iBeatStep++;

			// 例外チェック
			assert(m_iBeatStep < m_iNumBeats);

			// チェンジ後のBPMセット
			bChangedBeat = true;
		}
	}

	return bChangedBeat;
}


//*****************************************************************************************************************************
//		セットゲット
//*****************************************************************************************************************************

void BeatManager::Set(BaseMusicInfo *info)
{
	// 例外処理(SoflanManagerのSetを先に呼んでください)
	assert(m_pSoflanMgr->GetSoflans());

	// 既存のバッファ開放
	Clear();

	m_iNumBeats = info->OMSInfo.wNumBeats;
	m_Beats = new BeatData[m_iNumBeats];

	// ノーツ情報をリストに入れていく
	for (UINT i = 0; i < m_iNumBeats; i++)
	{
		memcpy_s(&m_Beats[i], sizeof(BeatData), &info->BeatDatas[i], sizeof(BeatData));

		//for (UINT j = info->OMSInfo.wNumSoflans - 1; j >= 0; j--)
		//{
		//	if (info->BeatDatas[i].iEventTime >= info->SoflanDatas[j].iEventTime)	// >ではなく、>=にしないとソフランがずれる
		//	{
		//		const float tick((60 / m_pSoflanMgr->GetSoflans()[j].fBPM / info->OMSInfo.sDivision) * 1000);
		//		const int val(info->BeatDatas[i].iEventTime - info->SoflanDatas[j].iEventTime);
		//		m_Beats[i].iEventTime = (int)(val * tick) + m_pSoflanMgr->GetSoflans()[j].iEventTime;	// ソフランのイベント時間+そっからの自分の時間的な何か
		//		break;
		//	}
		//}
	}

	m_iBeatStep = 0;
}

BeatData *BeatManager::GetBeatData()
{
	return &m_Beats[m_iBeatStep];
}
