#include "TDNLib.h"
#include "../Input/InputMIDI.h"	// ������C���N���[�h���Ȃ���ifdef���K������Ȃ�
#include "../Music/MusicInfo.h"
#include "ui.h"
#include "../Music/PlayCursor.h"
#include "../Music/SoflanManager.h"
#include "../Music/NoteManager.h"
#include "../Music/BeatManager.h"
#include "../Data/CustomizeData.h"
#include "../Data/GameScore.h"
#include "../Data/PlayerData.h"
#include "../Number/Number.h"
#include "../Music/BeatBar.h"
#include "../Scene/SceneMain.h"

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
UI::UI()
{
	
}
void UI::Initialize(SceneMain *pMain, GAUGE_OPTION GaugeOP)
{
	m_pMain = pMain;

	// �摜������

	// �L�[���C��
	m_pImages[IMAGE::BACK] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/back.png");

	//// �K�C�h
	//m_pImages[IMAGE::NOTE_GUIDE] = std::make_unique<tdn2DObj>("DATA/UI/Playing/guide_keyboard.png");
	//m_pImages[IMAGE::GUIDE] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/guide_keyboard.png");

	//// xx�X�e�[�W
	//m_pImages[IMAGE::STAGE] = std::make_unique<tdn2DObj>("DATA/UI/MusicSelect/stage.png");
	//m_pImages[IMAGE::STAGE]->SetScale(.65f);

	//// �X�L��
	//m_pImages[IMAGE::SKIN] = std::make_unique<tdn2DObj>(CustomizeDataMgr->GetSetedSkin(CUSTOMIZE_TYPE::SKIN).c_str());

	// �m�[�g
	m_pImages[IMAGE::NOTE] = std::make_unique<tdn2DObj>("DATA/Customize/Note/default.png");

	//// �T�h�v��
	//m_pImages[IMAGE::SUDDEN] = std::make_unique<tdn2DObj>(CustomizeDataMgr->GetSetedSkin(CUSTOMIZE_TYPE::SUDDEN).c_str());

	//// �Ō��r�[��
	//m_pImages[IMAGE::BEAM] = std::make_unique<tdn2DObj>("DATA/UI/Playing/beam_b.png");

	//// �����t�H���g
	//m_pImages[IMAGE::NUMBER] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/Fdefault.png");

	//// ��Փx
	//m_pImages[IMAGE::DIFFICULTY] = std::make_unique<tdn2DObj>("DATA/UI/Other/difficulty.png");

	// ����o�[
	m_pImages[IMAGE::JUDGE_BAR] = std::make_unique<tdn2DObj>("DATA/UI/Playing/judge_line.png");

	//// ����������Ȃ���
	//m_pImages[IMAGE::ON_LIGHT] = std::make_unique<tdn2DObj>("DATA/UI/Playing/push.png");
	//
	//// �Q�[�W
	//switch (GaugeOP)
	//{
	//case GAUGE_OPTION::NORMAL:
	//case GAUGE_OPTION::EASY:
	//	m_iGaugeSrcY = 0;
	//	break;
	//case GAUGE_OPTION::HARD:
	//case GAUGE_OPTION::GRADE:
	//	m_iGaugeSrcY = 32;
	//	break;
	//case GAUGE_OPTION::EX_HARD:
	//case GAUGE_OPTION::HAZARD:
	//	m_iGaugeSrcY = 64;
	//	break;
	//}
	//m_pImages[IMAGE::GAUGE] = std::make_unique<tdn2DObj>("DATA/UI/Playing/gauge.png");

	//memset(m_bBeam, false, sizeof(m_bBeam));
}

//*****************************************************************************************************************************
//	�`��
//*****************************************************************************************************************************
//------------------------------------------------------
//	�`��
//------------------------------------------------------
void UI::Render()
{
	//m_pImages[IMAGE::BACK]->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, 1280, 720);
	//m_pImages[IMAGE::KEYBOARD]->Render(KEYLANE_LEFT, 525, 1120, 150, 0, 0, 1120, 150);
	//m_pImages[IMAGE::KEYLINE]->Render(KEYLANE_LEFT, 0, 1120, KEYLANE_HEIGHT, 0, 0, 1120, 539);

	//// �Ō��r�[���`��
	//BeamRender();

	// rhythm�o�[
	m_pMain->m_pBeatBar->Render(0, JUDGE_LINE - 8 - 16);

	// ����o�[
	m_pImages[IMAGE::JUDGE_BAR]->Render(0, JUDGE_LINE - 8, 1120, 32, 0, 0, 1120, 32);

	// �m�[�c�`��
	NoteRender();

	//// �T�h�v���`��
	//SuddenRender();

	//// �I�v�V�����`��
	//if (KeyBoard(KB_SPACE))OptionRender();

	//// �X�L���t���[��
	//m_pImages[IMAGE::SKIN]->Render(0, 0);

	//// ���݂���
	//PushRender();

	// ���`��
	InfoRender();
}

void UI::PushRender()
{
	//FOR(NUM_KEYBOARD)
	//{
	//	if (m_bBeam[i])
	//	{
	//		// �������Ƃ��낪����Q�[��
	//		m_pImages[IMAGE::ON_LIGHT]->Render(i / 12 * 280, 630, 280, 90, 0, (i % 12) * 90, 280, 90);
	//	}
	//}
}

void UI::RenderBack()
{
	// �X�L���̉��n
	m_pImages[IMAGE::BACK]->Render(0, 133);
}

void UI::InfoRender()
{
	//// �X�R�A
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1248, 172, 24, 32, m_pGameScore->score, 6, false);

	//// �ő�R���{��
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1225, 277, 24, 32, m_pGameScore->MaxCombo, 4, false);

	//// �e���萔
	//for (int i = 0; i < (int)JUDGE_TYPE::MAX; i++)
	//{
	//	NumberRender(m_pImages[IMAGE::NUMBER].get(), 1246, 385 + i * 32, 24, 32, m_pGameScore->JudgeCount[i], 4, false);
	//}

	//// BPM
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1210, 585, 24, 32, (int)m_pSoflanMgr->GetBPM(), 3, false);
	tdnText::Draw(1150, 585, 0xffffffff, "BPM : %.1f", m_pMain->m_pSoflanMgr->GetBPM());

	// ���q
	tdnText::Draw(1150, 605, 0xffffffff, "���q : %d / %d", m_pMain->m_pBeatMgr->GetBeatData()->byNumerator, m_pMain->m_pBeatMgr->GetBeatData()->byDenominator);

	//// �n�C�X�s
	//int iHiSpeed = (int)((PlayerDataMgr->m_PlayerOption.HiSpeed * HI_SPEED_WIDTH) * 100);
	//m_pImages[IMAGE::NUMBER]->Render(1235, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//iHiSpeed /= 10;
	//m_pImages[IMAGE::NUMBER]->Render(1210, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//iHiSpeed /= 10;
	//m_pImages[IMAGE::NUMBER]->Render(1170, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//m_pImages[IMAGE::NUMBER]->Render(1195, 678, 16, 32, 240, 0, 16, 32);

	//// �X�e�[�W
	//switch (ModeMgr->GetMode())
	//{
	//	case PLAY_MODE::STANDARD:
	//		m_pImages[IMAGE::STAGE]->Render(1020, 60, 280, 64, 0, ModeMgr->GetStep() * 64, 280, 64);
	//		break;
	//	case PLAY_MODE::FREE:
	//		m_pImages[IMAGE::STAGE]->Render(1020, 60, 280, 64, 0, 256, 280, 64);
	//		break;
	//	case PLAY_MODE::GRADE:
	//		m_pImages[IMAGE::STAGE]->Render(1020, 60, 280, 64, 0, 320, 280, 64);
	//		break;
	//}

	//// �Q�[�W
	//float GaugePct = (GameScoreMgr->GetGameScore()->FinesseGauge / 100.0f);
	//m_pImages[IMAGE::GAUGE]->Render(32, 32, (int)(500 * GaugePct), 32, 0, m_iGaugeSrcY, (int)(500 * GaugePct), 32);
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 462, 62, 24, 32, GameScoreMgr->GetGameScore()->FinesseGauge, 3, false);
}

void UI::NoteRender()
{
	// �m�[�c���X�g���[�v
	for (auto it = m_pMain->m_pNoteMgr->GetList()->begin(); it != m_pMain->m_pNoteMgr->GetList()->end();)
	{
		int l_iPosX = (*it)->pNoteEP->byLaneNo, l_iPosY;
		// �m�[�c���W(Y)�Ƃ��Ă���
		m_pMain->m_pNoteMgr->GetNoteHiSpeedPos(*(*it), &l_iPosY);

		// ��CN�J�n�m�[�c��������
		if ((*it)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_START)
		{
			// �m�[�c���\�����C�����ゾ������A���̃��[���͕\������K�v�Ȃ��B(���̍l�����f���炵���œK��)
			if (l_iPosY + JUDGE_LINE < KEYLANE_TOP)
				break;

			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// �J�n�m�[�c�`��
			m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, 16, 40 * key_col, 16, 40, 16);

			// �K�C�h�`��
			//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + l_iPosY, i);

			// CN�J�n�m�[�c����̂ŁA���̎���CN�I���m�[�c�ł��邱�Ƃ��ۏႳ��Ă�͂��c�킯�Ȃ�����Ȃɐl���Â��Ȃ�
			for (auto it2 = it;; it2++)
			{
				if (((*it2)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_END) == 0) continue;
				if ((*it2)->pNoteEP->byLaneNo != (*it)->pNoteEP->byLaneNo) continue;

				// �I���m�[�c�̏I�����W(Y)
				int EndPosY;
				m_pMain->m_pNoteMgr->GetNoteHiSpeedPos(*(*it2), &EndPosY);

				// �o�Ă���Ƃ�����ォ�ǂ���
				if (EndPosY + JUDGE_LINE < KEYLANE_TOP)
				{
					// �J�n�ƏI���̊Ԃ̐�
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, KEYLANE_TOP, 40, (l_iPosY + JUDGE_LINE - KEYLANE_TOP), 80 + 40 * key_col, 0, 40, 16);
				}
				else
				{
					// �J�n�ƏI���̊Ԃ̐�
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + EndPosY, 40, (l_iPosY - EndPosY), 80 + 40 * key_col, 0, 40, 16);
					// �I���m�[�c�`��
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + EndPosY, 40, 16, 40 * key_col, 16, 40, 16);

					// �K�C�h�`��
					//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + EndPosY, i);
				}
				break;
			}
		}

		// ��CN�I���m�[�c��������(�����ɗ���Ƃ������́A�J�n�m�[�c�͏������ŁA�I���m�[�c�������Ă���Ƃ�����)
		else if ((*it)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_END)
		{
			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// �o�Ă���Ƃ�����ォ�ǂ���
			if (l_iPosY + JUDGE_LINE < KEYLANE_TOP)
			{
				// �J�n�ƏI���̊Ԃ̐�
				//m_pImages[IMAGE::NOTE]->Render(l_iPosX, KEYLANE_TOP, 40, (JUDGE_LINE - KEYLANE_TOP), 80 + 40 * key_col, 0, 40, 16);
			}
			else
			{
				// �J�n�ƏI���̊Ԃ̐�
				//m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, (-l_iPosY), 80 + 40 * key_col, 0, 40, 16);
				// �I���m�[�c�`��
				m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, 16, 40 * key_col, 16, 40, 16);

				// �K�C�h�`��
				//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + l_iPosY, i);
			}
		}

		// �ʏ�m�[�c
		else
		{
			l_iPosY += JUDGE_LINE;

			// �m�[�c���\�����C�����ゾ������A���̃��[���͕\������K�v�Ȃ��B(���̍l�����f���炵���œK��)
			if (l_iPosY < KEYLANE_TOP)
				break;

			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// �n�C�X�s
			//pos_y = (int)(pos_y / m_pSoflanMgr->m_ticks[m_pSoflanMgr->m_SoflanStep]);
			//pos_y = (int)(pos_y * .4f);

			m_pImages[IMAGE::NOTE]->Render(l_iPosX, l_iPosY, 40, 16, 40 * key_col, 0, 40, 16);

			// �K�C�h�`��
			//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, l_iPosY, i);
		}

		// �C�e���[�^�[�i�߂�
		it++;
	}
}

void UI::BeamRender()
{
	//FOR(NUM_KEYBOARD)
	//{
	//	if (m_bBeam[i])
	//	{
	//		int pos_x = NOTE_POS_COMPTE(i);
	//		// �r�[��
	//		m_pImages[IMAGE::BEAM]->Render(pos_x, 320, NOTE_WIDTH, JUDGE_LINE - 320, 0, 0, NOTE_WIDTH, 720);
	//	}
	//}
}

void UI::SuddenRender()
{
	//// �T�h�v���I���Ȃ�
	//if (PlayerDataMgr->m_PlayerOption.bSudden)
	//{
	//	//int l_iPosY = PlayerDataMgr->m_PlayerOption.SuddenPos;
	//	//m_pImages[IMAGE::SUDDEN]->Render(0, l_iPosY - 300);

	//	const int SuddenPos = PlayerDataMgr->m_PlayerOption.SuddenPos;
	//	const float percentage = (SuddenPos / 420.0f);
	//	m_pImages[IMAGE::SUDDEN]->Render(0, 133, 1120, 60 + (int)(440 * percentage), 0, 500 - (60 + (int)(440 * percentage)), 1120, 60 + (int)(440 * percentage));
	//}
}

void UI::OptionRender()
{
	//// �T�h�v���I���Ȃ�
	//if (PlayerDataMgr->m_PlayerOption.bSudden)
	//	tdnText::Draw(560, PlayerDataMgr->m_PlayerOption.SuddenPos + 140, 0xffffff00, "%d", PlayerDataMgr->m_PlayerOption.SuddenPos);	// �T�h�v���̐���

	//// �n�C�X�s
}

void UI::RenderMusicName(char *name)
{
	tdnText::Draw(745, 24, 0xffffffff, "%s", name);
}

void UI::GuideNoteRender(int x, int y, int lane)
{

}


//*****************************************************************************************************************************
//	�Z�b�g
//*****************************************************************************************************************************
//------------------------------------------------------
//	�r�[��
//------------------------------------------------------
void UI::SetBeam(char no, bool on)
{
	//m_bBeam[no] = on;
}
void UI::SetBeam(bool OnList[])
{
	//memcpy_s(m_bBeam, sizeof(bool)*NUM_KEYBOARD, OnList, sizeof(bool)*NUM_KEYBOARD);
}


//*****************************************************************************************************************************
//	�����჋�[��
//*****************************************************************************************************************************
//------------------------------------------------------
//	����������������
//------------------------------------------------------
BOOL UI::isBlackKey(int lane)
{
	int l_iWork(lane);
	l_iWork %= 12;

	return (l_iWork == 1 || l_iWork == 3 || l_iWork == 6 || l_iWork == 8 || l_iWork == 10) ? TRUE : FALSE;
}
//void UI::NumberRender(int x, int y, int val, int NumDigit)
//{
//	for (int i = 0; i < NumDigit; i++)
//	{
//		m_pImages[IMAGE::NUMBER]->Render(x + 24 * (NumDigit - i), y, 24, 32, (val % 10) * 24, 0, 24, 32);
//		val /= 10;
//	}
//}