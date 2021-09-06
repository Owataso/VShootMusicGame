#pragma once

#include "TDNLIB.h"

/********************************************/
//	�萔��\���̂Ȃǂ̏��̑q��
/********************************************/


/********************************************/
//	�萔
/********************************************/
static const int MAX_MUSIC = 127;	// �Ȃ̍ő吔
static const int MAX_OCTAVE = 4;
static const int JUDGE_LINE = 618;	// ����Y���W
static const int NUM_KEYBOARD = 12 * MAX_OCTAVE;
static const float HI_SPEED_WIDTH = .5f;			// �n�C�X�s�̍��ݕ�
static const int NOTE_WIDTH = 40;					// �m�[�c�̕�
static const int KEYLANE_LEFT = 0;					// �L�[���C����
static const int KEYLANE_TOP = 116;					// �L�[���C����
static const int KEYLANE_HEIGHT = 497;				// �����Ă����̏㉺��
static const int MAX_SCORE = 100000;				// �X�R�A�̍ő吔
static const int NUM_MUSIC_TYPE = 4;				// �u�P�ȁv�ɑ΂���g�p����BGM�����f�[�^�ő�l
static LPCSTR c_lpcPROJECT_OGG_NAME = "/Music1.ogg";// �v���W�F�N�g�œ��ꂷ��OGG�t�@�C����
static LPCSTR c_lpcPROJECT_OMS_NAME = "/Chart.oms";	// �v���W�F�N�g�œ��ꂷ��OMS�t�@�C����
static LPCSTR c_lpcPROJECT_OMT_NAME = "/Chart.omt";	// �v���W�F�N�g�œ��ꂷ��OMS�t�@�C����
static const int END_OF_NOTE = -1;					// OMT�̃m�[�c�I���R�[�h
//static const int EDIT_NO_SELECT_NOTE = 65535;		// �I��łȂ�
static const int EDIT_VR_NOTE_SIZE = 128;
static const int MS_APPEAR_NOTE_TIME = 1000;

static const int NUM_CELL_X = 32;
static const int NUM_CELL_Y = 18;
static const int NUM_SHOW_NOTE = 8;

enum class OMS_RESULT
{
	OK,					// ����
	FAILED_OPEN,		// �t�@�C���I�[�v�����s
	FAILED_READ_MIDI,	// MIDI����ɓǂݍ��܂�Ȃ�����
	FAILED_READ_OMS		// OMS����ɓǂݍ��܂�Ȃ�����
};

/********************************************/
//	�m�[�c���W�v�Z(��)
/********************************************/
#define NOTE_POS_COMPTE(key) (key*(1280 / 128))

//------------------------------------------------------
//	�X�R�A
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
	// F�͂���ȉ�
};
SCORE_RANK ScoreRankCheck(int score);// �����̃X�R�A����A���̃X�R�A�̃����N��f���o��


//------------------------------------------------------
//	��Փx
//------------------------------------------------------
enum class DIFFICULTY
{
	NORMAL,		// �܂�������
	HYPER,		// ���\������������
	EXPERT,		// �߂�����ނ���
	VIRTUOSO,	// ����Z�I(�N���A�����痧�h�ȃs�A�j�X�g�ł�)
	MAX
};
static const DWORD DifficultyColor[(int)DIFFICULTY::MAX] =
{
	0xff10ff10,	// ��
	0xffffff10,	// ��
	0xffff1010,	// ��
	0xffca10ca	// ��
};

//------------------------------------------------------
//	�N���A�����v
//------------------------------------------------------
enum class CLEAR_LAMP
{
	NO_PLAY,	// ���v���C
	FAILED,		// ���N���A
	EASY,		// �C�[�W�[
	CLEAR,		// �m�}�Q
	HARD,		// ��
	EX_HARD,	// EX��
	FULL_COMBO,	// �t����
	MAX
};


//------------------------------------------------------
//	�Q�[�W�I�v�V����
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
//	����
//------------------------------------------------------
enum class JUDGE_TYPE
{
	SUPER,	// �ō�
	GREAT,	// �ǂ�
	GOOD,	// �ӂ�
	BAD,	// ����
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
//	�W������
//------------------------------------------------------
enum class MUSIC_GENRE
{
	ORIGINAL,	// �I���W�i��
	GAME,		// �Q�[�����y
	CLASSIC,	// �N���V�b�N
	VOCALOID,	// �{�J��
	POPS,		// �A�j���Ƃ��Ō��̗�
	OTHER,		// ���̑�
	MAX
};


//------------------------------------------------------
//	�m�[�c
//------------------------------------------------------
enum class NOTE_TYPE
{
	NORMAL		= 1 << 0,		// CN�Ȃ�(�ʏ�m�[�c)
	CN_START	= 1 << 1,	// CN�J�n
	CN_END		= 1 << 2,	// CN�I��
	CHORDS		= 1 << 3,	// ��������
};

struct NoteDataEP
{
	int iEventTime;	// �C�x���g����
	WORD wSoflanNo;		// ���������Ԗڂ̃\�t�����̔ԍ���(�\�t�����ˑ��̍��W�ɂ���Ƃ��ɕK�v)
	BYTE byNoteType;	// �m�[�c�^�C�v���i�[(���enum���i�[����)	// char�^�ɂ����̂́A�O�̎g���Ă����(tone)���g���܂킹�邩��Ƃ����������
	BYTE byLaneNo;		// ���[���ԍ�(�v��MIDI�őł����񂾃h���~�t�@�\���V�h�̃f�[�^)
	NoteDataEP() :iEventTime(0), wSoflanNo(0), byNoteType(0), byLaneNo(0) {}
};
struct NoteDataV2
{
	int iEventTime;	// �C�x���g����
	char cNoteType;		// �m�[�c�^�C�v���i�[(���enum���i�[����)	// char�^�ɂ����̂́A�O�̎g���Ă����(tone)���g���܂킹�邩��Ƃ����������

	WORD wKeySoundID;	// �L�[����ID
	int iPosX;			// �X�}�z��z�肵��X���W
	int iPosY;			// �X�}�z��z�肵��Y���W
};
struct NoteDataVRShoot
{
	int iEventTime;	// �C�x���g����
	WORD wSoflanNo;		// ���������Ԗڂ̃\�t�����̔ԍ���(�\�t�����ˑ��̍��W�ɂ���Ƃ��ɕK�v)
	BYTE byNoteType;	// �m�[�c�^�C�v���i�[(���enum���i�[����)	// char�^�ɂ����̂́A�O�̎g���Ă����(tone)���g���܂킹�邩��Ƃ����������
	float fDegree;		// ��������̊p�x 0 �` 360(���ʂ�0)
	float fRateY;		// Y���W(0�`1)
	WORD wCNEndIndex;	// CN�I���̃m�[�c(EP���ƃ��[���Ŕ��f���Ă������A���̃Q�[�����Ƃ��ꂪ���邱�ƂɂȂ肻��)
	//WORD wKeySoundID;	// �L�[����ID

	Vector2 GetScreenPosition(int iMaxDegree);
	Vector2 GetCenterScreenPosition(int iMaxDegree);
	//void SetScreenPosition(Vector2 *position);
	void SetScreenPosition(Vector2 *position, int iMaxDegree);
	Vector3 GetPosition();
};

// ���x�ω�
struct SoflanData
{
	int iEventTime;			// �C�x���g����
	float fBPM;				// BPM�l(�����_��)
	float fTick;			// tick�l(60 / BPM / ���\�l * 1000)
};

// ���q
struct BeatData
{
	int iEventTime;		// �C�x���g����
	BYTE byNumerator;	// ���q
	BYTE byDenominator;	// ����
};

// �t�H���_�[�̏����(�G�f�B�b�g�p�ƒʏ�p�̕ʂŕ�����)
class FolderBase
{
public:
	std::string sMusicName;				// �Ȗ�(�t�H���_�[�̖��O�ł�����)
	int level[(int)DIFFICULTY::MAX];	// ���x��

	FolderBase() :sMusicName(""){}
	virtual ~FolderBase(){}
};


class FolderInfo : public FolderBase
{
public:
	bool bNameEnglish;								// �Ȗ����p�ꂩ�ǂ���
	bool bMovieExist;								// ���悪���邩�ǂ���
	int iOctaveWidths[(int)DIFFICULTY::MAX];			// �I�N�^�[�u��
	std::string sSortMusicName;						// �\�[�g�p�̃W��������
	float fMinBPM, fMaxBPM;	// �I�ȏ���BPM100~200�Ƃ��̃\�t�����Ŏg��

	BYTE byID;				// ID(0�`255)
	std::string sArtistName;	// ��ȎҖ�
	MUSIC_GENRE eMusicGenre;	// �W������

	FolderInfo() :FolderBase(), sArtistName(""), bNameEnglish(true){}
};

// �G�f�B�b�g�p�̃t�H���_�[�f�[�^
class EditFolderInfo : public FolderBase
{
public:
	bool bMusicExistance[(int)DIFFICULTY::MAX];			// �Ȃ������Ă邩(��{�I��0�Ԃ����̗��p�ƂȂ邪�AAnother�ŋȕς��Ƃ���肽���̂œ�Փx�Ԃ�ێ�����)�����{���Ɋ�{�I�ɂ�0�ɂ���true������
	bool bAudition;										// �����p��wav�f�[�^�������Ă邩
	bool bDifficultyExistance[(int)DIFFICULTY::MAX];	// ��Փx�����݂��Ă��邩

	EditFolderInfo(){
		assert(0);// �f�t�H���g�R���X�g���N�^�͌Ăяo���Ȃ���
	}
	EditFolderInfo(LPCSTR FileName);
};

enum class OMS_TYPE
{
	NO_LOAD = -1,	// ���[�h���ĂȂ�
	BASE,			// �ǂ̕��ʂɂł��ł���悤�ɔėp����������������
	VR_SHOOT,		// VR�V���[�e�B���O�p
	MAX
};

//// �t�@�C���ǂݏ����p���킽�����ʍ\����(�r�b�g�t�B�[���h)
//struct OMSInfo
//{
//	//unsigned int level		 : 5;	// �Ȃ̃��x�� 5�r�b�g(31)
//	unsigned int PlayMusicNo : 3;	// �t�H���_�̒��̂ǂ̔ԍ��̋Ȃ��Đ����邩(1�`4�̒l�A0�Ȃ�ȂȂ�) 3�r�b�g(7)
//	unsigned int PlayTime	 : 10;	// �Đ�����(�b�P�ʁA�Ȃ�����ꍇ�s�v) 10�r�b�g(1023) 17��5�b
//	unsigned int NumNotes	 : 14;	// �m�[�g�� 14�r�b�g(16383)
//	unsigned int NumSoflans	 : 10;	// �\�t������ 10�r�b�g(1023)
//	signed int shift		 : 16;	// �Ȃƕ��ʂ̍��𒲐�����p ��������16�r�b�g(-32768 �` 32767)
//	unsigned int division	 : 16;	// ���\�l 16�r�b�g(65535)
//	unsigned int BaseOctave	 : 7;	// �J�n�I�N�^�[�u 7�r�b�g(127)
//};
//
//struct OMSInfoV2
//{
//	unsigned int PlayMusicNo : 3;	// �t�H���_�̒��̂ǂ̔ԍ��̋Ȃ��Đ����邩(1�`4�̒l�A0�Ȃ�ȂȂ�) 3�r�b�g(7)
//	unsigned int PlayTime : 10;	// �Đ�����(�b�P�ʁA�Ȃ�����ꍇ�s�v) 10�r�b�g(1023) 17��5�b
//	unsigned int NumNotes : 14;	// �m�[�g�� 14�r�b�g(16383)
//	unsigned int NumSoflans : 10;	// �\�t������ 10�r�b�g(1023)
//	signed int shift : 16;	// �Ȃƕ��ʂ̍��𒲐�����p ��������16�r�b�g(-32768 �` 32767)
//	unsigned int division : 16;	// ���\�l 16�r�b�g(65535)
//	unsigned int BaseOctave : 7;	// �J�n�I�N�^�[�u 7�r�b�g(127)
//	unsigned int OctaveWidth : 3;	// �I�N�^�[�u��(1�`4) 3�r�b�g(7)
//	unsigned int Other1 : 16;		// ����̒ǉ���z�肵���󂫘g
//	unsigned int Other2 : 16;
//	unsigned int Other3 : 16;
//};

struct BaseOMSInfo
{
	BYTE byPlayMusicNo;		// �t�H���_�̒��̂ǂ̔ԍ��̋Ȃ��Đ����邩
	WORD wNumNotes;			// �m�[�c��
	WORD wNumSoflans;		// �\�t������
	WORD wNumBeats;			// ���q�ύX��
	short sDivision;		// ���\�l
	short sShift;			// shift�l
};

enum class EP_NOTE_DATA_TYPE
{
	EVENT_TIME,	// �C�x���g����
	NOTE,		// �m�[�c�^�C�v
	LANE,		// ���[���ԍ�
};

enum class VRSHOOT_NOTE_DATA_TYPE
{
	EVENT_TIME,		// �C�x���g����
	NOTE,			// �m�[�c�^�C�v
	DEGREE,			// ���S����̊p�x(����x���W)
	RATE_Y,			// Y���W(0�`1)
	CN_END_INDEX,	// CN��̃m�[�c�̃C���f�b�N�X
};

// �Ȃ̏��
class BaseMusicInfo
{
public:
	//DIFFICULTY eDifficulty;	// ��Փx
	//BYTE byID;				// ID(0�`255)�I�ȉ�ʂ���ړ����ɒl��ێ���������������
	BaseOMSInfo OMSInfo;		// OMS���

	SoflanData *SoflanDatas;	// BPM�ω��C�x���g(�ϒ�)
	BeatData *BeatDatas;		// ���q�ύX�C�x���g(�ϒ�)

	BaseMusicInfo() :OMSInfo(BaseOMSInfo{}), SoflanDatas(nullptr), BeatDatas(nullptr) {}
	virtual void Release()
	{
		// ��񏉊���
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
	NoteDataEP *NoteDatas;	// �m�[�c�C�x���g(�ϒ�)

	MusicInfoEP() :BaseMusicInfo(), NoteDatas(nullptr){}
	void Release()override
	{
		//tdnDebug::OutPutLog("�L�e���O�}\n");
		if (NoteDatas)
		{
			delete[] NoteDatas;
			NoteDatas = nullptr;
		}
		//tdnDebug::OutPutLog("�L�e���O�}\n");
		BaseMusicInfo::Release();
		//tdnDebug::OutPutLog("�L�e���O�}\n");
	}

	std::string GetNoteData(int id, EP_NOTE_DATA_TYPE type);
	void SetNoteData(LPCSTR value, int id, EP_NOTE_DATA_TYPE type);
};

class MusicInfoVRShoot : public BaseMusicInfo
{
public:
	NoteDataVRShoot *NoteDatas;	// �m�[�c�C�x���g(�ϒ�)
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