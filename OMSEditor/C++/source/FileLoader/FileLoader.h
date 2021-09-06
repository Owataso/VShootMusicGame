#pragma once

//*****************************************************************************************************************************
//
//		���̃N���X��sceneEdit�Ŏg���I�I		�E�B���h�E�v���V�[�W���̃R�[���o�b�N��sceneEdit���Ȃ���������ʂ���
//
//*****************************************************************************************************************************

class FileLoader
{
public:

	// �L�����Ɩ�����		new���s���Ă���̂ŗL������������͕K�����������ĂԂ��ƁI
	static void Enable();
	static void Disable();

	/* ���E�B���h�E�v���V�[�W������Ăяo�� */
	static bool LoadFile(char *filename);

	/* ��sceneEdit����Ăяo�� */
	static bool GetWavFilePath(char out[]);
	static bool GetMIDIFilePath(char out[]);


	// 1�����Ȃ����̂����t�@�C���p�X
	char m_WavName[256];
	char m_MIDIName[256];

private:
	static FileLoader *pInstance;

	// �R���X�g���N�^
	FileLoader();
};