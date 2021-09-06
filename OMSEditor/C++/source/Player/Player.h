#pragma once

//===============================================
//	�O���錾
//===============================================
class UI;
class NoteManager;
class JudgeManager;


/********************************************/
//	���t����v���C���[
/********************************************/
class Player
{
public:
	//===============================================
	//	�萔
	//===============================================
	enum class MODE { YOU, AUTO };

	//===============================================
	//	�������E���
	//===============================================
	Player();
	~Player();
	void Initialize(UI *pUI, NoteManager *pNoteMgr, JudgeManager *pJudgeMgr, MODE eMode);

	//===============================================
	//	�X�V
	//===============================================
	void Update();

private:
	//===============================================
	//	����Ϗ��N���X
	//===============================================
	class Input
	{
	public:
		class Base
		{
		protected:
			Player *me;
		public:
			Base(Player *me) :me(me) {}
			virtual void Update() = 0;
		};
		/*���ۂɂ���Ă��炤�l*/
		class You : public Base
		{
		public:
			You(Player *me) :Base(me) {}
			void Update()override;
		};
		/*DJ AUTO����*/
		class DJ_AUTO : public Base
		{
		public:
			DJ_AUTO(Player *me) :Base(me) {}
			void Update()override;
		};
	};


	//===============================================
	//	�����o�ϐ�
	//===============================================
	int m_iNumKey;				// �L�[�̐�
	BYTE *m_byKeyList;			// �L�[�{�[�h�̃L�[�R�[�h���X�g
	bool *m_bON;				// �����Ă�炵�ĂȂ����̉ϔz��
	Input::Base *m_pInput;		// �Ϗ�
	UI *m_pUI;					// �Q�Ƃ��邾��UI
	NoteManager *m_pNoteMgr;	// �Q�Ƃ��邾��Note
	JudgeManager *m_pJudgeMgr;	// �Q�Ƃ��邾��Judge


	//===============================================
	//	�����o�֐�
	//===============================================
	void ControlSudden();				// �T�h�D���I�v�V��������
	void ControlHiSpeed();				// �n�C�X�s�I�v�V��������
	void LoadKeyCode(int iOctaveWidth);	// �L�[�{�[�h���͂̃L�[�R�[�h�ǂݍ���
};