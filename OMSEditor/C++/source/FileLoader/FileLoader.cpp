#include "FileLoader.h"
#include<string.h>
#include<assert.h>
#include<Windows.h>
#include<filesystem>

FileLoader *FileLoader::pInstance = nullptr;

//*********************************************************************************************
//		有効・無効可
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
//		ウィンドウプロシージャのコールバック関数から呼び出す
//*********************************************************************************************
bool FileLoader::LoadFile(char *filename)
{
	// 有効化じゃないので蹴る
	if (!pInstance) return false;

	// ファイルの拡張子取得
	namespace sys = std::tr2::sys;
	sys::path file_path(filename);

	// wavファイル取得
	if (file_path.extension() == ".wav")
		strcpy_s(pInstance->m_WavName, _countof(pInstance->m_WavName), filename);

	// MIDIファイル取得
	else if (file_path.extension() == ".mid")
		strcpy_s(pInstance->m_MIDIName, _countof(pInstance->m_MIDIName), filename);

	// それ以外のファイルなので例外処理
	else
	{
		assert(0);
		return false;
	}

	return true;
}



//*********************************************************************************************
//		sceneEditから呼び出す
//*********************************************************************************************
bool FileLoader::GetWavFilePath(char out[])
{
	// 文字列が空だったらまだ読み込めてないのでfalseを返す
	if (strcmp("", pInstance->m_WavName) == 0) return false;

	strcpy_s(out, sizeof(out), pInstance->m_WavName);
	return true;
}

bool FileLoader::GetMIDIFilePath(char out[])
{
	// 文字列が空だったらまだ読み込めてないのでfalseを返す
	if (strcmp("", pInstance->m_MIDIName) == 0) return false;

	strcpy_s(out, sizeof(out), pInstance->m_MIDIName);
	return true;
}




//*********************************************************************************************
//		初期化
//*********************************************************************************************
FileLoader::FileLoader()
{
	// 文字列を空にする
	memset(m_WavName, '\0', sizeof(m_WavName));
	memset(m_MIDIName, '\0', sizeof(m_MIDIName));
}