#include "TDNLib.h"
#include "../Music/MusicInfo.h"
#include "JudgeAction.h"
#include "JudgeManager.h"
#include "../Music/PlayCursor.h"
#include "../Data/CustomizeData.h"

const int JudgeActionManager::m_ciJudgeAppTime(45);

//*****************************************************************************************************************************
//
//		����A�N�V���������Ǘ�
//
//*****************************************************************************************************************************
//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
JudgeActionManager::JudgeActionManager(UINT NumNote, JudgeManager *pJudgeMgr) :
	m_pJudgeMgr(pJudgeMgr), 
	m_iNumNote(NumNote), 
	m_pJudgeAction(nullptr), 
	m_pJudgeStr(nullptr), 
	m_pFullCombo(nullptr), 
	m_bFullCombo(false), 
	m_pCNEffect(nullptr),
	m_iSuperAnimFrame(0),
	m_iSuperAnimPanel(0)
{
}
void JudgeActionManager::Initialize(bool bShowTiming)
{
	// �����G�t�F�N�g
	m_tagExplosion.pImage = new tdn2DObj(
		//(char*)CustomizeDataMgr->GetSetedSkin(CUSTOMIZE_TYPE::EXPLOSION).c_str()
		"DATA/Customize/Explosion/plasma.png"
	);	// �摜
	m_tagExplosion.iNumPanel = m_tagExplosion.pImage->GetWidth() / 128;

	// CN�G�t�F�N�g
	m_pCNEffect = std::make_shared<tdn2DObj>("DATA/Effect/CN.png");												// �A�j���̃R�}�̐�

	// ���蕶���摜
	m_pJudgeStr = std::make_shared<tdn2DObj>("DATA/UI/Playing/judge.png");
	m_pJudgeStr->SetARGB((BYTE)245, (BYTE)255, (BYTE)255, (BYTE)255);

	// ����A�N�V��������
	m_pJudgeAction = std::make_unique<JudgeAction>(bShowTiming);

	// �t���R���G�t�F�N�g���ɐ���
	m_pFullCombo = new FullComboEffect;
}

//------------------------------------------------------
//	���
//------------------------------------------------------
JudgeActionManager::~JudgeActionManager()
{
	SAFE_DELETE(m_pFullCombo);

	for (auto it = m_JudgeEffectList.begin(); it != m_JudgeEffectList.end();)
	{
		delete (*it);
		it = m_JudgeEffectList.erase(it);
	}
}


//*****************************************************************************************************************************
//	�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//	�X�V
//------------------------------------------------------
void JudgeActionManager::Update()
{
	// SUPER�A�j���X�V
	if (++m_iSuperAnimFrame > 3)
	{
		m_iSuperAnimFrame = 0;
		if (++m_iSuperAnimPanel > 3) m_iSuperAnimPanel = 0;
	}

	// ���蕶�������X�V
	m_pJudgeAction->Update();

	// ����G�t�F�N�g�X�V
	//for (JudgeEffect *e = JudgeEffect::roots; e != nullptr;)
	//{
	//	JudgeEffect *next = e->next;
	//	// �X�V
	//	if(!e->Update()) delete e;
	//	e = next;
	//}
	for (auto it = m_JudgeEffectList.begin(); it != m_JudgeEffectList.end();)
	{
		if ((*it)->Update())
		{
			delete (*it);
			it = m_JudgeEffectList.erase(it);
		}
		else it++;
	}

	// �t���R�����o�X�V
	//if (m_bFullCombo)m_pFullCombo->Update();

	// �t���R�����o�f�o�b�O
	//if (KeyBoardTRG(KB_F) && KeyBoard(KB_SPACE))
	//{
	//	OnFullCombo();
	//}
}

bool JudgeActionManager::isFullComboEnd()
{
	assert(m_pFullCombo);
	return m_pFullCombo->isEnd();
}


//*****************************************************************************************************************************
//	�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�`��
//------------------------------------------------------
void JudgeActionManager::Render()
{
	// ���蕶�������`��
	m_pJudgeAction->Render(m_pJudgeStr.get(), m_iSuperAnimPanel);

	// ����G�t�F�N�g�`��
	//for (JudgeEffect *e = JudgeEffect::roots; e != nullptr; e = e->next)
	//{
	//	// �`��
	//	e->Render(m_explosion.pImage);
	//}
	for (auto &it : m_JudgeEffectList) it->Render();

	// CN�G�t�F�N�g�`��
	static int iEffectFrame(0);
	if(++iEffectFrame > 16) iEffectFrame=0;
	FOR(NUM_KEYBOARD)
	{
		if(m_pJudgeMgr->isCN(i)) m_pCNEffect->Render(-43 + KEYLANE_LEFT + NOTE_POS_COMPTE(i), JUDGE_LINE - 60, 128, 128, (iEffectFrame / 2) * 128, 0, 128, 128);
	}

	// �t���R�����o�`��
	if (m_bFullCombo)m_pFullCombo->Render();
}

//*****************************************************************************************************************************
//	�Z�b�g
//*****************************************************************************************************************************
//------------------------------------------------------
//	���X�g�ǉ�
//------------------------------------------------------
void JudgeActionManager::Set(char lane, int combo, JUDGE_TYPE type, JUDGE_TIMING eTiming)
{
	m_pJudgeAction->Set(lane, combo, m_ciJudgeAppTime, type, eTiming);	// ����Ȃ��������������B���[���ԍ������I�N�^�[�u�ڂɂ��邩���܂薳���̐؂�̂Ċ���Z�Ŕz��Q��

	// GREAT�ȏ�̔���Ȃ�A�G�t�F�N�g���Ăяo��
	if(type == JUDGE_TYPE::SUPER || type== JUDGE_TYPE::GREAT) m_JudgeEffectList.push_back(new JudgeEffect(lane, &m_tagExplosion));

	// �t���R���{���Ă��牉�oON
	if (combo == m_iNumNote)
	{
		OnFullCombo();
	}
}

void JudgeActionManager::Set(const Vector2 &pos, int combo, JUDGE_TYPE type, JUDGE_TIMING eTiming)
{
	if (type == JUDGE_TYPE::SUPER || type == JUDGE_TYPE::GREAT) m_JudgeEffectList.push_back(new JudgeEffect(pos, &m_tagExplosion));

	// �t���R���{���Ă��牉�oON
	if (combo == m_iNumNote)
	{
		OnFullCombo();
	}
}

//------------------------------------------------------
//	����N���A
//------------------------------------------------------
void JudgeActionManager::Clear()
{
	// ����G�t�F�N�g�S�폜
	//for (JudgeEffect *e = JudgeEffect::roots; e != nullptr;)
	//{
	//	JudgeEffect *prev = e->next;
	//	delete e;
	//	e = prev;
	//}
	for (auto it = m_JudgeEffectList.begin(); it != m_JudgeEffectList.end();)
	{
		delete (*it);
		it = m_JudgeEffectList.erase(it);
	}

	// �\�����ԂƂ��S��0�ɂ���
	m_pJudgeAction->Off();
}

//------------------------------------------------------
//	�t���R��ON
//------------------------------------------------------
void JudgeActionManager::OnFullCombo()
{
	// �t���R���t���OON
	m_bFullCombo = true;

	// ������Ԃɖ߂�(������Đ��ł���悤�ɂ����˂�)
	m_pFullCombo->Reset();
}

//*****************************************************************************************************************************
//
//		����A�N�V����
//
//*****************************************************************************************************************************
//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
JudgeAction::JudgeAction(bool bShowTiming) :
	m_iOctave(0), 
	m_iCombo(0), 
	m_iAppTime(0), 
	m_eJudgeType(JUDGE_TYPE::MAX), 
	m_eJudgeTiming(JUDGE_TIMING::JUST),
	m_iPosX(KEYLANE_LEFT + 2 * (40 * 7)),
	m_bShowTiming(bShowTiming)
{

}

//*****************************************************************************************************************************
//	�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�`��
//------------------------------------------------------
void JudgeAction::Render(tdn2DObj *pJudgeStr, int iPanel)
{
	// �\�����ĂȂ��I
	if (m_iAppTime <= 0) return;

	// �S�����񂪏������S���S���ȃR�[�h
	const int l_ciSrcX((int)m_eJudgeType * 256), l_ciSrcY(iPanel * 32);

	pJudgeStr->Render(m_iPosX, JUDGE_LINE - 65, 256, 32, l_ciSrcX, l_ciSrcY, 256, 32);
	tdnText::Draw(m_iPosX + 110, JUDGE_LINE - 100, 0xccffffff, "%d",m_iCombo);

	if (m_bShowTiming)
	{
		if (m_eJudgeTiming == JUDGE_TIMING::FAST)tdnText::Draw(m_iPosX + 90, JUDGE_LINE - 130, 0xccff0000, "FAST");
		else if (m_eJudgeTiming == JUDGE_TIMING::SLOW)tdnText::Draw(m_iPosX + 90, JUDGE_LINE - 130, 0xcc0000ff, "SLOW");
	}
}

//------------------------------------------------------
//	����Z�b�g
//------------------------------------------------------
void JudgeAction::Set(char cLaneNo, int iCombo, int iTime, JUDGE_TYPE eType, JUDGE_TIMING eTiming)
{
	m_iCombo = iCombo;
	m_iAppTime = iTime;
	m_eJudgeType = eType;
	m_eJudgeTiming = eTiming;

	m_iPosX = KEYLANE_LEFT + (cLaneNo / (128 / 4)) * (40 * 7);
}


//*****************************************************************************************************************************
//
//		����G�t�F�N�g
//
//*****************************************************************************************************************************
JudgeEffect::JudgeEffect(char cLaneNo, JudgeActionManager::ExplosionInfo *pExplosion) :
	m_pRefExplosion(pExplosion),
	m_iCurrentFrame(0),
	m_iCurrentPanel(0)
{
	m_vPos.x = (float)(-43 + KEYLANE_LEFT + NOTE_POS_COMPTE(cLaneNo));
	m_vPos.y = (float)(JUDGE_LINE - 60);
}

//*****************************************************************************************************************************
//	�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//	�X�V
//------------------------------------------------------
bool JudgeEffect::Update()
{
	// �R�}���菈��
	if (++m_iCurrentFrame > 1)	// 2/60�b�ɃA�j���[�V����
	{
		m_iCurrentFrame = 0;
		m_iCurrentPanel++;
	}

	const bool l_cbEnd(m_iCurrentPanel >= m_pRefExplosion->iNumPanel);
	return l_cbEnd;
}

//*****************************************************************************************************************************
//	�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�`��
//------------------------------------------------------
void JudgeEffect::Render()
{
	m_pRefExplosion->pImage->Render((int)m_vPos.x, (int)m_vPos.y, 128, 128, m_iCurrentPanel * 128, 0, 128, 128);
}







//*****************************************************************************************************************************
//
//		�t���R�����o
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//	�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//	�X�V
//------------------------------------------------------
void FullComboEffect::Update()
{
	// �t���R���N���X�X�V
	m_pFullComboCross->Update();

	// �t���q�����X�V
	m_pFullComboExplosion->Update();
}

//*****************************************************************************************************************************
//	�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�`��
//------------------------------------------------------
void FullComboEffect::Render()
{
	// �t���R�������`��
	m_pFullComboExplosion->Render();

	// �t���R���N���X�`��
	m_pFullComboCross->Render();
}


//*****************************************************************************************************************************
//
//		�t���R���N���X
//
//*****************************************************************************************************************************

void FullComboEffect::FullComboCross::Update()
{
	m_pFull->Update();
	m_pCombo->Update();

	static int VanishWaitFrame = 0;	// �����Ă����莞�ԑ҂t���[��
	switch (m_iStep)
	{
	case 0:
		// �����Z
		m_byAlpha = min(m_byAlpha + 12, 255);
		m_pFull->pImage->SetARGB((m_byAlpha << 24) | 0x00ffffff);
		m_pCombo->pImage->SetARGB((m_byAlpha << 24) | 0x00ffffff);

		// ���������|��������
		if (m_pCombo->pos.x - m_pFull->pos.x < 150)
		{
			// ���x����
			m_pFull->Deceleration();
			m_pCombo->Deceleration();

			m_iStep++;
		}
		break;

	case 1:	// �������
		// ���݂��������ς񂾂�
		if (m_pCombo->pos.x - m_pFull->pos.x < 0)
		{
			// ���x��߂�
			m_pFull->ReAcceleration();
			m_pCombo->ReAcceleration();

			m_iStep++;
		}
		break;

	case 2:
		// �����Z
		m_byAlpha = max(m_byAlpha - 12, 0);
		m_pFull->pImage->SetARGB((m_byAlpha << 24) | 0x00ffffff);
		m_pCombo->pImage->SetARGB((m_byAlpha << 24) | 0x00ffffff);

		// ���݂������ԗ��ꂽ��
		if (m_pCombo->pos.x - m_pFull->pos.x < -1280)
		{
			VanishWaitFrame = 0;
			m_iStep++;
		}

		break;

	case 3:
		// �����Ă�����Ƒ҂t���[��
		if (++VanishWaitFrame > 90) m_iStep++;	// �Ƃ肠������ڏI��
		break;
	case 4:
		break;	// �I���҂�
	}
}

void FullComboEffect::FullComboCross::Render()
{
	m_pFull->Render();
	m_pCombo->Render();
}


//*****************************************************************************************************************************
//
//		�t���R������
//
//*****************************************************************************************************************************
FullComboEffect::FullComboExplosion::FullComboExplosion() :m_iAnimFrame(0), m_iAnimPanel(0)
{
	// �����摜������
	m_pImage = std::make_unique<tdn2DObj>("DATA/UI/FullCombo/Default/explosion.png");
}

void FullComboEffect::FullComboExplosion::Update()
{
	// �����̏o�Ă������I�I
	if (isEnd()) return;

	// �A�j���[�V��������
	if (++m_iAnimFrame > 1)
	{
		m_iAnimFrame = 0;
		m_iAnimPanel++;
	}
}

void FullComboEffect::FullComboExplosion::Render()
{
	// �����̏o�Ă������I�I
	if (isEnd()) return;

	m_pImage->Render(120, 134, 880, 495, (m_iAnimPanel % 6) * 512, (m_iAnimPanel / 6) * 256, 512, 256);
}