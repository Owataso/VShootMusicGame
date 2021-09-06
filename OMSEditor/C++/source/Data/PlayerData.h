#pragma once

struct MusicRecord;
enum class GAUGE_OPTION;


//===============================================
//	�i�ʂ̒萔
//===============================================
enum class GRADE
{
	NONE = -1,		// �����Ȃ����
	FIRST,		// ���i
	SECOND,		// ��i
	THERD,		// �O�i
	FOURTH,		// �l�i
	FIFTH,		// �ܒi
	SIXTH,		// �Z�i
	SEVENTH,	// ���i
	EIGHTH,		// ���i
	NINTH,		// ��i
	TENTH,		// �\�i
	BRILLIANCE,	// �F�`
	MAX
};

//===============================================
//	�X�L���f�[�^�̍ő吔
//===============================================
static const int SKINDATA_MAX = 63;


/********************************************/
//	�v���C���[�Ɋւ�����̍\����
/********************************************/
namespace PlayerData
{
	//===============================================
	//	�v���C���
	//===============================================
	struct Infomation
	{
		char PlayerName[11];		// �v���C���[�̖��O(10����) string�^�ɂ��Ȃ��̂́A�f�[�^�����o���ɂ���ČŒ蒷�ɂ��邽�߁B
		int PlayCount;				// �V�񂾉�
		//DWORD id;					// �l�b�g���[�N�Ńf�[�^�Ǘ�������Ƒz�肵���ꍇ�̃��[�U�[ID
		DWORD PlayTime;				// �v���C����
	};

	//===============================================
	//	�Ȃ̃X�R�A�L�^
	//===============================================
	struct MusicRecord
	{
		unsigned int ClearLamp : 4;	// �N���A�����v(����7��ނ�����A4�r�b�g(15)����΂������Ƃ��������ȍl��)
		unsigned int HiScore : 17;	// �n�C�X�R�A(�ō�100000�Ȃ̂ŁA17�r�b�g(131071)
	};

	//===============================================
	//	�i�ʏ��
	//===============================================
	struct GradeRecord
	{
		unsigned int bPass : 1;				// ���i(0or1)(������͒B�����Ŕ���Ƃ�)
		unsigned int AchievementPct : 7;	// �B����(0�`100�Ȃ̂ŁA7�r�b�g(127))
	};

	//===============================================
	//	�i�ʂ̃f�[�^1�g�]�����̂ŁA�ė��p
	//===============================================
	struct Other
	{
		unsigned int version : 2;			// �f�[�^�̃o�[�W���� 3�r�b�g(7)
		unsigned int empty : 6;				// �󂫘g
	};


	//===============================================
	//	�v���C���[�̃I�v�V����
	//===============================================
	// 
	//struct Option
	//{
	//	unsigned int bSudden		:1;		// �T�h�D��
	//	unsigned int SuddenPos		:10;	// �T�h�D�����W(Y)10�r�b�g(1023)
	//	unsigned int HiSpeed		:5;		// �n�C�X�s 5�r�b�g(31) * 0.25
	//	unsigned int iGaugeOption	:3;		// �Q�[�W(enum���Ə����n�ˑ��Ȃ̂Ŋ댯) 3�r�b�g(7)
	//	//unsigned int BasisOctave	:7;		// �J�n�I�N�^�[�u 7�r�b�g(127) �����\�p�ɂɕς�肻���Ȃ̂ŁA�e�L�X�g�ŊǗ�����
	//	unsigned int bMoviePlay		:1;		// ���t���ɓ�����Đ����邩�ǂ���
	//	unsigned int MovieAlpha		:8;		// ���t�w�i�ŗ�������̓����x(255)
	//	unsigned int bAutoPlay		:1;		// DJ_AUTO����
	//	unsigned int bGuide			:1;		// �K�C�h���肩�Ȃ���
	//};
	struct OptionV2
	{
		unsigned int bSudden		:1;		// �T�h�D��
		unsigned int SuddenPos		:10;	// �T�h�D�����W(Y)10�r�b�g(1023)
		unsigned int HiSpeed		:5;		// �n�C�X�s 5�r�b�g(31) * 0.25
		unsigned int iGaugeOption	:3;		// �Q�[�W(enum���Ə����n�ˑ��Ȃ̂Ŋ댯) 3�r�b�g(7)
		//unsigned int BasisOctave	:7;		// �J�n�I�N�^�[�u 7�r�b�g(127) �����\�p�ɂɕς�肻���Ȃ̂ŁA�e�L�X�g�ŊǗ�����
		unsigned int bMoviePlay		:1;		// ���t���ɓ�����Đ����邩�ǂ���
		unsigned int MovieAlpha		:8;		// ���t�w�i�ŗ�������̓����x(255)
		unsigned int bAutoPlay		:1;		// DJ_AUTO����
		unsigned int bGuide			:1;		// �K�C�h���肩�Ȃ���
		unsigned int bRegulSpeed	:1;		// ���M�����[�X�s�[�h(�\�t�����Ȃ�)		
		unsigned int bPushStartGuide:1;		// ���̌��Ղ���n�܂��I�̗L�薳��
		unsigned int bShowTiming	:1;		// ����^�C�~���O�\��
		unsigned int Other1 : 14;			// ����̒ǉ���z�肵���󂫘g
		unsigned int Other2 : 16;
		unsigned int Other3 : 16;
	};

	//===============================================
	//	�X�L��
	//===============================================
	struct CustomizeRecord
	{
		int no;			// �I��ł�X�L���̔ԍ�
		Mybool bUnlock;	// ����
	};

}


/********************************************/
//	�v���C���[�f�[�^�p(�V���O���g��)
/********************************************/
class PlayerDataManager
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static PlayerDataManager *GetInstance(){ static PlayerDataManager instance; return &instance; }


	//===============================================
	//	�Z�[�u�t���O
	//===============================================
	void OnSaveFlag(){ m_bSaveFlag = true; }

	//===============================================
	//	������(�Z�[�u�f�[�^�̃��[�h)
	//===============================================
	bool Initialize();

	//===============================================
	//	�����o�ϐ�(�A�N�Z�X���y�ɂ����������̂�public)
	//===============================================
	PlayerData::Infomation m_PlayerInfo;				// �v���C���
	PlayerData::Other m_OtherData;						// �i�ʂ̋󂫘g�ł��̑��̏�񃊃T�C�N��
	PlayerData::GradeRecord **m_GradeRecords;			// �i�ʏ��(�Q�[���J�n���̒i�ʂ͂�������T��)
	GRADE m_grade;										// �������擾���Ă���ő�̒i��
	Mybool *m_bUnlockMusics;							// �Ȃ̉��փf�[�^(�萔MAX_MUSIC���w�b�_�Ő錾�������Ȃ��̂ŁA�|�C���^�ϒ��ɂ���)
	PlayerData::CustomizeRecord **m_CustomizeRecords;	// �X�L���̏��(�ǂ̃X�L��)
	PlayerData::OptionV2 m_PlayerOption;				// �v���C���[�̃I�v�V����


	//===============================================
	//	�f�X�g���N�^
	//===============================================
	~PlayerDataManager();


	//===============================================
	//	�V�K�f�[�^�쐬
	//===============================================
	void NewCreate(char *PlayerName);

	//===============================================
	//	�Z�[�u
	//===============================================
	void SavePlayerData();

	//===============================================
	//	�Ȃ̋L�^�̎擾
	//===============================================
	PlayerData::MusicRecord *GetMusicRecord(int id, int iDifficulty){ return &m_MusicRecords[id][iDifficulty]; }

private:
	//===============================================
	//	�����o�֐�
	//===============================================
	bool LoadPlayerData();
	void ClearData();		// ������炠������(�f�[�^�S����)


	//===============================================
	//	�����o�ϐ�
	//===============================================
	clock_t m_StartClock;	// �v���C���ԉ��Z�p
	bool m_bSaveFlag;		// true�ɂ�����������Ƃ��ɃZ�[�u����
	PlayerData::MusicRecord	**m_MusicRecords;			// �Ȃ̃n�C�X�R�A�Ƃ��ۑ�����p�̃f�[�^(�_�u���|�C���^�����ǁAm_MusicRecords[id][difficlyt].score�Ƃ����g����������)


	PlayerDataManager();
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define PlayerDataMgr (PlayerDataManager::GetInstance())