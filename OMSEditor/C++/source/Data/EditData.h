#pragma once

#include "../Music/MusicInfo.h"

enum class EDIT_STATE
{
	NO_OPEN,		// まだ何も開いていない状態
	OMS_BASE_OPEND,	// 汎用OMSデータ(MIDI読み込んだ後or汎用OMSデータ読みこんだ後)
};

/********************************************/
//	MIDI読み込みデータ管理
/********************************************/
class EditDataManager
{
public:
	//===============================================
	//	初期化・解放
	//===============================================
	void Initialize();
	~EditDataManager();

	//===============================================
	//	更新
	//===============================================
	void EditUpdateVRShoot();

	//===============================================
	//	読み込んだMIDIファイルから譜面を作成
	//===============================================
	OMS_RESULT LoadMIDI_MakeMusicScore(LPCSTR lpcFileName);

	//===============================================
	//	独自譜面ファイルの読み込み・書き出し
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
	//	プロジェクトの読み込み・書き出し
	//===============================================
	int LoadProject(LPCSTR lpcProjectDirectory);
	int LoadProjectByOMS(LPCSTR lpcFileName);
	void SaveProject();


	//===============================================
	//	OGG読み込み(プロジェクトフォルダ内にコピーする)
	//===============================================
	void InportOGG(LPCSTR lpcFileName);

	//===============================================
	//	譜面タイプの変更
	//===============================================
	void BaseToVRShooting();

	//===============================================
	//	ノーツ編集
	//===============================================
	//void EditStart();
	//UINT SetEditNoteVRShoot(std::vector<WORD> *list);
	//UINT SetEditNoteVRShootString(LPCSTR indicesString);
	//std::vector<WORD> *GetEditNoteIndices() { return &m_SelectNoteIndices; }
	//void SetScreenPositionVRShoot(Vector2 *position);

	//===============================================
	//	リスト追加
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
	//	アクセサ
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
	std::string m_sProjectDirectory;// プロジェクトフォルダ内のディレクトリ(""なら開いていないという意味でもある)
	std::string m_sProjectName;		// プロジェクトの名前(譜面ファイルをChart固定にしたくなかったから)

	bool m_bOGGOpend;				// OGGファイルあるかどうか
	bool m_bSaveFlag;				// セーブフラグ(trueなら閉じようとしたときにセーブするか聞く)
	OMS_TYPE m_eOMSLoadType;		// 譜面ファイル開いたかどうか
	short m_sShift;					// 共通
	//WORD m_iEditNoteIndex;			// 編集しているノーツのインデックス

	//std::vector<BaseNoteData*>	m_vNoteList;
	//std::vector<SoflanData*>	m_vSoflanList;
	//std::vector<BeatData*>		m_vBeatList;

	void Sort();
	void Clear();
};