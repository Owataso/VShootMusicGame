#include "MusicInfo.h"



//------------------------------------------------------
//	ランクチェック
//------------------------------------------------------
SCORE_RANK ScoreRankCheck(int score){ for (int i = 0; i<(int)SCORE_RANK::MAX; i++){ if (score >= SCORE_BORDER[i])return (SCORE_RANK)i; }return SCORE_RANK::F; }

std::string GetNoteEnumString(BYTE byNoteType)
{
	std::string ret = (byNoteType & (BYTE)NOTE_TYPE::CHORDS) ? "同時:" : "";
	if (byNoteType & (BYTE)NOTE_TYPE::NORMAL)	ret += "N";
	if (byNoteType & (BYTE)NOTE_TYPE::CN_START) ret += "CN開始";
	if (byNoteType & (BYTE)NOTE_TYPE::CN_END)	ret += "CN終了";

	return ret;
}

//------------------------------------------------------
//	フォルダー情報構造体初期化
//------------------------------------------------------
EditFolderInfo::EditFolderInfo(LPCSTR Filename): bAudition(false)
{
	sMusicName = Filename;

	// 配列初期化
	memset(bDifficultyExistance, false, (size_t)DIFFICULTY::MAX);
	memset(bMusicExistance, false, (size_t)DIFFICULTY::MAX);

	// フォルダーの中身を列挙する
	DirectoryInfo *di = new DirectoryInfo;
	//std::string path = "DATA/Musics/";
	//path += music_name;
	tdnFile::EnumDirectory(Filename, di, false);

	// 列挙したファイルを見て情報を設定
	for (UINT i = 0; i < di->FileNames.size(); i++)
	{
		if (di->FileNames[i] == "NORMAL") bDifficultyExistance[(int)DIFFICULTY::NORMAL] = true;
		else if (di->FileNames[i] == "HYPER") bDifficultyExistance[(int)DIFFICULTY::HYPER] = true;
		else if (di->FileNames[i] == "EXPERT") bDifficultyExistance[(int)DIFFICULTY::EXPERT] = true;
		else if (di->FileNames[i] == "VIRTUOSO") bDifficultyExistance[(int)DIFFICULTY::VIRTUOSO] = true;
		else if (di->FileNames[i] == "Audition") bAudition = true;
		else if (di->FileNames[i] == "Music1") bMusicExistance[(int)DIFFICULTY::NORMAL] = true;
		else if (di->FileNames[i] == "Music2") bMusicExistance[(int)DIFFICULTY::HYPER] = true;
		else if (di->FileNames[i] == "Music3") bMusicExistance[(int)DIFFICULTY::EXPERT] = true;
		else if (di->FileNames[i] == "Music4") bMusicExistance[(int)DIFFICULTY::VIRTUOSO] = true;
		else MyAssert(false,"エラー: フォルダーの中身列挙\nもしかして Music1がmusic1になってたり");	// 例外処理
	}
	delete di;
}

std::string MusicInfoEP::GetNoteData(int index, EP_NOTE_DATA_TYPE type)
{
	//tdnDebug::OutPutLog("キテルグマ\n");
	switch (type)
	{
	case EP_NOTE_DATA_TYPE::EVENT_TIME:
		//tdnDebug::OutPutLog("キテルグマ\n");
		return std::to_string(NoteDatas[index].iEventTime);
		break;

	case EP_NOTE_DATA_TYPE::NOTE:
		return GetNoteEnumString(NoteDatas[index].byNoteType);
		break;

	case EP_NOTE_DATA_TYPE::LANE:
		return std::to_string(NoteDatas[index].byLaneNo);
		break;

	default:
		return "なし";
		break;
	}
}

void MusicInfoEP::SetNoteData(LPCSTR value, int index, EP_NOTE_DATA_TYPE type)
{
	//tdnDebug::OutPutLog("キテルグマ\n");
	//switch (no)
	//{
	//case 0:
	//	//tdnDebug::OutPutLog("キテルグマ\n");
	//	//NoteDatas[index].iEventTime;
	//	break;
	//
	//case 1:
	//	//NoteDatas[index].byNoteType;
	//	break;
	//
	//case 2:
	//	//NoteDatas[index].byLaneNo;
	//	break;
	//
	//default:
	//	//
	//	break;
	//}
}

std::string MusicInfoVRShoot::GetNoteData(int index, VRSHOOT_NOTE_DATA_TYPE type)
{
	switch (type)
	{
	case VRSHOOT_NOTE_DATA_TYPE::EVENT_TIME:
		return std::to_string(NoteDatas[index].iEventTime);
		break;

	case VRSHOOT_NOTE_DATA_TYPE::NOTE:
		//return c_NoteTypeMap.at((NOTE_TYPE)NoteDatas[index].byNoteType);
		return GetNoteEnumString(NoteDatas[index].byNoteType);
		break;

	case VRSHOOT_NOTE_DATA_TYPE::DEGREE:
		return  std::to_string(NoteDatas[index].fDegree);
		break;

	case VRSHOOT_NOTE_DATA_TYPE::RATE_Y:
		return std::to_string(NoteDatas[index].fRateY);
		break;

	case VRSHOOT_NOTE_DATA_TYPE::CN_END_INDEX:
		return (NoteDatas[index].wCNEndIndex != 0) ? std::to_string(NoteDatas[index].wCNEndIndex) : "なし";
		break;

	default:
		return "なし";
		break;
	}
}

void MusicInfoVRShoot::SetNoteData(LPCSTR value, int index, VRSHOOT_NOTE_DATA_TYPE type)
{
	switch (type)
	{
	//case 0:
	//	return std::to_string(NoteDatas[index].iEventTime);
	//	break;
	//
	//case 1:
	//	return c_NoteTypeMap.at((NOTE_TYPE)NoteDatas[index].byNoteType);
	//	break;

	case VRSHOOT_NOTE_DATA_TYPE::DEGREE:
		NoteDatas[index].fDegree = std::stof(std::string(value));
		break;

	case VRSHOOT_NOTE_DATA_TYPE::RATE_Y:
		NoteDatas[index].fRateY = std::stof(std::string(value));
		break;

	//case VRSHOOT_NOTE_DATA_TYPE::CN_END_INDEX:
	//	NoteDatas[index].wCNEndIndex;
	//	break;

	//default:
	//	return "なし";
	//	break;
	}
}

Vector2 NoteDataVRShoot::GetScreenPosition(int iMaxDegree)
{
	Vector2 ret;

	ret.x = fDegree / 180;	// -1～1
	ret.x = (ret.x + 1) / 2;// 0～1
	ret.x *= tdnSystem::GetScreenSize().right;
	//ret.x -= EDIT_VR_NOTE_SIZE;

	// 最大角度に応じてXをいじる
	ret.x -= 640;
	ret.x *= 360.0f / iMaxDegree;
	ret.x += 640;

	//ret.x = 640 - (256 / 2);						// 中心
	//ret.x += (fDegree / 180) * 640;	// 中心からの移動

	// 半分しない
	//ret.y = Math::Blend(1 - fRateY, -(EDIT_VR_NOTE_SIZE / 2), tdnSystem::GetScreenSize().bottom - EDIT_VR_NOTE_SIZE - (EDIT_VR_NOTE_SIZE / 2));
	
	// 半分する
	//ret.y = Math::Blend(1 - fRateY, -EDIT_VR_NOTE_SIZE, (float)tdnSystem::GetScreenSize().bottom - EDIT_VR_NOTE_SIZE);

	// エラーの原因になるのでYを0～720にする
	ret.y = Math::Blend(1 - fRateY, 0, (float)tdnSystem::GetScreenSize().bottom);

	//tdnDebug::OutPutLog("%.1f, %.1f\n", ret.x, ret.y);
	
	return ret;
}

Vector2 NoteDataVRShoot::GetCenterScreenPosition(int iMaxDegree)
{
	return GetScreenPosition(iMaxDegree) + Vector2(EDIT_VR_NOTE_SIZE / 2, EDIT_VR_NOTE_SIZE / 2);
}

//void NoteDataVRShoot::SetScreenPosition(Vector2 * position)
//{
//	fDegree = ((position->x / tdnSystem::GetScreenSize().right) - 0.5f) * 2;	// -1～1
//	fDegree *= 180;
//
//	fRateY = 1 - ((position->y) / tdnSystem::GetScreenSize().bottom);
//
//	// エラーチェック
//	assert(fDegree >= -180 && fDegree <= 180);
//	assert(fRateY >= 0 && fRateY <= 1);
//}

void NoteDataVRShoot::SetScreenPosition(Vector2 * position, int iMaxDegree)
{
	fDegree = ((position->x / tdnSystem::GetScreenSize().right) - 0.5f) * 2;	// -1～1
	fDegree *= 180;

	for (int i = iMaxDegree; i != 360; i *= 2)
	{
		fDegree /= 2;
	}

	// ★クランプ
	fDegree = Math::Clamp(fDegree, -180, 180);

	//// 最大角度に応じてXをいじる
	//int rate = 360 / iMaxDegree;
	//float x = position->x;
	//for (int i = iMaxDegree; i != 360; i *= 2)
	//{
	//	x -= 640;
	//}
	//x /= rate;
	//x += 640;
	////x += 640 + 320 + 160;
	//int add = 320;
	//int iScreenWidth = tdnSystem::GetScreenSize().right;
	//for (int i = iMaxDegree; i != 360; i *= 2)
	//{
	//	x += add;
	//	add /= 2;
	//	iScreenWidth *= 2;
	//}

	//fDegree = ((position->x / iScreenWidth) - 0.5f) * 2;	// -1～1
	//fDegree *= 180;
	//fDegree = Math::Clamp(fDegree, -180, 180);

	fRateY = 1 - ((position->y) / tdnSystem::GetScreenSize().bottom);

	// エラーチェック
	//assert(fDegree >= -180 && fDegree <= 180);
	assert(fRateY >= 0 && fRateY <= 1);
}

Vector3 NoteDataVRShoot::GetPosition()
{
	static const float radius(1000);
	const float radian(D3DXToRadian(fDegree));

	Vector3 pos;
	pos.x = sinf(radian) * radius;
	pos.y = fRateY * 300;
	pos.z = cosf(radian) * radius;

	//tdnDebug::OutPutLog("%f\n", pos.y);

	return pos;
}
