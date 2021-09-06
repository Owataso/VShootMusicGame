#pragma once


//===============================================
//	�J�X�^�}�C�Y�̒萔
//===============================================
enum class CUSTOMIZE_TYPE
{
	SKIN,
	NOTE,
	EXPLOSION,
	SUDDEN,
	VOICE,
	SELECT_MUSIC,
	MAX
};

/********************************************/
//	�J�X�^�}�C�Y�Ǘ��N���X<Singleton>
/********************************************/
class CustomizeDataManager
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static CustomizeDataManager *GetInstance(){ static CustomizeDataManager instance; return &instance; }


	//===============================================
	//	�X�L���f�[�^�̎擾
	//===============================================
	std::string GetSkin(CUSTOMIZE_TYPE type, int no);	// �����ԍ��̃X�L���擾
	std::string GetSetedSkin(CUSTOMIZE_TYPE type);		// �Z�b�g���ꂽ�X�L�����擾
	std::string GetSkinName(CUSTOMIZE_TYPE type, int no);
	UINT GetNumSkin(CUSTOMIZE_TYPE type){ return m_vDataList[(int)type].size(); }

	//===============================================
	//	�f�X�g���N�^
	//===============================================
	~CustomizeDataManager();

private:
	//===============================================
	//	�J�X�^�}�C�Y�̃f�[�^
	//===============================================
	struct CustomizeData
	{
		char path[64];		// �t�@�C����
		char ID[64];		// ���O
	};


	//===============================================
	//	�J�X�^�}�C�Y�f�[�^�̃��X�g
	//===============================================
	std::vector<CustomizeData*> m_vDataList[(int)CUSTOMIZE_TYPE::MAX];


	//===============================================
	//	�f�[�^�̃��[�h
	//===============================================
	void LoadSettingData();
	void LoadSkin(CUSTOMIZE_TYPE eSkin);


	//===============================================
	//	�V���O���g���̍�@
	//===============================================
	CustomizeDataManager();
	CustomizeDataManager(const CustomizeDataManager&){}
	CustomizeDataManager &operator =(const CustomizeDataManager&);
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define CustomizeDataMgr CustomizeDataManager::GetInstance()