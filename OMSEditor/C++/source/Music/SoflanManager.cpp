#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "MusicInfo.h"
#include "PlayCursor.h"
#include "SoflanManager.h"

//sList *sList::roots = nullptr;

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
SoflanManager::SoflanManager() :m_Soflans(nullptr){}

//------------------------------------------------------
//	���
//------------------------------------------------------
SoflanManager::~SoflanManager()
{
	if (m_Soflans)
	{
		delete[] m_Soflans;
		m_Soflans = nullptr;
	}
}

//*****************************************************************************************************************************
//		�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//		�X�V
//------------------------------------------------------
bool SoflanManager::Update(DWORD dwCurrentPosition)
{
	bool bChangedBPM(false);

	if (m_iSoflanStep < m_iNumSoflans - 1)
	{
		int position = (int)((dwCurrentPosition - m_Soflans[m_iSoflanStep + 1].iEventTime));
		if (position >= 0)
		{
			// �J�[�\��������
			m_iSoflanStep++;

			// ��O�`�F�b�N
			assert(m_iSoflanStep < m_iNumSoflans);

			// �`�F���W���BPM�Z�b�g
			bChangedBPM = true;
		}
	}

	return bChangedBPM;
	//for (sList *e = sList::roots; e != nullptr;)
	//{
	//	sList* next = e->next;
	//	// �X�V
	//	int position = (int)((note_mng->GetMsecond() - e->instance->event_time * note_mng->tick));
	//
	//	extern int BPMflag;
	//	if(BPMflag == 2)position -= 290;
	//
	//	if (position > 0)
	//	{
	//		bpm = e->instance->bpm;
	//		note_mng->SetBPM(bpm);
	//		delete e;
	//	}
	//	e = next;
	//}
}


//*****************************************************************************************************************************
//		�Z�b�g�Q�b�g
//*****************************************************************************************************************************
float SoflanManager::GetBPM()
{
	return m_Soflans[m_iSoflanStep].fBPM;
}

void SoflanManager::Set(BaseMusicInfo *info)
{
	// �\�t������
	m_iNumSoflans = info->OMSInfo.wNumSoflans;

	if (m_Soflans) delete[] m_Soflans;
	m_Soflans = new SoflanData[m_iNumSoflans];

	for (UINT i = 0; i < m_iNumSoflans; i++)
	{
		memcpy_s(&m_Soflans[i], sizeof(SoflanData), &info->SoflanDatas[i], sizeof(SoflanData));
	}

	/*
	
	BPM���ω�����ƃm�[�g�����������艺�ɂ���Ă��܂��B
	�����͔{�����ς�������ƂŁAtick�l���ς��̂͂����Ƃ��āA�{�����ς��ȑO�̎��Ԃɂ����̔{�����������Ă��܂��̂����B
	�ς��ȑO�̎��Ԃ͕ς��O�̔{���A���ꂼ������Ă��Ȃ��Ƃ���Ă��܂�

	----------------------------------------------------------------------------

	��:

	�uBPM60�v�@�@�uBPM130�v		�uBPM260�v
	�\�\�\�\�\�\���\�\�\�\�\�\���\�\�\�\�\�\���iBPM130�̏ꍇ�j
	��100			��100		��100

	�uBPM60�v�uBPM130�v�uBPM260�v
	�\�\�\�\���\�\�\�\���\�\�\�\���iBPM260�̏ꍇ�j
	��50		��50		��50

	�����Ȃ��Ă��܂�������B



	----------------------------------------------------------------------------

			�@�uBPM60�v�@	�@�uBPM130�v	�uBPM260�v
	�\�\�\�\�\�\�\�\�\�\�\�\���\�\�\�\�\�\���\�\�\��
				��200			��100		��50

	��������̂������B



	evt[0]=0		2.08
	evt[1]=240		0.96
	evt[2]=7920		0.48

	��
	evt[1] = evt[1] * 2.08
	evt[2] = (7920 * 0.96) + evt[1]

	*/

	//// �ω���̍�(���̂܂ܕ����Ă�����BPM���ς�����ۂ����B)
	//int *shift = new int[m_info->omsInfo.NumSoflans];
	//
	//// �ω��O�̍�(BPM���ς��O�̍���ۑ�)
	//int *shift2 = new int[m_info->omsInfo.NumSoflans];
	//
	//int *val = new int[m_info->omsInfo.NumSoflans];
	//
	//// 0�͎g��Ȃ�
	//shift[0] = shift2[0] = val[0] = 0;

	//for (UINT i = 1; i < m_iNumSoflans; i++)
	//{
		//// �ω���̍��{��
		//float tick2 = (60 / m_soflans[i].bpm / m_info->omsInfo.division) * 1000;
		//
		//// �ω��O�̍��{��
		//float tick = (60 / m_soflans[i - 1].bpm / m_info->omsInfo.division) * 1000;
		//
		//// �\�t�����J�n����I���܂ł̍���ۑ�
		//val[i] = m_soflans[i].event_time - m_soflans[i - 1].event_time;
		//
		//// �ς��O�ƕς������̔{�����ꂼ����|����
		//shift[i] = (int)(m_soflans[i - 1].event_time);
		//shift2[i] = (int)(val[i] * tick);
		//
		//m_soflans[i].event_time = shift[i] + val[i];
		
		//// ����ł��܂�������
		//int val = m_Soflans[i].iEventTime - info->SoflanDatas[i - 1].iEventTime;
		//m_Soflans[i].iEventTime = (int)(val * m_fTicks[i - 1]) + m_Soflans[i - 1].iEventTime;
	//}

	//m_soflans[2].event_time = 7700;

	//float t = (60 / m_soflans[0].bpm / m_info->omsInfo.division) * 1000;
	//m_soflans[1].event_time = (int)(m_soflans[1].event_time * t);
	//t = (60 / m_soflans[1].bpm / m_info->omsInfo.division) * 1000;
	//m_soflans[2].event_time = (int)(m_soflans[2].event_time * t) + m_soflans[1].event_time;

	//for (UINT i = 1; i < m_info->omsInfo.NumSoflans; i++)
	//{
	//	//for (int j = i - 1; j >= 1; j--)
	//	//{
	//		m_soflans[i].event_time = val[i] + shift[i];
	//	//}
	//}

	//delete[] shift;
	//delete[] shift2;
	//delete[] val;

	m_iSoflanStep = 0;
}