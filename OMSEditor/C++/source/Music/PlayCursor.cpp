#include	"TDNLIB.h"
#include	"PlayCursor.h"
#include "../Sound/SoundManager.h"
#include <time.h>

//*****************************************************************************************************************************
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
PlayCursor::PlayCursor() :m_pSoundPlayer(nullptr), m_bSoundPlay(false), m_lpcFileName(nullptr), m_sShift(0), m_dwOffset(0){}
void PlayCursor::Initialize()
{
	m_pSoundPlayer = nullptr;
	m_lpcFileName = nullptr;
	m_bSoundPlay = false;
}
//------------------------------------------------------
//	解放
//------------------------------------------------------
PlayCursor::~PlayCursor()
{
	SAFE_DELETE(m_pSoundPlayer);
}


//*****************************************************************************************************************************
//		サウンドアクセス
//*****************************************************************************************************************************
//------------------------------------------------------
//		音源セット
//------------------------------------------------------
void PlayCursor::SetMusic(LPCSTR lpcFileName)
{
	//tdnDebug::OutPutLog("Play:%s\n", lpcFileName);

	// 曲設定、nullなら曲なし
	if(!m_pSoundPlayer)m_pSoundPlayer = new tdnSoundBGM;
	m_pSoundPlayer->Set(0, lpcFileName);

	//tdnDebug::OutPutLog("あ");

	m_bSoundPlay = true;
}
//void PlayCursor::SetMusic(LPBYTE data, DWORD size, LPWAVEFORMATEX wfx)
//{
//	if (!m_player) m_player = new tdnSoundBGM;
//	m_player->Set(0, data, size, wfx);
//}

//------------------------------------------------------
//		再生・停止
//------------------------------------------------------
void PlayCursor::Play(int Mlimit)
{
	//tdnDebug::OutPutLog("い");

	// 曲が入ってるなら、それを再生
	if (m_bSoundPlay)
	{
		m_pSoundPlayer->PlayOffset(0, false, m_dwOffset);
	}

	// そうでないならclock()でタイム計測
	else
	{
		// 再生時間が設定されていない(デフォルト引数の0になってる)
		if (Mlimit == 0) MessageBox(0, "音源情報がないのに再生時間が設定されていない", "エラー", MB_OK);
		m_iLimit = Mlimit;
		m_iStart = clock();
	}
	//tdnDebug::OutPutLog("う");

	m_iMSecond = m_iDelta= 0;
}
void PlayCursor::Stop()
{
	if (m_bSoundPlay)
	{
		m_pSoundPlayer->Stop(0);
	}
	else
	{
		m_iMSecond = m_iStart = 0;
	}
}
//------------------------------------------------------
//		現在再生位置取得
//------------------------------------------------------
int PlayCursor::GetMSecond()
{
	// 時間格納変数
	int val;

	// 曲が再生してたら、directSoundの再生座標とってくる関数
	if (m_bSoundPlay)
	{
		//val = m_pStreaming->GetPlayCursor();
		//
		//if (val >= m_delta)
		//{
		//	m_delta = val;
		//	val += m_MSecond;
		//}
		//else
		//{
		//	m_MSecond += tdnStreamSound::STRSECOND * 1000;
		//	m_delta = val;
		//}
		//
		//val += m_MSecond;

		val = m_pSoundPlayer->GetPlayMSecond(0);
	}

	// そうじゃなかったら計った時間と今の時間の差分
	else
	{
		m_iMSecond = clock() - m_iStart;
		val = m_iMSecond;
	}

	// 実際の時間に補正値を加算したのを返す
	return val + m_sShift;
}


//------------------------------------------------------
//		終わったかどうか
//------------------------------------------------------
bool PlayCursor::isEnd()
{
	if (m_bSoundPlay)return !m_pSoundPlayer->isPlay(0);	// 曲が再生してるかどうか
	else
	{
		return (clock() - m_iStart >= m_iLimit);	// 設定した制限時間に達したか
	}
}