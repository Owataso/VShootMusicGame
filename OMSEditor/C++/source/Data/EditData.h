#pragma once

#include "../Music/MusicInfo.h"

enum class EDIT_STATE
{
	NO_OPEN,		// �܂������J���Ă��Ȃ����
	OMS_BASE_OPEND,	// �ėpOMS�f�[�^(MIDI�ǂݍ��񂾌�or�ėpOMS�f�[�^�ǂ݂��񂾌�)
};

/********************************************/
//	MIDI�ǂݍ��݃f�[�^�Ǘ�
/********************************************/
class EditDataManager
{
public:
	//===============================================
	//	�������E���
	//===============================================
	void Initialize();
	~EditDataManager();

	//===============================================
	//	�X�V
	//===============================================
	void EditUpdateVRShoot();

	//===============================================
	//	�ǂݍ���MIDI�t�@�C�����畈�ʂ��쐬
	//===============================================
	OMS_RESULT LoadMIDI_MakeMusicScore(LPCSTR lpcFileName);

	//===============================================
	//	�Ǝ����ʃt�@�C���̓ǂݍ��݁E�����o��
	//===============================================
	void WriteOMS(LPCSTR lpcFileName);
	void WriteBaseOMS(std::ofstream *ofs);
	void WriteVRShootOMS(std::ofstream *ofs);

	void WriteOMT(LPCSTR lpcFileName);
	void WriteBaseOMT(std::ofstream *ofs);
	void WriteVRShootOMT(std::ofstream *ofs);

	OMS_RESULT LoadOMS(LPCSTR lpcFileName);
	OMS_RESULT LoadOMT(LPCSTR lpcFileName);

	//===============================================
	//	�v���W�F�N�g�̓ǂݍ��݁E�����o��
	//===============================================
	int LoadProject(LPCSTR lpcProjectDirectory);
	int LoadProjectByOMS(LPCSTR lpcFileName);
	void SaveProject();


	//===============================================
	//	OGG�ǂݍ���(�v���W�F�N�g�t�H���_���ɃR�s�[����)
	//===============================================
	void InportOGG(LPCSTR lpcFileName);

	//===============================================
	//	���ʃ^�C�v�̕ύX
	//===============================================
	void BaseToVRShooting();

	//===============================================
	//	�m�[�c�ҏW
	//===============================================
	//void EditStart();
	//UINT SetEditNoteVRShoot(std::vector<WORD> *list);
	//UINT SetEditNoteVRShootString(LPCSTR indicesString);
	//std::vector<WORD> *GetEditNoteIndices() { return &m_SelectNoteIndices; }
	//void SetScreenPositionVRShoot(Vector2 *position);

	//===============================================
	//	���X�g�ǉ�
	//===============================================
	//void PushNote(const BaseNoteData &note)
	//{
	//	BaseNoteData *set = new BaseNoteData(note);
	//	m_vNoteList.push_back(set);
	//}
	//void PushSoflan(const SoflanData &soflan)
	//{
	//	SoflanData *set = new SoflanData(soflan);
	//	m_vSoflanList.push_back(set);
	//}
	//void PushBeat(const BeatData &beat)
	//{
	//	BeatData *set = new BeatData(beat);
	//	m_vBeatList.push_back(set);
	//}

	//===============================================
	//	�A�N�Z�T
	//===============================================
	OMS_TYPE GetOMSOpenType() { return m_eOMSLoadType; }
	std::string GetProjectDirectory() { return m_sProjectDirectory; }
	bool isMusicOpend() { return m_bOGGOpend; }
	MusicInfoEP *GetMusicInfoEP() { return &m_tagMusicInfoEP; }
	MusicInfoVRShoot *GetMusicInfoVRShoot() { return &m_tagMusicInfoVRShoot; }
	short GetShift() { return m_sShift; }
	void SetShift(short val) { m_sShift = val; }

private:
	// 
	MusicInfoEP m_tagMusicInfoEP;
	MusicInfoVRShoot m_tagMusicInfoVRShoot;
	std::string m_sProjectDirectory;// �v���W�F�N�g�t�H���_���̃f�B���N�g��(""�Ȃ�J���Ă��Ȃ��Ƃ����Ӗ��ł�����)
	std::string m_sProjectName;		// �v���W�F�N�g�̖��O(���ʃt�@�C����Chart�Œ�ɂ������Ȃ���������)

	bool m_bOGGOpend;				// OGG�t�@�C�����邩�ǂ���
	bool m_bSaveFlag;				// �Z�[�u�t���O(true�Ȃ���悤�Ƃ����Ƃ��ɃZ�[�u���邩����)
	OMS_TYPE m_eOMSLoadType;		// ���ʃt�@�C���J�������ǂ���
	short m_sShift;					// ����
	//WORD m_iEditNoteIndex;			// �ҏW���Ă���m�[�c�̃C���f�b�N�X

	//std::vector<BaseNoteData*>	m_vNoteList;
	//std::vector<SoflanData*>	m_vSoflanList;
	//std::vector<BeatData*>		m_vBeatList;

	void Sort();
	void Clear();
};