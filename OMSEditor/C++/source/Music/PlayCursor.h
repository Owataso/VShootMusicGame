#pragma once

/********************************************/
//	サウンドの再生位置管理
/********************************************/
class PlayCursor
{
public:
	//===============================================
	//	初期化・解放
	//===============================================
	PlayCursor();
	void Initialize();
	~PlayCursor();

	//===============================================
	//	サウンド制御
	//===============================================
	void SetMusic(LPCSTR lpcFileName);
	void Play(int Mlimit = 0);	// ミリ秒単位での再生時間を設定
	void Stop();

	//===============================================
	//	アクセサ
	//===============================================
	int GetMSecond();	// ミリ秒
	void SetShift(short val){ m_sShift = val; }
	void SetOffset(DWORD val) { m_dwOffset = val; }
	void SetSpeed(float speed){ m_pSoundPlayer->SetSpeed(0, speed); }
	bool isEnd();

private:
	//===============================================
	//	メンバ変数
	//===============================================
	tdnSoundBGM *m_pSoundPlayer;	// 曲流す用
	LPCSTR *m_lpcFileName;			// 曲のファイル名
	int m_iDelta;					// デルタタイム
	int m_iStart;					// 計測開始時間
	int m_iMSecond;					// 経過時間<秒/1000>
	int m_iLimit;					// 設定時間
	short m_sShift;					// 補正時間
	DWORD m_dwOffset;				// 再生開始時間(エディット用)
	bool m_bSoundPlay;				// サウンド再生フラグ
};