#include "TDNLib.h"
#include "../Input/InputMIDI.h"	// これをインクルードしないとifdefが適応されない
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
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
UI::UI()
{
	
}
void UI::Initialize(SceneMain *pMain, GAUGE_OPTION GaugeOP)
{
	m_pMain = pMain;

	// 画像初期化

	// キーライン
	m_pImages[IMAGE::BACK] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/back.png");

	//// ガイド
	//m_pImages[IMAGE::NOTE_GUIDE] = std::make_unique<tdn2DObj>("DATA/UI/Playing/guide_keyboard.png");
	//m_pImages[IMAGE::GUIDE] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/guide_keyboard.png");

	//// xxステージ
	//m_pImages[IMAGE::STAGE] = std::make_unique<tdn2DObj>("DATA/UI/MusicSelect/stage.png");
	//m_pImages[IMAGE::STAGE]->SetScale(.65f);

	//// スキン
	//m_pImages[IMAGE::SKIN] = std::make_unique<tdn2DObj>(CustomizeDataMgr->GetSetedSkin(CUSTOMIZE_TYPE::SKIN).c_str());

	// ノート
	m_pImages[IMAGE::NOTE] = std::make_unique<tdn2DObj>("DATA/Customize/Note/default.png");

	//// サドプラ
	//m_pImages[IMAGE::SUDDEN] = std::make_unique<tdn2DObj>(CustomizeDataMgr->GetSetedSkin(CUSTOMIZE_TYPE::SUDDEN).c_str());

	//// 打鍵ビーム
	//m_pImages[IMAGE::BEAM] = std::make_unique<tdn2DObj>("DATA/UI/Playing/beam_b.png");

	//// 数字フォント
	//m_pImages[IMAGE::NUMBER] = std::make_unique<tdn2DObj>("DATA/Customize/Skin/Fdefault.png");

	//// 難易度
	//m_pImages[IMAGE::DIFFICULTY] = std::make_unique<tdn2DObj>("DATA/UI/Other/difficulty.png");

	// 判定バー
	m_pImages[IMAGE::JUDGE_BAR] = std::make_unique<tdn2DObj>("DATA/UI/Playing/judge_line.png");

	//// 押したら青くなるやつ
	//m_pImages[IMAGE::ON_LIGHT] = std::make_unique<tdn2DObj>("DATA/UI/Playing/push.png");
	//
	//// ゲージ
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
//	描画
//*****************************************************************************************************************************
//------------------------------------------------------
//	描画
//------------------------------------------------------
void UI::Render()
{
	//m_pImages[IMAGE::BACK]->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, 1280, 720);
	//m_pImages[IMAGE::KEYBOARD]->Render(KEYLANE_LEFT, 525, 1120, 150, 0, 0, 1120, 150);
	//m_pImages[IMAGE::KEYLINE]->Render(KEYLANE_LEFT, 0, 1120, KEYLANE_HEIGHT, 0, 0, 1120, 539);

	//// 打鍵ビーム描画
	//BeamRender();

	// rhythmバー
	m_pMain->m_pBeatBar->Render(0, JUDGE_LINE - 8 - 16);

	// 判定バー
	m_pImages[IMAGE::JUDGE_BAR]->Render(0, JUDGE_LINE - 8, 1120, 32, 0, 0, 1120, 32);

	// ノーツ描画
	NoteRender();

	//// サドプラ描画
	//SuddenRender();

	//// オプション描画
	//if (KeyBoard(KB_SPACE))OptionRender();

	//// スキンフレーム
	//m_pImages[IMAGE::SKIN]->Render(0, 0);

	//// ごみくそ
	//PushRender();

	// 情報描画
	InfoRender();
}

void UI::PushRender()
{
	//FOR(NUM_KEYBOARD)
	//{
	//	if (m_bBeam[i])
	//	{
	//		// 押したところが光るゲーム
	//		m_pImages[IMAGE::ON_LIGHT]->Render(i / 12 * 280, 630, 280, 90, 0, (i % 12) * 90, 280, 90);
	//	}
	//}
}

void UI::RenderBack()
{
	// スキンの下地
	m_pImages[IMAGE::BACK]->Render(0, 133);
}

void UI::InfoRender()
{
	//// スコア
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1248, 172, 24, 32, m_pGameScore->score, 6, false);

	//// 最大コンボ数
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1225, 277, 24, 32, m_pGameScore->MaxCombo, 4, false);

	//// 各判定数
	//for (int i = 0; i < (int)JUDGE_TYPE::MAX; i++)
	//{
	//	NumberRender(m_pImages[IMAGE::NUMBER].get(), 1246, 385 + i * 32, 24, 32, m_pGameScore->JudgeCount[i], 4, false);
	//}

	//// BPM
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 1210, 585, 24, 32, (int)m_pSoflanMgr->GetBPM(), 3, false);
	tdnText::Draw(1150, 585, 0xffffffff, "BPM : %.1f", m_pMain->m_pSoflanMgr->GetBPM());

	// 拍子
	tdnText::Draw(1150, 605, 0xffffffff, "拍子 : %d / %d", m_pMain->m_pBeatMgr->GetBeatData()->byNumerator, m_pMain->m_pBeatMgr->GetBeatData()->byDenominator);

	//// ハイスピ
	//int iHiSpeed = (int)((PlayerDataMgr->m_PlayerOption.HiSpeed * HI_SPEED_WIDTH) * 100);
	//m_pImages[IMAGE::NUMBER]->Render(1235, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//iHiSpeed /= 10;
	//m_pImages[IMAGE::NUMBER]->Render(1210, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//iHiSpeed /= 10;
	//m_pImages[IMAGE::NUMBER]->Render(1170, 678, 24, 32, (iHiSpeed % 10) * 24, 0, 24, 32);
	//m_pImages[IMAGE::NUMBER]->Render(1195, 678, 16, 32, 240, 0, 16, 32);

	//// ステージ
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

	//// ゲージ
	//float GaugePct = (GameScoreMgr->GetGameScore()->FinesseGauge / 100.0f);
	//m_pImages[IMAGE::GAUGE]->Render(32, 32, (int)(500 * GaugePct), 32, 0, m_iGaugeSrcY, (int)(500 * GaugePct), 32);
	//NumberRender(m_pImages[IMAGE::NUMBER].get(), 462, 62, 24, 32, GameScoreMgr->GetGameScore()->FinesseGauge, 3, false);
}

void UI::NoteRender()
{
	// ノーツリストループ
	for (auto it = m_pMain->m_pNoteMgr->GetList()->begin(); it != m_pMain->m_pNoteMgr->GetList()->end();)
	{
		int l_iPosX = (*it)->pNoteEP->byLaneNo, l_iPosY;
		// ノーツ座標(Y)とってくる
		m_pMain->m_pNoteMgr->GetNoteHiSpeedPos(*(*it), &l_iPosY);

		// ★CN開始ノーツだったら
		if ((*it)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_START)
		{
			// ノーツが表示ラインより上だったら、そのレーンは表示する必要なし。(俺の考えた素晴らしい最適化)
			if (l_iPosY + JUDGE_LINE < KEYLANE_TOP)
				break;

			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// 開始ノーツ描画
			m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, 16, 40 * key_col, 16, 40, 16);

			// ガイド描画
			//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + l_iPosY, i);

			// CN開始ノーツあるので、その次はCN終了ノーツであることが保障されてるはず…わけないそんなに人生甘くない
			for (auto it2 = it;; it2++)
			{
				if (((*it2)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_END) == 0) continue;
				if ((*it2)->pNoteEP->byLaneNo != (*it)->pNoteEP->byLaneNo) continue;

				// 終了ノーツの終わり座標(Y)
				int EndPosY;
				m_pMain->m_pNoteMgr->GetNoteHiSpeedPos(*(*it2), &EndPosY);

				// 出てくるところより上かどうか
				if (EndPosY + JUDGE_LINE < KEYLANE_TOP)
				{
					// 開始と終了の間の線
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, KEYLANE_TOP, 40, (l_iPosY + JUDGE_LINE - KEYLANE_TOP), 80 + 40 * key_col, 0, 40, 16);
				}
				else
				{
					// 開始と終了の間の線
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + EndPosY, 40, (l_iPosY - EndPosY), 80 + 40 * key_col, 0, 40, 16);
					// 終了ノーツ描画
					m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + EndPosY, 40, 16, 40 * key_col, 16, 40, 16);

					// ガイド描画
					//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + EndPosY, i);
				}
				break;
			}
		}

		// ★CN終了ノーツだったら(ここに来るという事は、開始ノーツは処理中で、終了ノーツだけ来ているという事)
		else if ((*it)->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_END)
		{
			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// 出てくるところより上かどうか
			if (l_iPosY + JUDGE_LINE < KEYLANE_TOP)
			{
				// 開始と終了の間の線
				//m_pImages[IMAGE::NOTE]->Render(l_iPosX, KEYLANE_TOP, 40, (JUDGE_LINE - KEYLANE_TOP), 80 + 40 * key_col, 0, 40, 16);
			}
			else
			{
				// 開始と終了の間の線
				//m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, (-l_iPosY), 80 + 40 * key_col, 0, 40, 16);
				// 終了ノーツ描画
				m_pImages[IMAGE::NOTE]->Render(l_iPosX, JUDGE_LINE + l_iPosY, 40, 16, 40 * key_col, 16, 40, 16);

				// ガイド描画
				//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, JUDGE_LINE + l_iPosY, i);
			}
		}

		// 通常ノーツ
		else
		{
			l_iPosY += JUDGE_LINE;

			// ノーツが表示ラインより上だったら、そのレーンは表示する必要なし。(俺の考えた素晴らしい最適化)
			if (l_iPosY < KEYLANE_TOP)
				break;

			BOOL key_col = isBlackKey(l_iPosX);
			l_iPosX = NOTE_POS_COMPTE(l_iPosX);

			// ハイスピ
			//pos_y = (int)(pos_y / m_pSoflanMgr->m_ticks[m_pSoflanMgr->m_SoflanStep]);
			//pos_y = (int)(pos_y * .4f);

			m_pImages[IMAGE::NOTE]->Render(l_iPosX, l_iPosY, 40, 16, 40 * key_col, 0, 40, 16);

			// ガイド描画
			//if (PlayerDataMgr->m_PlayerOption.bGuide)GuideNoteRender(l_iPosX, l_iPosY, i);
		}

		// イテレーター進める
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
	//		// ビーム
	//		m_pImages[IMAGE::BEAM]->Render(pos_x, 320, NOTE_WIDTH, JUDGE_LINE - 320, 0, 0, NOTE_WIDTH, 720);
	//	}
	//}
}

void UI::SuddenRender()
{
	//// サドプラオンなら
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
	//// サドプラオンなら
	//if (PlayerDataMgr->m_PlayerOption.bSudden)
	//	tdnText::Draw(560, PlayerDataMgr->m_PlayerOption.SuddenPos + 140, 0xffffff00, "%d", PlayerDataMgr->m_PlayerOption.SuddenPos);	// サドプラの数字

	//// ハイスピ
}

void UI::RenderMusicName(char *name)
{
	tdnText::Draw(745, 24, 0xffffffff, "%s", name);
}

void UI::GuideNoteRender(int x, int y, int lane)
{

}


//*****************************************************************************************************************************
//	セット
//*****************************************************************************************************************************
//------------------------------------------------------
//	ビーム
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
//	ごちゃルーム
//*****************************************************************************************************************************
//------------------------------------------------------
//	白鍵か黒鍵か判定
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