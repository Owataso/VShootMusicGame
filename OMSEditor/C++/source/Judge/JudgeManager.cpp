#include "tdnlib.h"
#include "../Music/MusicInfo.h"
#include "JudgeManager.h"
#include "../Music/NoteManager.h"
#include "JudgeAction.h"
#include "../Data/GameScore.h"
#include "Gauge.h"
#include "../Scene/SceneMain.h"

//*****************************************************************************************************************************
//	判定して結果を返す
//*****************************************************************************************************************************
JUDGE_TYPE JudgeManager::ComputeJudgeResult(int iPosY, BOOL bUp)
{
	// 各判定幅
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
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
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

	// ゲームスコア管理クラスのスコアをアドレス参照
	m_GameScore = GameScoreMgr->GetGameScore();

	// 判定SE
	m_pSE = std::make_unique<tdnSoundSE>();
	m_pSE->Set(0, 12, "DATA/Sound/SE/clap.wav");

	// ゲージくん初期化
	//m_GaugeManager = new GaugeManager(op, &m_GameScore->FinesseGauge);

	// なんかリセット
	Reset();
}

//------------------------------------------------------
//	解放
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
//		更新・描画
//*****************************************************************************************************************************
//------------------------------------------------------
//	更新
//------------------------------------------------------
bool JudgeManager::Update()
{
	m_pJudgeActMgr->Update();

	// 閉店フラグを返す
	//return m_GaugeManager->isFailed();
	return false;
}

//------------------------------------------------------
//	描画
//------------------------------------------------------
void JudgeManager::Render()
{
	m_pJudgeActMgr->Render();
}


//*****************************************************************************************************************************
//		ノート判定
//*****************************************************************************************************************************
void JudgeManager::JudgeNote(char cLaneNo)
{
	// ノーツリストループ
	for (auto &it : *m_pRefNoteMgr->GetList())
	{
		int l_iPosY;
		m_pRefNoteMgr->GetNoteAbsolutePos((*it), &l_iPosY);	// ノーツの絶対位置

		JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

		// 絶対値処理
		l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

		JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, FALSE));

		if(JudgeResult == JUDGE_TYPE::SUPER)
		{
			l_eJudgeTiming = JUDGE_TIMING::JUST;
			m_pSE->Play(0);
		}

		// 何かしらの判定を起こしていたら
		if (JudgeResult != JUDGE_TYPE::MAX)
		{
			// 判定個数更新
			m_GameScore->JudgeCount[(int)JudgeResult]++;

			// コンボ数更新
			m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
			// 最大コンボ数更新
			m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

			// ジャッジアクション君にデータ送信
			m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);

			// ゲージ増減
			//m_GaugeManager->Compute(JudgeResult);

			// スコア計算
			m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

			// ノーツ消す
			it->bErase = true;

			// ノーツがCN開始だったらCNフラグON
			if (it->pNoteEP->byNoteType & (BYTE)NOTE_TYPE::CN_START) m_bCN[cLaneNo] = true;
		}
		break;
	}
}

void JudgeManager::JudgeNote(PlayNoteInfo * note)
{
	int l_iPosY;
	m_pRefNoteMgr->GetNoteAbsolutePos(*note, &l_iPosY);	// ノーツの絶対位置

	JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

	// 絶対値処理
	l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

	JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, FALSE));

	if (JudgeResult == JUDGE_TYPE::SUPER)
	{
		l_eJudgeTiming = JUDGE_TIMING::JUST;
		m_pSE->Play(0);
	}

	// 何かしらの判定を起こしていたら
	if (JudgeResult != JUDGE_TYPE::MAX)
	{
		// 判定個数更新
		m_GameScore->JudgeCount[(int)JudgeResult]++;

		// コンボ数更新
		m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
		// 最大コンボ数更新
		m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

		// ジャッジアクション君にデータ送信
		//m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);
		m_pJudgeActMgr->Set(note->pNoteVRShoot->GetCenterScreenPosition(m_pRefSceneMain->m_iMaxDegree) - Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE), m_iCombo, JudgeResult, l_eJudgeTiming);

		// ゲージ増減
		//m_GaugeManager->Compute(JudgeResult);

		// スコア計算
		m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

		// ノーツ消す
		note->bErase = true;

		// ノーツがCN開始だったらCNフラグON
		//if (note->pNoteVRShoot->byNoteType & (BYTE)NOTE_TYPE::CN_START) m_bCN[cLaneNo] = true;
	}
}

void JudgeManager::JudgeUpKey(char cLaneNo)
{
	// CN押してなかったら出ていけぇ！！
	if (!m_bCN[cLaneNo]) return;

	int l_iPosY;
	auto it = *m_pRefNoteMgr->GetList()->begin();
	m_pRefNoteMgr->GetNoteAbsolutePos((*it), &l_iPosY);	// ノーツの絶対位置

	JUDGE_TIMING l_eJudgeTiming((l_iPosY > 0) ? JUDGE_TIMING::SLOW : JUDGE_TIMING::FAST);

	// 絶対値処理
	l_iPosY = (l_iPosY < 0) ? l_iPosY * -1 : l_iPosY;

	JUDGE_TYPE JudgeResult(ComputeJudgeResult(l_iPosY, TRUE));

	if(JudgeResult == JUDGE_TYPE::SUPER)
	{
		l_eJudgeTiming = JUDGE_TIMING::JUST;
		m_pSE->Play(0);
	}

	if (JudgeResult == JUDGE_TYPE::MAX) JudgeResult = JUDGE_TYPE::BAD;

	// 判定個数更新
	m_GameScore->JudgeCount[(int)JudgeResult]++;

	// コンボ数更新
	m_iCombo = (JudgeResult == JUDGE_TYPE::BAD) ? 0 : m_iCombo + 1;
	// 最大コンボ数更新
	m_GameScore->MaxCombo = max(m_GameScore->MaxCombo, m_iCombo);

	// ジャッジアクション君にデータ送信
	m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JudgeResult, l_eJudgeTiming);

	// ゲージ増減
	//m_GaugeManager->Compute(JudgeResult);

	// スコア計算
	m_GameScore->ComputeScore(m_pRefNoteMgr->GetNumNorts());

	// ノーツ消す
	it->bErase = true;

	// フラグ終了
	m_bCN[cLaneNo] = false;
}

void JudgeManager::LostNote(char cLaneNo)
{
	// BAD判定カウント
	m_GameScore->JudgeCount[(int)JUDGE_TYPE::BAD]++;

	// コンボ数更新
	m_iCombo = 0;

	// ジャッジアクション君にデータ送信
	m_pJudgeActMgr->Set(cLaneNo, m_iCombo, JUDGE_TYPE::BAD, JUDGE_TIMING::SLOW);

	// ゲージ減
	//m_GaugeManager->Compute(JUDGE_TYPE::BAD);

	// CN押し越しを想定して、CNフラグをオフにする
	m_bCN[cLaneNo] = false;
}

void JudgeManager::Reset()
{
	// スコアとゲージリセット
	GameScoreMgr->ResetExceptGauge();	// 段位用にゲージを残しての初期化を行う

	//// ゲージオプションに応じて初期ゲージの値を設定
	//int StartGauge;
	//switch (m_GaugeManager->GetGaugeOption())
	//{
	//	// 22%スタート
	//case GAUGE_OPTION::NORMAL:
	//case GAUGE_OPTION::EASY:
	//	StartGauge = 22;
	//	break;

	//	// 100%スタート
	//case GAUGE_OPTION::HARD:
	//case GAUGE_OPTION::EX_HARD:
	//case GAUGE_OPTION::HAZARD:
	//	StartGauge = 100;
	//	break;

	//	// 前のゲージからのスタート
	//case GAUGE_OPTION::GRADE:
	//	StartGauge = GameScoreMgr->GetGameScore()->FinesseGauge;
	//	break;
	//}
	//m_GaugeManager->GaugeSet(StartGauge);
	m_iCombo = 0;

	// 判定アクションクリア
	m_pJudgeActMgr->Clear();

	// ノーツ回復量設定
	//m_GaugeManager->SetNumNorts(m_pRefNoteMgr->GetNumNorts());

	// CNフラグオフ
	memset(m_bCN, false, sizeof(bool)*NUM_KEYBOARD);
}

// フルコン演出中かどうか
bool JudgeManager::isFullComboNow(){ return m_pJudgeActMgr->isFullComboNow(); }
bool JudgeManager::isFullComboEnd(){ return m_pJudgeActMgr->isFullComboEnd(); }