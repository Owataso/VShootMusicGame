#include "tdnlib.h"
#include "../Sound/SoundManager.h"
#include "MusicInfo.h"
#include "NoteManager.h"
#include "PlayCursor.h"
#include "SoflanManager.h"
#include "../Data/PlayerData.h"
#include "../Judge/JudgeManager.h"

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
NoteManager::NoteManager(): m_iNumNorts(0), m_pJudgeMgr(nullptr), m_pSoflanMgr(nullptr)
{}

void NoteManager::Initialize(SoflanManager *pSoflanMgr, JudgeManager *pJudgeMgr)
{
	m_pSoflanMgr = pSoflanMgr;
	m_pJudgeMgr = pJudgeMgr;

	/*
	���\�l�́A�S�����������e�B�b�N����ݒ肵�܂��B�Ⴆ�΁A�^�C���x�[�X��120�Ȃ�W��������60�e�B�b�N�A�O�A�W��������40�e�B�b�N�A�S������480�e�B�b�N�ɂȂ�܂��B
	���\�l�́A�ʏ�120�A196�A240�A480�Ȃǂ��g���܂��B���̒l��ݒ肵�Ă��\���܂��񂪁A�V�[�P���T�ɂ���Ă̓T�|�[�g���Ă��Ȃ��ꍇ������܂��B
	�P�e�B�b�N�̎���(�b) = 60 / �e���| / �^�C���x�[�X

	�Ⴆ�΁A�e���|120�Ń^�C���x�[�X��480�Ȃ��1�e�B�b�N��1 / 960�b�A��P�~���b�ɂȂ�܂��B
	*/

	// ���X�g�z��̓��I�m��
	m_NoteList.clear();
}
//------------------------------------------------------
//	���
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
//		�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//		�X�V
//------------------------------------------------------
void NoteManager::Update(DWORD dwCurrentPosition)
{
	// �X�V����
	isVR ? VRShootUpdate(dwCurrentPosition) : EPUpdate(dwCurrentPosition);
}

void NoteManager::EditUpdate(DWORD dwCurrentPosition)
{
	// ���W�X�V���邾��
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

		// �œK��
		if ((*it)->iPosition < -5000) break;

		// �����`�F�b�N
		if ((*it)->bErase)
		{
			//InputMIDIMgr->SetTone((*it)->note.tone);
			delete (*it);
			it = m_NoteList.erase(it);
		}

		// DJAUTO
		else if ((*it)->iPosition > -16)
		{
			// ���苭��SUPER
			m_pJudgeMgr->JudgeNote((*it)->pNoteEP->byLaneNo);
			//delete (*it);
			//it = m_NoteList.erase(it);
			break;
		}
		else
		{
			// �m�[�c���W�v�Z
			it++;
		}
	}
}

void NoteManager::VRShootUpdate(DWORD dwCurrentPosition)
{
	for (auto it = m_NoteList.begin(); it != m_NoteList.end();)
	{
		(*it)->UpdatePos(dwCurrentPosition);

		// �œK��
		if ((*it)->iPosition < -5000) break;

		// �����`�F�b�N
		if ((*it)->bErase)
		{
			//InputMIDIMgr->SetTone((*it)->note.tone);
			delete (*it);
			it = m_NoteList.erase(it);
		}

		// DJAUTO
		else if ((*it)->iPosition > -16)
		{
			// ���苭��SUPER
			m_pJudgeMgr->JudgeNote((*it));
			//delete (*it);
			//it = m_NoteList.erase(it);
			break;
		}
		else
		{
			// �m�[�c���W�v�Z
			it++;
		}
	}
}

//*****************************************************************************************************************************
//		�Z�b�g�Q�b�g
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

	// BPM�ɂ��e��
	//if (!PlayerDataMgr->m_PlayerOption.bRegulSpeed)
		*pos_y = (int)(*pos_y / m_pSoflanMgr->GetSoflans()[note.pNoteEP->wSoflanNo].fTick);
	// �n�C�X�s�[�h�ɂ��e��
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
	// ��O����(SoflanManager��Set���ɌĂ�ł�������)
	assert(m_pSoflanMgr->GetSoflans());

	// �x�[�X�I�N�^�[�u�ۑ�
	//m_cBaseOctave = info.omsInfo.BaseOctave;

	// �����̃o�b�t�@�J��
 	Clear();

	m_iNumNorts = info->OMSInfo.wNumNotes;

	isVR = false;

	// �m�[�c�������X�g�ɓ���Ă���
	for (UINT i = 0; i < m_iNumNorts; i++)
	{
		PlayNoteInfo *set = new PlayNoteInfo;
		set->bErase = false;
		//set->pNote = std::make_shared<NoteDataEP>();
		//memcpy_s(set->pNote.get(), sizeof(NoteDataEP), &info->NoteDatas[i], sizeof(NoteDataEP));
		set->pNoteEP = &info->NoteDatas[i];
		set->iPosition = -114514;
		set->wIndex = i;

		// �\�t�����Ή��p�̃C�x���g�^�C���z��쐬
		//set->EventTimeArray = new UINT[m_info->omsInfo.NumSoflans];
		//memcpy_s(set->EventTimeArray, sizeof(UINT)*m_info->omsInfo.NumSoflans, &set->note->iEventTime, sizeof(UINT)*m_info->omsInfo.NumSoflans);

		//float tick = (60 / m_SoflanMgr->m_soflans[0].fBPM / m_info->omsInfo.division) * 1000;
		//set->EventTimeArray[0] = (int)(note->iEventTime * tick);
		//UINT debug[300];
		
		// �G�f�B�b�g�̕��Ōv�Z����悤�ɂ���
		//for (UINT j = info->OMSInfo.wNumSoflans - 1; j >= 0; j--)
		//{
		//	if (info->NoteDatas[i].iEventTime >= info->SoflanDatas[j].iEventTime)	// >�ł͂Ȃ��A>=�ɂ��Ȃ��ƃ\�t�����������
		//	{
		//		const float tick((60 / m_pSoflanMgr->GetSoflans()[j].fBPM / info->OMSInfo.sDivision) * 1000);
		//		const int val(info->NoteDatas[i].iEventTime - info->SoflanDatas[j].iEventTime);
		//		set->pNote->iEventTime = (int)(val * tick) + m_pSoflanMgr->GetSoflans()[j].iEventTime;	// �\�t�����̃C�x���g����+��������̎����̎��ԓI�ȉ���
		//		set->iSoflanNo = j;

		//		//tdnDebug::OutPutLog("�m�[�g%d: %d\n j: %d\n Soflan[j]: %d\n tick: %.1f\n BPM: %.1f\n", i, info->NoteDatas[i].iEventTime, j, m_pSoflanMgr->GetSoflans()[j].iEventTime, tick, m_pSoflanMgr->GetSoflans()[j].fBPM);
		//		break;
		//	}
		//}

		//int lane = info->notes[i].cLaneNo - m_cBaseOctave;	// �m�[�c�̃��[��

		m_NoteList.push_back(set);
	}

	//int i = 1;
	//for(auto it : m_NoteList)
	//{
	//	tdnDebug::OutPutLog("�m�[�g%d: %d\n", i, it->pNote->iEventTime);
	//}


	// �ω���̍�(���̂܂ܕ����Ă�����BPM���ς�����ۂ����B)
	//int *shift = new int[m_info->omsInfo.NumSoflans];
	//
	//// �ω��O�̍�(BPM���ς��O�̍���ۑ�)
	//int *shift2 = new int[m_info->omsInfo.NumSoflans];
	//
	//// 0�͎g��Ȃ�
	//shift[0] = shift2[0] = 0;
	//
	//for (UINT i = 1; i < m_info->omsInfo.NumSoflans; i++)
	//{
	//	// �ω���̍��{��
	//	float tick2 = (60 / m_SoflanMgr->m_soflans[i].fBPM / m_info->omsInfo.division) * 1000;
	//
	//	// �ω��O�̍��{��
	//	float tick = (60 / m_SoflanMgr->m_soflans[i - 1].fBPM / m_info->omsInfo.division) * 1000;
	//
	//	// �\�t�����J�n����I���܂ł̍���ۑ�
	//	int val = m_SoflanMgr->m_soflans[i].iEventTime - m_SoflanMgr->m_soflans[i - 1].iEventTime;
	//
	//	// �ς��O�ƕς������̔{�����ꂼ����|����
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
	// ��O����(SoflanManager��Set���ɌĂ�ł�������)
	assert(m_pSoflanMgr->GetSoflans());

	// �����̃o�b�t�@�J��
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

	// �m�[�c�������X�g�ɓ���Ă���
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

	//// �S�m�[�c����A�ŏ��ɉ������m�[�c��T��
	//	// ���̃��[���ɂ͉����~���Ă��Ȃ��ꍇ�̏���
	//if (m_NoteList.empty())

	//// ���[���̈�ԍŏ��̃m�[�c(�ŏ��̃C�x���g���Ԃ̌v�������ł����̂ŁA2�ڈȍ~�̃��[���̃m�[�c�͂���Ȃ�)
	//auto LaneFirstNote = m_NoteList.begin();

	//const UINT iNoteEventTime((*LaneFirstNote)->Note.iEventTime);
	//// �ŒZ���Ԃ��Z��������
	//if (iNoteEventTime < iMinEventTime)
	//{
	//	// �m�[�c�J�n�ŒZ���ԍX�V
	//	iMinEventTime = iNoteEventTime;

	//	pOutLanes->clear();
	//	pOutLanes->push_back(i);
	//}

	//// ���������ɂ��Ή�
	//else if (iNoteEventTime == iMinEventTime) pOutLanes->push_back(i);
}

void PlayNoteInfo::UpdatePos(DWORD dwCurrentPosition)
{
	if (pNoteEP) iPosition = dwCurrentPosition - pNoteEP->iEventTime;
	else if (pNoteVRShoot)iPosition = dwCurrentPosition - pNoteVRShoot->iEventTime;
}
