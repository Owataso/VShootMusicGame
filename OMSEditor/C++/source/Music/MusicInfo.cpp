#include "MusicInfo.h"



//------------------------------------------------------
//	�����N�`�F�b�N
//------------------------------------------------------
SCORE_RANK ScoreRankCheck(int score){ for (int i = 0; i<(int)SCORE_RANK::MAX; i++){ if (score >= SCORE_BORDER[i])return (SCORE_RANK)i; }return SCORE_RANK::F; }

std::string GetNoteEnumString(BYTE byNoteType)
{
	std::string ret = (byNoteType & (BYTE)NOTE_TYPE::CHORDS) ? "����:" : "";
	if (byNoteType & (BYTE)NOTE_TYPE::NORMAL)	ret += "N";
	if (byNoteType & (BYTE)NOTE_TYPE::CN_START) ret += "CN�J�n";
	if (byNoteType & (BYTE)NOTE_TYPE::CN_END)	ret += "CN�I��";

	return ret;
}

//------------------------------------------------------
//	�t�H���_�[���\���̏�����
//------------------------------------------------------
EditFolderInfo::EditFolderInfo(LPCSTR Filename): bAudition(false)
{
	sMusicName = Filename;

	// �z�񏉊���
	memset(bDifficultyExistance, false, (size_t)DIFFICULTY::MAX);
	memset(bMusicExistance, false, (size_t)DIFFICULTY::MAX);

	// �t�H���_�[�̒��g��񋓂���
	DirectoryInfo *di = new DirectoryInfo;
	//std::string path = "DATA/Musics/";
	//path += music_name;
	tdnFile::EnumDirectory(Filename, di, false);

	// �񋓂����t�@�C�������ď���ݒ�
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
		else MyAssert(false,"�G���[: �t�H���_�[�̒��g��\n���������� Music1��music1�ɂȂ��Ă���");	// ��O����
	}
	delete di;
}

std::string MusicInfoEP::GetNoteData(int index, EP_NOTE_DATA_TYPE type)
{
	//tdnDebug::OutPutLog("�L�e���O�}\n");
	switch (type)
	{
	case EP_NOTE_DATA_TYPE::EVENT_TIME:
		//tdnDebug::OutPutLog("�L�e���O�}\n");
		return std::to_string(NoteDatas[index].iEventTime);
		break;

	case EP_NOTE_DATA_TYPE::NOTE:
		return GetNoteEnumString(NoteDatas[index].byNoteType);
		break;

	case EP_NOTE_DATA_TYPE::LANE:
		return std::to_string(NoteDatas[index].byLaneNo);
		break;

	default:
		return "�Ȃ�";
		break;
	}
}

void MusicInfoEP::SetNoteData(LPCSTR value, int index, EP_NOTE_DATA_TYPE type)
{
	//tdnDebug::OutPutLog("�L�e���O�}\n");
	//switch (no)
	//{
	//case 0:
	//	//tdnDebug::OutPutLog("�L�e���O�}\n");
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
		return (NoteDatas[index].wCNEndIndex != 0) ? std::to_string(NoteDatas[index].wCNEndIndex) : "�Ȃ�";
		break;

	default:
		return "�Ȃ�";
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
	//	return "�Ȃ�";
	//	break;
	}
}

Vector2 NoteDataVRShoot::GetScreenPosition(int iMaxDegree)
{
	Vector2 ret;

	ret.x = fDegree / 180;	// -1�`1
	ret.x = (ret.x + 1) / 2;// 0�`1
	ret.x *= tdnSystem::GetScreenSize().right;
	//ret.x -= EDIT_VR_NOTE_SIZE;

	// �ő�p�x�ɉ�����X��������
	ret.x -= 640;
	ret.x *= 360.0f / iMaxDegree;
	ret.x += 640;

	//ret.x = 640 - (256 / 2);						// ���S
	//ret.x += (fDegree / 180) * 640;	// ���S����̈ړ�

	// �������Ȃ�
	//ret.y = Math::Blend(1 - fRateY, -(EDIT_VR_NOTE_SIZE / 2), tdnSystem::GetScreenSize().bottom - EDIT_VR_NOTE_SIZE - (EDIT_VR_NOTE_SIZE / 2));
	
	// ��������
	//ret.y = Math::Blend(1 - fRateY, -EDIT_VR_NOTE_SIZE, (float)tdnSystem::GetScreenSize().bottom - EDIT_VR_NOTE_SIZE);

	// �G���[�̌����ɂȂ�̂�Y��0�`720�ɂ���
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
//	fDegree = ((position->x / tdnSystem::GetScreenSize().right) - 0.5f) * 2;	// -1�`1
//	fDegree *= 180;
//
//	fRateY = 1 - ((position->y) / tdnSystem::GetScreenSize().bottom);
//
//	// �G���[�`�F�b�N
//	assert(fDegree >= -180 && fDegree <= 180);
//	assert(fRateY >= 0 && fRateY <= 1);
//}

void NoteDataVRShoot::SetScreenPosition(Vector2 * position, int iMaxDegree)
{
	fDegree = ((position->x / tdnSystem::GetScreenSize().right) - 0.5f) * 2;	// -1�`1
	fDegree *= 180;

	for (int i = iMaxDegree; i != 360; i *= 2)
	{
		fDegree /= 2;
	}

	// ���N�����v
	fDegree = Math::Clamp(fDegree, -180, 180);

	//// �ő�p�x�ɉ�����X��������
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

	//fDegree = ((position->x / iScreenWidth) - 0.5f) * 2;	// -1�`1
	//fDegree *= 180;
	//fDegree = Math::Clamp(fDegree, -180, 180);

	fRateY = 1 - ((position->y) / tdnSystem::GetScreenSize().bottom);

	// �G���[�`�F�b�N
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
