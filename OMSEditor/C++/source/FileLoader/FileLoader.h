#pragma once

//*****************************************************************************************************************************
//
//		このクラスはsceneEditで使う！！		ウィンドウプロシージャのコールバックとsceneEditをつなげる役割を果たす
//
//*****************************************************************************************************************************

class FileLoader
{
public:

	// 有効化と無効化		newを行っているので有効化をした後は必ず無効化を呼ぶこと！
	static void Enable();
	static void Disable();

	/* ★ウィンドウプロシージャから呼び出す */
	static bool LoadFile(char *filename);

	/* ★sceneEditから呼び出す */
	static bool GetWavFilePath(char out[]);
	static bool GetMIDIFilePath(char out[]);


	// 1つしかない実体が持つファイルパス
	char m_WavName[256];
	char m_MIDIName[256];

private:
	static FileLoader *pInstance;

	// コンストラクタ
	FileLoader();
};