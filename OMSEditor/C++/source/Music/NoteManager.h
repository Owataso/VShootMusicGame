#pragma once

//===============================================
//	前方宣言
//===============================================
struct NoteDataEP;
struct NoteDataVRShoot;
struct SoflanInfo;
class MusicInfoEP;
class MusicInfoVRShoot;
class SoflanManager;
class JudgeManager;


/********************************************/
//	流れてくるノーツの情報
/********************************************/
struct PlayNoteInfo
{
	//UINT *EventTimeArray;	// ★ソフランするとずれるので、ソフラン分のイベントタイムをもつ
	//std::shared_ptr<NoteDataEP> pNote;		// ノーツ実体
	NoteDataEP *pNoteEP;
	NoteDataVRShoot *pNoteVRShoot;
	int iPosition;			// 位置
	bool bErase;			// 役目を終えたか
	WORD wIndex;

	PlayNoteInfo():pNoteEP(nullptr),pNoteVRShoot(nullptr), wIndex(0){}
	~PlayNoteInfo(){}

	void SetNote(NoteDataEP *pNote)
	{
		pNoteEP = pNote;
		pNoteVRShoot = nullptr;
	}
	void SetNote(NoteDataVRShoot *pNote)
	{
		pNoteVRShoot = pNote;
		pNoteEP = nullptr;
	}
	void UpdatePos(DWORD dwCurrentPosition);
};


/********************************************/
//	ノーツ管理クラス
/********************************************/
class NoteManager
{
public:
	//===============================================
	//	コンストラクタ・デストラクタ
	//===============================================
	NoteManager();
	~NoteManager();

	//===============================================
	//	初期化・解放
	//===============================================
	void Initialize(SoflanManager *pSoflanMgr, JudgeManager *pJudgeMgr);

	//===============================================
	//	更新
	//===============================================
	void Update(DWORD dwCurrentPosition);
	void EditUpdate(DWORD dwCurrentPosition);

	//===============================================
	//	アクセサ
	//===============================================
	std::list<PlayNoteInfo*> *GetList();	// リスト取得
	UINT GetNumNorts(){ return m_iNumNorts; }
	void GetStartNotesLane(std::vector<int> *pOutLanes);
	void GetNoteAbsolutePos(const PlayNoteInfo &note, int *pos_y);	// 絶対的なノーツ座標(主に判定で使用)
	void GetNoteHiSpeedPos(const PlayNoteInfo &note, int *pos_y);	// BPMとか、ハイスピに依存したノーツの座標(主にUIで使用)
	void Set(MusicInfoEP *info);
	void Set(MusicInfoVRShoot *info, DWORD dwOffset = 0);

private:
	//===============================================
	//	メンバ変数
	//===============================================
	SoflanManager *m_pSoflanMgr;		// ソフラン管理を指し示す
	JudgeManager *m_pJudgeMgr;		// 判定管理さんの実体を指し示す
	UINT m_iNumNorts;					// ノーツ数
	//char m_cBaseOctave;					// 基本オクターブ
	//std::list<PlayNoteInfo*> *m_Lists;	// ノーツリスト
	std::list<PlayNoteInfo*> m_NoteList;	// ノーツリスト(単一)
	bool isVR;

	//===============================================
	//	メンバ関数
	//===============================================
	void Clear();	// ノーツクリア
	void SetTick(int second, float bpm);
	// 最適化前と最適化後で分けたノーツ更新
	void EPUpdate(DWORD dwCurrentPosition);
	void VRShootUpdate(DWORD dwCurrentPosition);
};