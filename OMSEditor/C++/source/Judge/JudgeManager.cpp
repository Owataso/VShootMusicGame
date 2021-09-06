#include "tdnlib.h"
#include "../Music/MusicInfo.h"
#include "JudgeManager.h"
#include "../Music/NoteManager.h"
#include "JudgeAction.h"
#include "../Data/GameScore.h"
#include "Gauge.h"
#include "../Scene/SceneMain.h"

//*****************************************************************************************************************************
//	���肵�Č��ʂ�Ԃ�
//*****************************************************************************************************************************
JUDGE_TYPE JudgeManager::ComputeJudgeResult(int iPosY, BOOL bUp)
{
	// �e���蕝
	static const int l_ciSuperWidth(40);
	static const int l_ciGreadWidth(80);
	static const int l_ciGoodWidth(150);
	static const int l_ciBadWidth(180);

	if (iPosY < l_ciSuperWidth * (bUp + 1))	return JUDGE_TYPE::SUPER;
	if (iPosY < l_ciGreadWidth * (bUp + 1)) return JUDGE_TYPE::GREAT;
	if (iPosY < l_ciGoodWidth * (bUp + 1))	return JUDGE_TYPE::GOOD;
	if (iPosY < l_ciBadWidth * (bUp + 1))	return JUDGE_TYPE::BAD;
	else
	{
		return JUDGE_TYPE::MAX;
	}
}


//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
JudgeManager::JudgeManager() : 
	m_pRefNoteMgr(nullptr), 
	m_pJudgeActMgr(nullptr),
	m_GameScore(nullptr), 
	m_iCombo(0), 
	m_bCN(nullptr),
	m_pSE(nullptr)
{

}
void JudgeManager::Initialize(SceneMain *pMain, NoteManager *pNoteMgr, GAUGE_OPTION op, bool bShowTiming)
{
	m_pRefSceneMain = pMain;
	m_pRefNoteMgr = pNoteMgr;
	m_pJudgeActMgr = new JudgeActionManager(pNoteMgr->GetNumNorts(), this);
	m_pJudgeActMgr->Initialize(bShowTiming);

	m_bCN = new bool[CHAR_MAX];
	m_iCombo = 0;

	// �Q�[���X�R�A�Ǘ��N���X�̃X�R�A���A�h���X�Q��
	m_GameScore = GameScoreMgr->GetGameScore();

	// ����SE
	m_pSE = std::make_unique<tdnSoundSE>();
	m_pSE->Set(0, 12, "DATA/Sound/SE/clap.wav");

	// �Q�[�W���񏉊���
	//m_GaugeManager = new GaugeManager(op, &m_GameScore->FinesseGauge);

	// �Ȃ񂩃��Z�b�g
	Reset();
}

//------------------------------------------------------
//	���
//------------------------------------------------------
JudgeManager::~JudgeManager()
{
	SAFE_DELETE(m_pJudgeActMgr);
	//SAFE_DELETE(m_GaugeManager);
	if (m_bCN)
	{
		delete[] m_bCN;
		m_bCN = nullptr;
	}
}

//*****************************************************************************************************************************
//		�X�V�E�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�X�V
//------------------------------------------------------
bool JudgeManager::Update()
{
	m_pJudgeActMgr->Update();

	// �X�t���O��Ԃ�
	//return m_GaugeManager->isFailed();
	return false;
}

//------------------------------------------------------
//	�`��
//------------------------------------------------------
void JudgeManager::Render()
{
	m_pJudgeActMgr->Render();
}


//*****************************************************************************************************************************
//		�m�[�g����
//*****************************************************************************************************************************
void JudgeManager::JudgeNote(char cLaneNo)
{
	// �m�[�c���X�g���[�v
	for (auto &it : *m_pRefNoteMgr->GetList())
	{
		int l_iPosY;
		m_pRefNoteMgr->GetNoteAbsolutePos((*it), &l_iPosY);	// �m�[�c�̐�Έʒu

		JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

		// ��Βl����
		l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

		JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, FALSE));

		if(JudgeResult == JUDGE_TYPE::SUPER)
		{
			l_eJudgeTiming = JUDGE_TIMING::JUST;
			m_pSE->Play(0);
		}

		// ��������̔�����N�����Ă�����
		if (JudgeResult != JUDGE_TYPE::MAX)
		{
			// ������X�V
			m_GameScore->JudgeCount[(int)JudgeResult]++;

			// �R���{���X�V
			m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
			// �ő�R���{���X�V
			m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

			// �W���b�W�A�N�V�����N�Ƀf�[�^���M
			m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);

			// �Q�[�W����
			//m_GaugeManager->Compute(JudgeResult);

			// �X�R�A�v�Z
			m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

			// �m�[�c����
			it->bErase = true;

			// �m�[�c��CN�J�n��������CN�t���OON
			if (it->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_START) m_bCN[cLaneNo] = true;
		}
		break;
	}
}

void JudgeManager::JudgeNote(PlayNoteInfo * note)
{
	int l_iPosY;
	m_pRefNoteMgr->GetNoteAbsolutePos(*note, &l_iPosY);	// �m�[�c�̐�Έʒu

	JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

	// ��Βl����
	l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

	JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, FALSE));

	if (JudgeResult == JUDGE_TYPE::SUPER)
	{
		l_eJudgeTiming = JUDGE_TIMING::JUST;
		m_pSE->Play(0);
	}

	// ��������̔�����N�����Ă�����
	if (JudgeResult != JUDGE_TYPE::MAX)
	{
		// ������X�V
		m_GameScore->JudgeCount[(int)JudgeResult]++;

		// �R���{���X�V
		m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
		// �ő�R���{���X�V
		m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

		// �W���b�W�A�N�V�����N�Ƀf�[�^���M
		//m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);
		m_pJudgeActMgr->Set(note->pNoteVRShoot->GetCenterScreenPosition(m_pRefSceneMain->m_iMaxDegree) - Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE), m_iCombo, JudgeResult, l_eJudgeTiming);

		// �Q�[�W����
		//m_GaugeManager->Compute(JudgeResult);

		// �X�R�A�v�Z
		m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

		// �m�[�c����
		note->bErase = true;

		// �m�[�c��CN�J�n��������CN�t���OON
		//if (note->pNoteVRShoot->byNoteType & (BYTE)NOTE_TYPE::CN_START) m_bCN[cLaneNo] = true;
	}
}

void JudgeManager::JudgeUpKey(char cLaneNo)
{
	// CN�����ĂȂ�������o�Ă������I�I
	if (!m_bCN[cLaneNo]) return;

	int l_iPosY;
	auto it = *m_pRefNoteMgr->GetList()->begin();
	m_pRefNoteMgr->GetNoteAbsolutePos((*it), &l_iPosY);	// �m�[�c�̐�Έʒu

	JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

	// ��Βl����
	l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

	JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, TRUE));

	if(JudgeResult == JUDGE_TYPE::SUPER)
	{
		l_eJudgeTiming = JUDGE_TIMING::JUST;
		m_pSE->Play(0);
	}

	if (JudgeResult == JUDGE_TYPE::MAX) JudgeResult = JUDGE_TYPE::BAD;

	// ������X�V
	m_GameScore->JudgeCount[(int)JudgeResult]++;

	// �R���{���X�V
	m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
	// �ő�R���{���X�V
	m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

	// �W���b�W�A�N�V�����N�Ƀf�[�^���M
	m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);

	// �Q�[�W����
	//m_GaugeManager->Compute(JudgeResult);

	// �X�R�A�v�Z
	m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

	// �m�[�c����
	it->bErase = true;

	// �t���O�I��
	m_bCN[cLaneNo] = false;
}

void JudgeManager::LostNote(char cLaneNo)
{
	// BAD����J�E���g
	m_GameScore->JudgeCount[(int)JUDGE_TYPE::BAD]++;

	// �R���{���X�V
	m_iCombo = 0;

	// �W���b�W�A�N�V�����N�Ƀf�[�^���M
	m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JUDGE_TYPE::BAD, JUDGE_TIMING::SLOW);

	// �Q�[�W��
	//m_GaugeManager->Compute(JUDGE_TYPE::BAD);

	// CN�����z����z�肵�āACN�t���O���I�t�ɂ���
	m_bCN[cLaneNo] = false;
}

void JudgeManager::Reset()
{
	// �X�R�A�ƃQ�[�W���Z�b�g
	GameScoreMgr->ResetExceptGauge();	// �i�ʗp�ɃQ�[�W���c���Ă̏��������s��

	//// �Q�[�W�I�v�V�����ɉ����ď����Q�[�W�̒l��ݒ�
	//int StartGauge;
	//switch (m_GaugeManager->GetGaugeOption())
	//{
	//	// 22%�X�^�[�g
	//case GAUGE_OPTION::NORMAL:
	//case GAUGE_OPTION::EASY:
	//	StartGauge = 22;
	//	break;

	//	// 100%�X�^�[�g
	//case GAUGE_OPTION::HARD:
	//case GAUGE_OPTION::EX_HARD:
	//case GAUGE_OPTION::HAZARD:
	//	StartGauge = 100;
	//	break;

	//	// �O�̃Q�[�W����̃X�^�[�g
	//case GAUGE_OPTION::GRADE:
	//	StartGauge = GameScoreMgr->GetGameScore()->FinesseGauge;
	//	break;
	//}
	//m_GaugeManager->GaugeSet(StartGauge);
	m_iCombo = 0;

	// ����A�N�V�����N���A
	m_pJudgeActMgr->Clear();

	// �m�[�c�񕜗ʐݒ�
	//m_GaugeManager->SetNumNorts(m_pRefNoteMgr->GetNumNorts());

	// CN�t���O�I�t
	memset(m_bCN, false, sizeof(bool)*NUM_KEYBOARD);
}

// �t���R�����o�����ǂ���
bool JudgeManager::isFullComboNow(){ return m_pJudgeActMgr->isFullComboNow(); }
bool JudgeManager::isFullComboEnd(){ return m_pJudgeActMgr->isFullComboEnd(); }