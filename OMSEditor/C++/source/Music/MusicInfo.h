#pragma once

#include "TDNLIB.h"

/********************************************/
//	定数や構造体などの情報の倉庫
/********************************************/


/********************************************/
//	定数
/********************************************/
static const int MAX_MUSIC = 127;	// 曲の最大数
static const int MAX_OCTAVE = 4;
static const int JUDGE_LINE = 618;	// 判定Y座標
static const int NUM_KEYBOARD = 12 * MAX_OCTAVE;
static const float HI_SPEED_WIDTH = .5f;			// ハイスピの刻み幅
static const int NOTE_WIDTH = 40;					// ノーツの幅
static const int KEYLANE_LEFT = 0;					// キーライン左
static const int KEYLANE_TOP = 116;					// キーライン上
static const int KEYLANE_HEIGHT = 497;				// 落ちてくるやつの上下幅
static const int MAX_SCORE = 100000;				// スコアの最大数
static const int NUM_MUSIC_TYPE = 4;				// 「１曲」に対する使用するBGM音源データ最大値
static LPCSTR c_lpcPROJECT_OGG_NAME = "/Music1.ogg";// プロジェクトで統一するOGGファイル名
static LPCSTR c_lpcPROJECT_OMS_NAME = "/Chart.oms";	// プロジェクトで統一するOMSファイル名
static LPCSTR c_lpcPROJECT_OMT_NAME = "/Chart.omt";	// プロジェクトで統一するOMSファイル名
static const int END_OF_NOTE = -1;					// OMTのノーツ終了コード
//static const int EDIT_NO_SELECT_NOTE = 65535;		// 選んでない
static const int EDIT_VR_NOTE_SIZE = 128;
static const int MS_APPEAR_NOTE_TIME = 1000;

static const int NUM_CELL_X = 32;
static const int NUM_CELL_Y = 18;
static const int NUM_SHOW_NOTE = 8;

enum class OMS_RESULT
{
	OK,					// 成功
	FAILED_OPEN,		// ファイルオープン失敗
	FAILED_READ_MIDI,	// MIDI正常に読み込まれなかった
	FAILED_READ_OMS		// OMS正常に読み込まれなかった
};

/********************************************/
//	ノーツ座標計算(横)
/********************************************/
#define NOTE_POS_COMPTE(key) (key*(1280 / 128))

//------------------------------------------------------
//	スコア
//------------------------------------------------------
enum class SCORE_RANK
{
	AAA, AA, A, B, C, D, E, F, MAX
};
static const int SCORE_BORDER[(int)SCORE_RANK::MAX] =
{
	90000,	// AAA
	80000,	// AA
	70000,	// A
	60000,	// B
	50000,	// C
	40000,	// D
	30000,	// E
	// Fはそれ以下
};
SCORE_RANK ScoreRankCheck(int score);// 引数のスコアから、そのスコアのランクを吐き出す


//------------------------------------------------------
//	難易度
//------------------------------------------------------
enum class DIFFICULTY
{
	NORMAL,		// まだいける
	HYPER,		// 結構歯ごたえある
	EXPERT,		// めっちゃむずい
	VIRTUOSO,	// 超絶技巧(クリアしたら立派なピアニストです)
	MAX
};
static const DWORD DifficultyColor[(int)DIFFICULTY::MAX] =
{
	0xff10ff10,	// 緑
	0xffffff10,	// 黄
	0xffff1010,	// 赤
	0xffca10ca	// 紫
};

//------------------------------------------------------
//	クリアランプ
//------------------------------------------------------
enum class CLEAR_LAMP
{
	NO_PLAY,	// 未プレイ
	FAILED,		// 未クリア
	EASY,		// イージー
	CLEAR,		// ノマゲ
	HARD,		// 難
	EX_HARD,	// EX難
	FULL_COMBO,	// フル婚
	MAX
};


//------------------------------------------------------
//	ゲージオプション
//------------------------------------------------------
enum class GAUGE_OPTION
{
	NORMAL,
	EASY,
	HARD,
	EX_HARD,
	HAZARD,
	GRADE,
	MAX
};

//------------------------------------------------------
//	判定
//------------------------------------------------------
enum class JUDGE_TYPE
{
	SUPER,	// 最高
	GREAT,	// 良い
	GOOD,	// ふつう
	BAD,	// 悪い
	MAX
};
enum class JUDGE_TIMING
{
	JUST,
	FAST,
	SLOW,
	MAX
};


//------------------------------------------------------
//	ジャンル
//------------------------------------------------------
enum class MUSIC_GENRE
{
	ORIGINAL,	// オリジナル
	GAME,		// ゲーム音楽
	CLASSIC,	// クラシック
	VOCALOID,	// ボカロ
	POPS,		// アニメとか版権の類
	OTHER,		// その他
	MAX
};


//------------------------------------------------------
//	ノーツ
//------------------------------------------------------
enum class NOTE_TYPE
{
	NORMAL		= 1 << 0,		// CNなし(通常ノーツ)
	CN_START	= 1 << 1,	// CN開始
	CN_END		= 1 << 2,	// CN終了
	CHORDS		= 1 << 3,	// 同時押し
};

struct NoteDataEP
{
	int iEventTime;	// イベント時間
	WORD wSoflanNo;		// 自分が何番目のソフランの番号か(ソフラン依存の座標にするときに必要)
	BYTE byNoteType;	// ノーツタイプを格納(上のenumを格納する)	// char型にしたのは、前の使ってた情報(tone)を使いまわせるからという無理やり
	BYTE byLaneNo;		// レーン番号(要はMIDIで打ち込んだドレミファソラシドのデータ)
	NoteDataEP() :iEventTime(0), wSoflanNo(0), byNoteType(0), byLaneNo(0) {}
};
struct NoteDataV2
{
	int iEventTime;	// イベント時間
	char cNoteType;		// ノーツタイプを格納(上のenumを格納する)	// char型にしたのは、前の使ってた情報(tone)を使いまわせるからという無理やり

	WORD wKeySoundID;	// キー音のID
	int iPosX;			// スマホを想定したX座標
	int iPosY;			// スマホを想定したY座標
};
struct NoteDataVRShoot
{
	int iEventTime;	// イベント時間
	WORD wSoflanNo;		// 自分が何番目のソフランの番号か(ソフラン依存の座標にするときに必要)
	BYTE byNoteType;	// ノーツタイプを格納(上のenumを格納する)	// char型にしたのは、前の使ってた情報(tone)を使いまわせるからという無理やり
	float fDegree;		// 中央からの角度 0 ～ 360(正面が0)
	float fRateY;		// Y座標(0～1)
	WORD wCNEndIndex;	// CN終了のノーツ(EPだとレーンで判断していたが、このゲームだとこれがいることになりそう)
	//WORD wKeySoundID;	// キー音のID

	Vector2 GetScreenPosition(int iMaxDegree);
	Vector2 GetCenterScreenPosition(int iMaxDegree);
	//void SetScreenPosition(Vector2 *position);
	void SetScreenPosition(Vector2 *position, int iMaxDegree);
	Vector3 GetPosition();
};

// 速度変化
struct SoflanData
{
	int iEventTime;			// イベント時間
	float fBPM;				// BPM値(小数点込)
	float fTick;			// tick値(60 / BPM / 分能値 * 1000)
};

// 拍子
struct BeatData
{
	int iEventTime;		// イベント時間
	BYTE byNumerator;	// 分子
	BYTE byDenominator;	// 分母
};

// フォルダーの情報基底(エディット用と通常用の別で分ける)
class FolderBase
{
public:
	std::string sMusicName;				// 曲名(フォルダーの名前でもある)
	int level[(int)DIFFICULTY::MAX];	// レベル

	FolderBase() :sMusicName(""){}
	virtual ~FolderBase(){}
};


class FolderInfo : public FolderBase
{
public:
	bool bNameEnglish;								// 曲名が英語かどうか
	bool bMovieExist;								// 動画があるかどうか
	int iOctaveWidths[(int)DIFFICULTY::MAX];			// オクターブ幅
	std::string sSortMusicName;						// ソート用のジャンル名
	float fMinBPM, fMaxBPM;	// 選曲情報でBPM100~200とかのソフランで使う

	BYTE byID;				// ID(0～255)
	std::string sArtistName;	// 作曲者名
	MUSIC_GENRE eMusicGenre;	// ジャンル

	FolderInfo() :FolderBase(), sArtistName(""), bNameEnglish(true){}
};

// エディット用のフォルダーデータ
class EditFolderInfo : public FolderBase
{
public:
	bool bMusicExistance[(int)DIFFICULTY::MAX];			// 曲が入ってるか(基本的に0番だけの利用となるが、Anotherで曲変わるとかやりたいので難易度ぶん保持する)ただ本当に基本的には0にだけtrueを入れる
	bool bAudition;										// 試聴用のwavデータが入ってるか
	bool bDifficultyExistance[(int)DIFFICULTY::MAX];	// 難易度が存在しているか

	EditFolderInfo(){
		assert(0);// デフォルトコンストラクタは呼び出さないで
	}
	EditFolderInfo(LPCSTR FileName);
};

enum class OMS_TYPE
{
	NO_LOAD = -1,	// ロードしてない
	BASE,			// どの譜面にでもできるように汎用性を持たせたもの
	VR_SHOOT,		// VRシューティング用
	MAX
};

//// ファイル読み書き用おわたそ譜面構造体(ビットフィールド)
//struct OMSInfo
//{
//	//unsigned int level		 : 5;	// 曲のレベル 5ビット(31)
//	unsigned int PlayMusicNo : 3;	// フォルダの中のどの番号の曲を再生するか(1～4の値、0なら曲なし) 3ビット(7)
//	unsigned int PlayTime	 : 10;	// 再生時間(秒単位、曲がある場合不要) 10ビット(1023) 17分5秒
//	unsigned int NumNotes	 : 14;	// ノート数 14ビット(16383)
//	unsigned int NumSoflans	 : 10;	// ソフラン回数 10ビット(1023)
//	signed int shift		 : 16;	// 曲と譜面の差を調整する用 符号あり16ビット(-32768 ～ 32767)
//	unsigned int division	 : 16;	// 分能値 16ビット(65535)
//	unsigned int BaseOctave	 : 7;	// 開始オクターブ 7ビット(127)
//};
//
//struct OMSInfoV2
//{
//	unsigned int PlayMusicNo : 3;	// フォルダの中のどの番号の曲を再生するか(1～4の値、0なら曲なし) 3ビット(7)
//	unsigned int PlayTime : 10;	// 再生時間(秒単位、曲がある場合不要) 10ビット(1023) 17分5秒
//	unsigned int NumNotes : 14;	// ノート数 14ビット(16383)
//	unsigned int NumSoflans : 10;	// ソフラン回数 10ビット(1023)
//	signed int shift : 16;	// 曲と譜面の差を調整する用 符号あり16ビット(-32768 ～ 32767)
//	unsigned int division : 16;	// 分能値 16ビット(65535)
//	unsigned int BaseOctave : 7;	// 開始オクターブ 7ビット(127)
//	unsigned int OctaveWidth : 3;	// オクターブ幅(1～4) 3ビット(7)
//	unsigned int Other1 : 16;		// 今後の追加を想定した空き枠
//	unsigned int Other2 : 16;
//	unsigned int Other3 : 16;
//};

struct BaseOMSInfo
{
	BYTE byPlayMusicNo;		// フォルダの中のどの番号の曲を再生するか
	WORD wNumNotes;			// ノーツ数
	WORD wNumSoflans;		// ソフラン回数
	WORD wNumBeats;			// 拍子変更回数
	short sDivision;		// 分能値
	short sShift;			// shift値
};

enum class EP_NOTE_DATA_TYPE
{
	EVENT_TIME,	// イベント時間
	NOTE,		// ノーツタイプ
	LANE,		// レーン番号
};

enum class VRSHOOT_NOTE_DATA_TYPE
{
	EVENT_TIME,		// イベント時間
	NOTE,			// ノーツタイプ
	DEGREE,			// 中心からの角度(実質x座標)
	RATE_Y,			// Y座標(0～1)
	CN_END_INDEX,	// CN先のノーツのインデックス
};

// 曲の情報
class BaseMusicInfo
{
public:
	//DIFFICULTY eDifficulty;	// 難易度
	//BYTE byID;				// ID(0～255)選曲画面から移動時に値を保持したかったから
	BaseOMSInfo OMSInfo;		// OMS情報

	SoflanData *SoflanDatas;	// BPM変化イベント(可変長)
	BeatData *BeatDatas;		// 拍子変更イベント(可変長)

	BaseMusicInfo() :OMSInfo(BaseOMSInfo{}), SoflanDatas(nullptr), BeatDatas(nullptr) {}
	virtual void Release()
	{
		// 情報初期化
		ZeroMemory(&OMSInfo, sizeof(BaseOMSInfo));

		if (SoflanDatas)
		{
			delete[] SoflanDatas;
			SoflanDatas = nullptr;
		}
		if (BeatDatas)
		{
			delete[] BeatDatas;
			BeatDatas = nullptr;
		}
	}

	//virtual std::string GetNoteData(int id, int no) = 0;
	//virtual void SetNoteData(LPCSTR value, int id, int no) = 0;
};

class MusicInfoEP : public BaseMusicInfo
{
public:
	NoteDataEP *NoteDatas;	// ノーツイベント(可変長)

	MusicInfoEP() :BaseMusicInfo(), NoteDatas(nullptr){}
	void Release()override
	{
		//tdnDebug::OutPutLog("キテルグマ\n");
		if (NoteDatas)
		{
			delete[] NoteDatas;
			NoteDatas = nullptr;
		}
		//tdnDebug::OutPutLog("キテルグマ\n");
		BaseMusicInfo::Release();
		//tdnDebug::OutPutLog("キテルグマ\n");
	}

	std::string GetNoteData(int id, EP_NOTE_DATA_TYPE type);
	void SetNoteData(LPCSTR value, int id, EP_NOTE_DATA_TYPE type);
};

class MusicInfoVRShoot : public BaseMusicInfo
{
public:
	NoteDataVRShoot *NoteDatas;	// ノーツイベント(可変長)
	int iMaxDegree;

	MusicInfoVRShoot() :BaseMusicInfo(), NoteDatas(nullptr), iMaxDegree(360){}
	void Release()override
	{
		if (NoteDatas)
		{
			delete[] NoteDatas;
			NoteDatas = nullptr;
		}
		BaseMusicInfo::Release();
	}

	std::string GetNoteData(int id, VRSHOOT_NOTE_DATA_TYPE type);
	void SetNoteData(LPCSTR value, int id, VRSHOOT_NOTE_DATA_TYPE type);
};