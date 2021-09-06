#pragma once

#include "../Music/MusicInfo.h"

//===============================================
//	�Q�[���̃X�R�A�̏��
//===============================================
struct GameScore
{
	int MaxCombo;							// �ő�R���{��
	int score;								// �X�R�A(�ő�100000)
	int JudgeCount[(int)JUDGE_TYPE::MAX];	// ���萔�B
	int FinesseGauge;						// �Q�[�W(��������)

	// �X�R�A�v�Z
	void ComputeScore(int NumNorts);
};

/********************************************/
//	�X�R�A�A�R���{�����̏���ۑ�����N���X<Singleton>
/********************************************/
class GameScoreManager
{
public:
	//===============================================
	//	���̎擾
	//===============================================
	static GameScoreManager *GetInstance(){ static GameScoreManager instance; return &instance; }


	//===============================================
	//	�i�ʊJ�n
	//===============================================
	void StartGrade()
	{
		Reset();
		m_tagGameScore.FinesseGauge = 100;
	}


	//===============================================
	//	�X�R�A���Z�b�g
	//===============================================
	void Reset(){ ZeroMemory(&m_tagGameScore, sizeof(GameScore)); }


	//===============================================
	//	�Q�[�W�ȊO�̏������Z�b�g���i�ʔF��Ŏg��
	//===============================================
	void ResetExceptGauge()
	{
		int gauge = m_tagGameScore.FinesseGauge;
		Reset();
		m_tagGameScore.FinesseGauge = gauge;
	}

	//===============================================
	//	�X�R�A�f�[�^�̃|�C���^��n��
	//===============================================
	GameScore* GetGameScore(){ return &m_tagGameScore; }

private:
	//===============================================
	//	�X�R�A�f�[�^�̎���
	//===============================================
	GameScore m_tagGameScore;


	//===============================================
	//	�V���O���g���̍�@�̍�@
	//===============================================
	GameScoreManager(){ Reset(); }
	GameScoreManager(const GameScoreManager&){}
	GameScoreManager &operator =(const GameScoreManager&){}
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define GameScoreMgr (GameScoreManager::GetInstance())
