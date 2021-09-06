#pragma once

#include "../Music/MusicInfo.h"
enum class DIFFICULTY;

//===============================================
//	�I�ȃf�[�^
//===============================================
struct SelectMusicData
{
	int id;					// �Ȃ�ID
	DIFFICULTY difficulty;	// �Ȃ̓�Փx
	SelectMusicData() :id(0){}
	SelectMusicData(int ID, DIFFICULTY difficulty) :id(ID), difficulty(difficulty){}
};


/********************************************/
//	scene�ׂ��ł̑I�ȃf�[�^�ێ��p<Singleton>
/********************************************/
class SelectMusicManager
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static SelectMusicManager *GetInstance(){ static SelectMusicManager instance; return &instance; }

	//===============================================
	//	�f�X�g���N�^
	//===============================================
	~SelectMusicManager(){ Clear(); }


	//===============================================
	//	�I�ȏ��̐ݒ�
	//===============================================
	void Set(MusicInfo *info);		// �G�f�B�b�g���[�h�p
	bool Set(const SelectMusicData &data);	// ����ȊO


	//===============================================
	//	�I�ȏ��̎擾
	//===============================================
	MusicInfo *Get(){ return &m_tagSelectMusicInfo; }
	std::string GetMusicPath()
	{
		char path[MAX_PATH];
		sprintf_s(path, MAX_PATH, "DATA/Musics/EP/No_%d/", m_tagSelectMusicInfo.byID);
		return std::string(path);
	}

private:
	//===============================================
	//	�����o�֐�
	//===============================================
	void Clear();	// �o�b�t�@�̊J��

	//===============================================
	//	�����o�֐�
	//===============================================
	MusicInfo m_tagSelectMusicInfo;	// �X�R�A�f�[�^�̎���

	//===============================================
	//	�V���O���g���̍�@
	//===============================================
	SelectMusicManager() : m_tagSelectMusicInfo(MusicInfo{}){}
	SelectMusicManager(const SelectMusicManager&){}
	SelectMusicManager &operator =(const SelectMusicManager&){}
};


/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define SelectMusicMgr (SelectMusicManager::GetInstance())
