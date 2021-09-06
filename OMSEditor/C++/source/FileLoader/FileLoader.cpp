#include "FileLoader.h"
#include<string.h>
#include<assert.h>
#include<Windows.h>
#include<filesystem>

FileLoader *FileLoader::pInstance = nullptr;

//*********************************************************************************************
//		�L���E������
//*********************************************************************************************
void FileLoader::Enable()
{
	if (!pInstance) pInstance = new FileLoader;
}
void FileLoader::Disable()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

//*********************************************************************************************
//		�E�B���h�E�v���V�[�W���̃R�[���o�b�N�֐�����Ăяo��
//*********************************************************************************************
bool FileLoader::LoadFile(char *filename)
{
	// �L��������Ȃ��̂ŏR��
	if (!pInstance) return false;

	// �t�@�C���̊g���q�擾
	namespace sys = std::tr2::sys;
	sys::path file_path(filename);

	// wav�t�@�C���擾
	if (file_path.extension() == ".wav")
		strcpy_s(pInstance->m_WavName, _countof(pInstance->m_WavName), filename);

	// MIDI�t�@�C���擾
	else if (file_path.extension() == ".mid")
		strcpy_s(pInstance->m_MIDIName, _countof(pInstance->m_MIDIName), filename);

	// ����ȊO�̃t�@�C���Ȃ̂ŗ�O����
	else
	{
		assert(0);
		return false;
	}

	return true;
}



//*********************************************************************************************
//		sceneEdit����Ăяo��
//*********************************************************************************************
bool FileLoader::GetWavFilePath(char out[])
{
	// �����񂪋󂾂�����܂��ǂݍ��߂ĂȂ��̂�false��Ԃ�
	if (strcmp("", pInstance->m_WavName) == 0) return false;

	strcpy_s(out, sizeof(out), pInstance->m_WavName);
	return true;
}

bool FileLoader::GetMIDIFilePath(char out[])
{
	// �����񂪋󂾂�����܂��ǂݍ��߂ĂȂ��̂�false��Ԃ�
	if (strcmp("", pInstance->m_MIDIName) == 0) return false;

	strcpy_s(out, sizeof(out), pInstance->m_MIDIName);
	return true;
}




//*********************************************************************************************
//		������
//*********************************************************************************************
FileLoader::FileLoader()
{
	// ���������ɂ���
	memset(m_WavName, '\0', sizeof(m_WavName));
	memset(m_MIDIName, '\0', sizeof(m_MIDIName));
}