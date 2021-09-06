#include "TDNLIB.h"
#include "EnumString.h"

//------------------------------------------------------
//	���̎擾
//------------------------------------------------------
EnumString &EnumString::GetInstance()
{
	static EnumString inst;
	return inst;
}

//------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------
EnumString::EnumString()
{
	// ��Փx�R���e�i������
	m_DifficultyMap[DIFFICULTY::NORMAL]		 = "NORMAL";
	m_DifficultyMap[DIFFICULTY::HYPER]		 = "HYPER";
	m_DifficultyMap[DIFFICULTY::EXPERT]		 = "EXPERT";
	m_DifficultyMap[DIFFICULTY::VIRTUOSO]	 = "VIRTUOSO";

	// �N���A�����v�R���e�i������
	m_ClearLampMap[CLEAR_LAMP::NO_PLAY]		 = "NO_PLAY";
	m_ClearLampMap[CLEAR_LAMP::FAILED]		 = "FAILED";
	m_ClearLampMap[CLEAR_LAMP::EASY]		 = "EASY CLEAR";
	m_ClearLampMap[CLEAR_LAMP::CLEAR]		 = "CLEAR";
	m_ClearLampMap[CLEAR_LAMP::HARD]		 = "HARD CLEAR";
	m_ClearLampMap[CLEAR_LAMP::EX_HARD]		 = "EX-HARD CLEAR";
	m_ClearLampMap[CLEAR_LAMP::FULL_COMBO]	 = "FULL-COMBO";

	// �W���������R���e�i������
	m_MusicGenreMap[MUSIC_GENRE::ORIGINAL]	 = "ORIGINAL";
	m_MusicGenreMap[MUSIC_GENRE::GAME]		 = "GAME";
	m_MusicGenreMap[MUSIC_GENRE::CLASSIC]	 = "CLASSIC";
	m_MusicGenreMap[MUSIC_GENRE::VOCALOID]	 = "VOCALOID";
	m_MusicGenreMap[MUSIC_GENRE::POPS]		 = "CLASSIC";
	m_MusicGenreMap[MUSIC_GENRE::OTHER]		 = "OTHER";

	// �Q�[�W�I�v�V����������
	m_GaugeOption[GAUGE_OPTION::NORMAL]	 = "NORMAL";
	m_GaugeOption[GAUGE_OPTION::EASY]	 = "EASY";
	m_GaugeOption[GAUGE_OPTION::HARD]	 = "HARD";
	m_GaugeOption[GAUGE_OPTION::EX_HARD] = "EX-HARD";
	m_GaugeOption[GAUGE_OPTION::HAZARD]	 = "HAZARD";
}