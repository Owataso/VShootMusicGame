#pragma once

#include "TDNLIB.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "../Input/InputMIDI.h"

/********************************************/
//	����E�B���h�E
/********************************************/
class QuestionWindow
{
public:

	//------------------------------------------------------
	//	����ɓ�����^�C�v
	//------------------------------------------------------
	enum ANSWER_TYPE
	{
		YES_NO	// �͂����������œ�����(�����ƍ����ł�������)
	};

	//------------------------------------------------------
	//	�R���X�g���N�^
	//------------------------------------------------------
	QuestionWindow(ANSWER_TYPE eAnswerType) :
		m_iStep(NO_ACTION),
		m_iAnswer(NO_ANSWER),
		m_pWindowImage(new tdn2DAnim("DATA/UI/Other/info_window.png")),
		m_lpcHeading(""),
		m_lpcQuestion("")
	{
		// �����̃^�C�v�ɂ���āA�A�N�V�����̃N���X��ς���
		switch (eAnswerType)
		{
		case QuestionWindow::YES_NO:
			m_pAnswerAction = std::make_unique<AnswerAction::YesNo>(this);
			break;
		default:
			assert(0);
			break;
		}
	}

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	bool Update()
	{
		switch (m_iStep)
		{
		case 0:	/* �E�B���h�E���J���܂ł̊� */

			// �摜�A�j���X�V
			m_pWindowImage->Update();

			// �摜�A�j���[�V�������I�������
			if (m_pWindowImage->GetAction()->IsEnd())
			{
				// ���̃X�e�b�v��
				m_iStep++;
			}
			break;

		case 1:	/* ���ۂɃv���C���[���I������� */

			// �I�����I�������
			if (m_pAnswerAction->Update())
			{
				// �摜�̏k���A�j�����Z�b�g����
				m_pWindowImage->OrderStretch(15, 1, 1, 1, 0);
				m_pWindowImage->Action();

				// �t�F�[�h������
				Fade::Set(Fade::MODE::FADE_IN, 8, 0, Fade::m_byAlpha, 0x00000000);

				// ���̃X�e�b�v��
				m_iStep++;
			}
			break;

		case 2:	/* �E�B���h�E������܂ł̊� */

			// �摜�A�j���X�V
			m_pWindowImage->Update();

			// �摜�A�j���[�V�������I�������
			if (m_pWindowImage->GetAction()->IsEnd())
			{
				// ������Ԃɂ���
				m_iStep = NO_ACTION;

				// �I���I
				return true;
			}
			break;
		}

		// �����ɗ�����܂��I����Ă��Ȃ�
		return false;
	}

	//------------------------------------------------------
	//	�`��
	//------------------------------------------------------
	void Render()
	{
		// �������ĂȂ�������o�Ă������I�I
		if (m_iStep == NO_ACTION)return;

		// �摜�A�j���`��
		m_pWindowImage->Render(310, 232);

		//static int x(0), y(0);
		//
		//if (KeyBoardTRG(KB_RIGHT))x += 10;
		//if (KeyBoardTRG(KB_LEFT))x -= 10;
		//if (KeyBoardTRG(KB_DOWN))y += 10;
		//if (KeyBoardTRG(KB_UP))y -= 10;

		// �I�𒆂̂ݕ`��
		if (m_iStep == 1)
		{
			// ���o���Ǝ���̕�����̕`��
			tdnFont::RenderStringCentering(m_lpcHeading, 20, 482, 236);
			tdnFont::RenderStringCentering(m_lpcQuestion, 24, 660, 300);

			// �Ϗ��`��
			m_pAnswerAction->Render();
		}
	}

	//------------------------------------------------------
	//	��������
	//------------------------------------------------------
	void Action(LPCSTR lpcHeading, LPCSTR lpcQuestion)
	{
		// �e������Z�b�g
		m_lpcHeading = lpcHeading;
		m_lpcQuestion = lpcQuestion;

		// �A�N�V��������ȃt���O(-1)����
		m_iStep = 0;

		// �𓚂�������
		m_iAnswer = NO_ANSWER;

		// �摜�̈����L�΂��A�j�����Z�b�g����
		m_pWindowImage->OrderStretch(15, 1, .0f);
		m_pWindowImage->Action();

		// �t�F�[�h���኱���Â�����
		Fade::Set(Fade::MODE::FADE_OUT, 8, 200, Fade::m_byAlpha, 0x00000000);

		// �E�B���h�E�J��SE
		g_pSE->Play("�E�B���h�E1");
	}

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	int GetAnswer(){ return m_iAnswer; }
	bool isAction(){ return (m_iStep != NO_ACTION); }

private:

	//------------------------------------------------------
	//	����̃^�C�v���ɃN���X�Ŏ���
	//------------------------------------------------------
	class AnswerAction
	{
	public:

		//------------------------------------------------------
		//	����̃^�C�v���
		//------------------------------------------------------
		class Base
		{
		public:
			Base(QuestionWindow *pParent) :m_pParent(pParent){}
			virtual ~Base(){}
			virtual bool Update() = 0;
			virtual void Render() = 0;

		protected:
			QuestionWindow  *m_pParent;	// ���̃N���X�����e�̎���
		};


		//------------------------------------------------------
		//	YES��NO�œ�����^�C�v
		//------------------------------------------------------
		class YesNo :public Base
		{
		public:
			YesNo(QuestionWindow *pParent) :Base(pParent){}
			bool Update()
			{
				// �����L�[(YES�`�F�b�N)
				if (InputMIDIMgr->GetEPInputTRG(EP_INPUT::WHITE))
				{
					// 1��ڂ̑I���Ȃ�
					if (m_pParent->m_iAnswer != TRUE)
					{
						// SE�̍Đ�
						g_pSE->Play("�J�[�\��2");

						// �𓚂�YES�ɂ���
						m_pParent->m_iAnswer = TRUE;
					}

					// 2�񓯂��̂�I��ł�̂ŏI���I
					else
					{
						// SE�̍Đ�
						g_pSE->Play("����2");

						return true;
					}
				}

				// �����L�[(NO�`�F�b�N)
				else if (InputMIDIMgr->GetEPInputTRG(EP_INPUT::BLACK))
				{
					// 1��ڂ̑I���Ȃ�
					if (m_pParent->m_iAnswer != FALSE)
					{
						// SE�̍Đ�
						g_pSE->Play("�J�[�\��2");

						// �𓚂�NO�ɂ���
						m_pParent->m_iAnswer = FALSE;
					}

					// 2�񓯂��̂�I��ł�̂ŏI���I
					else
					{
						// SE�̍Đ�
						g_pSE->Play("����2");

						return true;
					}
				}

				// �܂�������
				return false;
			}
			void Render()
			{
				tdnFont::RenderString("���� :", 30, 680, 380);
				tdnFont::RenderString("�͂��@", 30, 780, 380, (m_pParent->m_iAnswer == TRUE) ? 0xffffff00 : 0x80ffffff);
				tdnFont::RenderString("���� :", 30, 680, 420);
				tdnFont::RenderString("������", 30, 780, 420, (m_pParent->m_iAnswer == FALSE) ? 0xffffff00 : 0x80ffffff);
			}
		};
	};
	std::unique_ptr<AnswerAction::Base> m_pAnswerAction;


	//------------------------------------------------------
	//	����
	//------------------------------------------------------
	int m_iStep;
	static const int NO_ACTION = -1;

	//------------------------------------------------------
	//	�v���C���[�̑I����������
	//------------------------------------------------------
	int m_iAnswer;
	static const int NO_ANSWER = -1;

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	LPCSTR m_lpcHeading;		// ���o���̕�����
	LPCSTR m_lpcQuestion;		// ����̕�����

	//------------------------------------------------------
	//	�摜
	//------------------------------------------------------
	std::unique_ptr<tdn2DAnim> m_pWindowImage;	// �g�̉摜
};