#pragma once

//===============================================
//	�O���錾
//===============================================
enum class JUDGE_TYPE;
enum class JUDGE_TIMING;
class JudgeAction;
class FullComboEffect;
class JudgeEffect;
class JudgeManager;


/********************************************/
//	����A�N�V����(�G�t�F�N�g�Ƃ�)���Ǘ�����
/********************************************/
class JudgeActionManager
{
public:
	//===============================================
	//	�O���錾
	//===============================================
	JudgeActionManager(UINT NumNotes, JudgeManager *pJudgeMgr);
	~JudgeActionManager();

	//===============================================
	//	������
	//===============================================
	void Initialize(bool bShowTiming);
	void Clear();

	//===============================================
	//	�X�V�E�`��
	//===============================================
	void Update();
	void Render();


	//===============================================
	//	�A�N�Z�T
	//===============================================
	void Set(char cLaneNo, int iCombo, JUDGE_TYPE type, JUDGE_TIMING eTiming);
	void Set(const Vector2 &pos, int iCombo, JUDGE_TYPE type, JUDGE_TIMING eTiming);
	bool isFullComboNow() { return m_bFullCombo; }	// �t���R�����o�����ǂ���
	bool isFullComboEnd();							// �t���R�����o�I�������

	//===============================================
	//	�{���p�̍\����
	//===============================================
	struct ExplosionInfo
	{
		tdn2DObj *pImage;		// �����G�t�F�N�g�摜
		int iNumPanel;			// �R�}�̐�
		ExplosionInfo() :pImage(nullptr), iNumPanel(0) {}
		~ExplosionInfo() { SAFE_DELETE(pImage); }
	}m_tagExplosion;
private:

	//===============================================
	//	�����o�ϐ�
	//===============================================
	JudgeManager *m_pJudgeMgr;					// �����̎��̂������Ă���e�ł��锻��Ǘ�����̃|�C���^(CN�t���O�̎擾�Ŏg��)
	const UINT m_iNumNote;						// ���m�[�c��(�t���R������p)
	std::shared_ptr<tdn2DObj> m_pJudgeStr;						// ����̕����摜
	std::shared_ptr<tdn2DObj> m_pCNEffect;						// CN���̃G�t�F�N�g(����͋��ʂȂ̂ŁA�����ɒu��)
	static const int m_ciJudgeAppTime;			// ���肪�o�Ă鎞��
	std::list<JudgeEffect*> m_JudgeEffectList;	// �����̃G�t�F�N�g�̃��X�g
	std::unique_ptr<JudgeAction> m_pJudgeAction;// ����A�N�V����(�R���{���Ɣ���̕���)�����ł͉ϔz��|�C���^�Ŏg��
	FullComboEffect *m_pFullCombo;				// �t���R�����o�p
	bool m_bFullCombo;							// �t���R�����Ă���f�[�^�ǂނƍ������������Ń_�T���̂Ő�ɗe�ʊm�ۂ��Ă��̃t���O��ON�ɂ����甭��
	int m_iSuperAnimFrame, m_iSuperAnimPanel;	// SUPER�̃A�j���[�V����
	bool m_bShowTiming;							// ����\��

	//===============================================
	//	�����o�֐�
	//===============================================
	void OnFullCombo();							// �t���R�����oON
};


/********************************************/
//	���茋��(SUPER)�Ƃ�
/********************************************/
class JudgeAction
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	JudgeAction(bool bShowTiming);

	//===============================================
	//	�X�V�E�`��
	//===============================================
	void Update() { if(m_iAppTime > 0)m_iAppTime--; }
	void Render(tdn2DObj *pJudgeStr, int iPanel);

	//===============================================
	//	�A�N�Z�T�[
	//===============================================
	void Set(char cLaneNo, int iCombo, int iTime, JUDGE_TYPE eType, JUDGE_TIMING eTiming);
	void Off() { m_iCombo = 0; m_iAppTime = 0; }

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	int m_iOctave;			// 1�I�N�^�[�u�������̔���`��̈�
	int m_iCombo;			// �R���{��
	int m_iAppTime;			// �o������
	int m_iPosX;			// �����̍��W(Y�͋��ʂȂ̂łȂ�)
	BYTE m_byAlpha;			// ���蕶���̃A���t�@
	bool m_bShowTiming;	// ����\��
	JUDGE_TYPE m_eJudgeType;	// ���̔��肩
	JUDGE_TIMING m_eJudgeTiming;	// ����^�C�~���O

	//===============================================
	//	�f�t�H���g�R���X�g���N�^����
	//===============================================
	JudgeAction(){}
};


/********************************************/
//	�{��
/********************************************/
class JudgeEffect
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	JudgeEffect(char cLaneNo, JudgeActionManager::ExplosionInfo *pExplosion);
	JudgeEffect(const Vector2 &pos, JudgeActionManager::ExplosionInfo *pExplosion) :
		m_vPos(pos),
		m_pRefExplosion(pExplosion),
		m_iCurrentFrame(0),
		m_iCurrentPanel(0) {}

	//===============================================
	//	�X�V�E�`��
	//===============================================
	bool Update();
	void Render();

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	Vector2 m_vPos;
	//char m_cLaneNo;			// ���[���ԍ�
	int m_iCurrentFrame;	// ���݃t���[��
	int m_iCurrentPanel;	// ���݂̃R�}
	JudgeActionManager::ExplosionInfo *m_pRefExplosion;	// �摜�Ƃ��̃R�}���������Ă���
};


/********************************************/
//	�t���R�����o
/********************************************/
class FullComboEffect
{
public:
	//===============================================
	//	�R���X�g���N�^
	//===============================================
	FullComboEffect() :
		m_pFullComboCross(new FullComboCross),
		m_pFullComboExplosion(new FullComboExplosion){}

	//===============================================
	//	�X�V�E�`��
	//===============================================
	void Update();
	void Render();

	//===============================================
	//	�A�N�Z�T
	//===============================================
	void Reset(){ m_pFullComboCross->Reset(); m_pFullComboExplosion->Reset(); }
	bool isEnd(){ return m_pFullComboCross->isEnd(); }

private:
	//===============================================
	//	�t���R���{�����̌���
	//===============================================
	class FullComboCross
	{
	public:
		FullComboCross():m_iStep(0), m_byAlpha(0),
			m_pFull(new Info("DATA/UI/FullCombo/full.png", Vector2(50, 340), 12)),
			m_pCombo(new Info("DATA/UI/FullCombo/combo.png", Vector2(950, 400), -12)){}

		void Update();
		void Render();
		void Reset(){ m_pFull->Reset(); m_pCombo->Reset(); m_byAlpha = 0; m_iStep = 0; }
		bool isEnd(){ return (m_iStep == 4); }
	private:
		class Info
		{
		public:
			Vector2 pos;		// ���W
			const Vector2 OrgPos;
			std::unique_ptr<tdn2DObj> pImage;	// �摜
			Info(char *filename, const Vector2 &pos, float force) :pImage(new tdn2DObj(filename)), OrgPos(pos), OrgForce(force){ Reset(); }
			void Update()
			{
				moveX = moveX*.5f + force * .5f;	// �ړ��l���
				pos.x += moveX;					// ���W�X�V
			}
			void Render(){ pImage->Render((int)pos.x, (int)pos.y); }
			void Deceleration(){ force = OrgForce * .065f; }	// ����
			void ReAcceleration(){ force = OrgForce; }		// �ĉ���
			void Reset(){ pos = OrgPos; force = OrgForce; moveX = 0; pImage->SetARGB(0x00ffffff); }
		private:
			float moveX;		// �ړ��l
			float force;		// �ړ��ɂ������(��ԂŎg��)
			const float OrgForce;	// �ŏ��̈ړ���(�������Ė߂鎞�Ɏg��)
		};
		std::unique_ptr<Info> m_pFull, m_pCombo;

		int m_iStep;		// �ړ������������̑��x�ňړ��̃X�e�b�v
		BYTE m_byAlpha;	// �����̃A���t�@(Info���ł��Ȃ��̂́A�X�e�b�v������������������)
	};

	//===============================================
	//	�t���R������
	//===============================================
	class FullComboExplosion
	{
	public:
		FullComboExplosion();
		void Update();
		void Render();
		void Reset(){ m_iAnimFrame = m_iAnimPanel = 0; }
		bool isEnd(){ return (m_iAnimPanel == 59); }
	private:
		std::unique_ptr<tdn2DObj> m_pImage;
		int m_iAnimFrame, m_iAnimPanel;
	};


	//===============================================
	//	�����o�ϐ�
	//===============================================
	std::unique_ptr<FullComboCross> m_pFullComboCross;
	std::unique_ptr<FullComboExplosion> m_pFullComboExplosion;
};