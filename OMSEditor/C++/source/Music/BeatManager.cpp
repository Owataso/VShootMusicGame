#include "TDNLIB.h"
#include "MusicInfo.h"
#include "BeatManager.h"
#include "SoflanManager.h"

//sList *sList::roots = nullptr;

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
BeatManager::BeatManager() :m_Beats(nullptr), m_iBeatStep(0), m_iNumBeats(0), m_pSoflanMgr(nullptr){}

void BeatManager::Initialize(SoflanManager *pSoflanMgr)
{
	m_pSoflanMgr = pSoflanMgr;
}

//------------------------------------------------------
//	���
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
//		�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//		�X�V
//------------------------------------------------------
bool BeatManager::Update(DWORD dwCurrentPosition)
{
	bool bChangedBeat(false);

	if (m_iBeatStep < m_iNumBeats - 1)
	{
		int position = (int)((dwCurrentPosition - m_Beats[m_iBeatStep + 1].iEventTime));
		if (position >= 0)
		{
			// �J�[�\��������
			m_iBeatStep++;

			// ��O�`�F�b�N
			assert(m_iBeatStep < m_iNumBeats);

			// �`�F���W���BPM�Z�b�g
			bChangedBeat = true;
		}
	}

	return bChangedBeat;
}


//*****************************************************************************************************************************
//		�Z�b�g�Q�b�g
//*****************************************************************************************************************************

void BeatManager::Set(BaseMusicInfo *info)
{
	// ��O����(SoflanManager��Set���ɌĂ�ł�������)
	assert(m_pSoflanMgr->GetSoflans());

	// �����̃o�b�t�@�J��
	Clear();

	m_iNumBeats = info->OMSInfo.wNumBeats;
	m_Beats = new BeatData[m_iNumBeats];

	// �m�[�c�������X�g�ɓ���Ă���
	for (UINT i = 0; i < m_iNumBeats; i++)
	{
		memcpy_s(&m_Beats[i], sizeof(BeatData), &info->BeatDatas[i], sizeof(BeatData));

		//for (UINT j = info->OMSInfo.wNumSoflans - 1; j >= 0; j--)
		//{
		//	if (info->BeatDatas[i].iEventTime >= info->SoflanDatas[j].iEventTime)	// >�ł͂Ȃ��A>=�ɂ��Ȃ��ƃ\�t�����������
		//	{
		//		const float tick((60 / m_pSoflanMgr->GetSoflans()[j].fBPM / info->OMSInfo.sDivision) * 1000);
		//		const int val(info->BeatDatas[i].iEventTime - info->SoflanDatas[j].iEventTime);
		//		m_Beats[i].iEventTime = (int)(val * tick) + m_pSoflanMgr->GetSoflans()[j].iEventTime;	// �\�t�����̃C�x���g����+��������̎����̎��ԓI�ȉ���
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
