#pragma once

//===============================================
//	�O���錾
//===============================================
class UI;
class NoteManager;
class SceneMain;
class JudgeActionManager;
struct GameScore;
class GaugeManager;
enum class GAUGE_OPTION;
enum class JUDGE_TYPE;
struct PlayNoteInfo;

/********************************************/
//	�Q�[�W�̑������Ǘ�����N���X
/********************************************/
class JudgeManager
{
public:
	//===============================================
	//	�R���X�g���N�^�E�f�X�g���N�^
	//===============================================
	JudgeManager();
	~JudgeManager();


	//===============================================
	//	������
	//===============================================
	void Initialize(SceneMain *pMain, NoteManager *mng, GAUGE_OPTION op, bool bShowTiming);
	void Reset();


	//===============================================
	//	�X�V�E�`��
	//===============================================
	bool Update();	// �X������true���Ԃ�
	void Render();


	//===============================================
	//	�g���K�[�ɉ����Ĕ�������
	//===============================================
	void JudgeNote(char cLaneNo);	// �v���C���[���L�[���������u�ԂɌĂяo��
	void JudgeUpKey(char cLaneNo);	// �v���C���[���L�[�𗣂����u�ԂɌĂяo��(CN�̏I���m�[�c�̔���)
	void LostNote(char cLaneNo);	// �m�[�c�����胉�C�����߂�����Ăяo��

	void JudgeNote(PlayNoteInfo *note);

	//===============================================
	//	�g���K�[�ɉ����Ĕ�������
	//===============================================
	bool isFullComboNow();	// �t���R�����o�����ǂ���
	bool isFullComboEnd();	// �t���R�����o�I�������


	//===============================================
	//	�A�N�Z�T
	//===============================================
	bool isCN(char cLaneNo){return m_bCN[cLaneNo];}

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	int m_iCombo;						// �R���{�͂����ŊǗ�����
	bool *m_bCN;						// CN�Ŏg���B��̂��Ƃ��A�w�b�_�[�ɃC���N���[�h�������Ȃ��̂ŁA�ϔz��|�C���^�ɂ���
	NoteManager *m_pRefNoteMgr;			// �m�[�c�Ǘ�����Q�Ƃ��邾��
	SceneMain *m_pRefSceneMain;			// �p�x�Ƃ肽������
	JudgeActionManager *m_pJudgeActMgr;	// ����Ǘ�����
	GaugeManager *m_GaugeManager;		// �Q�[�W�Ǘ�����
	GameScore *m_GameScore;				// GameScoreManager<Singleton>�̃f�[�^���Q�Ƃ��āA����������|�C���^
	std::unique_ptr<tdnSoundSE> m_pSE;

	//===============================================
	//	�����o�֐�
	//===============================================
	JUDGE_TYPE ComputeJudgeResult(int iPosY, BOOL bUp);	// ���肵�Č��ʂ�Ԃ�
};