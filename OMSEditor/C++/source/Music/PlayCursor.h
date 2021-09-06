#pragma once

/********************************************/
//	�T�E���h�̍Đ��ʒu�Ǘ�
/********************************************/
class PlayCursor
{
public:
	//===============================================
	//	�������E���
	//===============================================
	PlayCursor();
	void Initialize();
	~PlayCursor();

	//===============================================
	//	�T�E���h����
	//===============================================
	void SetMusic(LPCSTR lpcFileName);
	void Play(int Mlimit = 0);	// �~���b�P�ʂł̍Đ����Ԃ�ݒ�
	void Stop();

	//===============================================
	//	�A�N�Z�T
	//===============================================
	int GetMSecond();	// �~���b
	void SetShift(short val){ m_sShift = val; }
	void SetOffset(DWORD val) { m_dwOffset = val; }
	void SetSpeed(float speed){ m_pSoundPlayer->SetSpeed(0, speed); }
	bool isEnd();

private:
	//===============================================
	//	�����o�ϐ�
	//===============================================
	tdnSoundBGM *m_pSoundPlayer;	// �ȗ����p
	LPCSTR *m_lpcFileName;			// �Ȃ̃t�@�C����
	int m_iDelta;					// �f���^�^�C��
	int m_iStart;					// �v���J�n����
	int m_iMSecond;					// �o�ߎ���<�b/1000>
	int m_iLimit;					// �ݒ莞��
	short m_sShift;					// �␳����
	DWORD m_dwOffset;				// �Đ��J�n����(�G�f�B�b�g�p)
	bool m_bSoundPlay;				// �T�E���h�Đ��t���O
};