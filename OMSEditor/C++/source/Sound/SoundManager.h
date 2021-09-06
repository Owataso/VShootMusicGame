#pragma once

//********************************************************************************************************************
//
//		�N���X�̐錾
//
//********************************************************************************************************************
//		�T�E���h�}�l�[�W���[(�O���[�o�����̂Ŋ֐��Ăяo��)
//****************************************************************************************************************

namespace SoundManager
{
	void Initialize();
	void Release();
	void Update();
	void SetListenerPos(const Vector2 &pos);
}

class iexSoundBase;
class tdnSoundSE;
class tdnSoundBGM;

class SE_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*�^�Ŕԍ���U�蕪����

	tdnSoundSE *play_manager;	// iexSound
	Vector2 m_ListenerPos;		// ��������l�̍��W


	int Play_in(int data_num, bool loop);
	int Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop);

public:

	//===============================================
	//	���f�[�^
	//===============================================
	struct	DATA{
		LPSTR id;					// �Ăяo���ۂɎg�p���鎯�ʃR�[�h
		LPSTR file_name;			// �t�@�C���p�X
		int play_simultaneously;	// ����̉��𕡐��Đ�����ő�l
		bool b3D;					// 3D�T�E���h�t���O
	};


	//===============================================
	//	�������Ɖ��
	//===============================================
	SE_Manager(){}
	~SE_Manager();
	void Initialize();

	//===============================================
	//	�X		�V
	//===============================================
	void Update();


	//===============================================
	//	��		��
	//===============================================
	bool isPlay(LPSTR _ID, int no);
	int Play(LPSTR _ID, bool loop = false);																			// �ȈՔ�																								// �{�����[���ݒ��
	int Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move = Vector2(0, 0), bool loop = false);				// 3D�ݒ��
	void SetTone(LPSTR _ID, int tone);
	void Stop(LPSTR _ID, int no);																					// Play�ŕԂ��Ă������l������
	void Stop_all();																								// �S���~�߂�
	void SetFX(LPSTR _ID, int no, DXA_FX flag);

	void SetListener(const Vector2 &pos);			// ���X�i�[���
};

class BGM_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*�^�Ŕԍ���U�蕪����

	tdnSoundBGM *play_manager;	// iexSound

	// �X�g���[�~���O�p
	tdnStreamSound *m_pStreamSound;
	bool m_bPlayStream;

	void Play_in(int data_num, bool loop);
	void Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop);

public:

	//===============================================
	//	���f�[�^
	//===============================================
	struct	DATA{
		LPSTR id;					// �Ăяo���ۂɎg�p���鎯�ʃR�[�h
		LPSTR file_name;			// �t�@�C���p�X
		bool b3D;
	};


	//===============================================
	//	�������Ɖ��
	//===============================================
	void Initialize();
	~BGM_Manager();

	//===============================================
	//	�X		�V
	//===============================================
	void Update();


	//===============================================
	//	��		��
	//===============================================
	void Play(LPSTR _ID, bool loop = true);																											// �ȈՔ�																								// �{�����[���ݒ��
	void Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front = Vector3(0, 0, -1), const Vector3 &move = Vector3(0, 0, 0), bool loop = true);	// 3D�ݒ��
	void Stop(LPSTR _ID);																															// Play�ŕԂ��Ă������l������
	void Stop_all();																																// �S���~�߂�

	void Fade_in(LPSTR _ID, float fade_speed, bool loop = true);
	void Fade_out(LPSTR _ID, float fade_speed);
	void Cross_fade(LPSTR inID, LPSTR outID, float fade_speed = .0075f, bool loop = true);
	void Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop);
	bool isPlay(LPSTR _ID);
	void SetSpeed(LPSTR _ID, float speed);

	void SetListener(const Vector2 &pos, const Vector2 &move);											// ���X�i�[���

	tdnStreamSound *PlayStream(LPSTR filename, bool bLoop = true){ return play_manager->PlayStream(filename, bLoop); }
	void PlayStreamIn(LPSTR filename, bool bLoop = true)
	{
		if (m_bPlayStream) m_pStreamSound->Stop();
		m_pStreamSound = play_manager->PlayStream(filename, bLoop);
		m_bPlayStream = true;
	}
	void StopStreamIn()
	{
		if (m_bPlayStream)
		{
			m_pStreamSound->Stop();
			m_bPlayStream = false;
		}
	}
};

//===============================================
//	����(�֐��Ăяo���͂������g��)
//===============================================
extern SE_Manager *g_pSE;
extern BGM_Manager *g_pBGM;