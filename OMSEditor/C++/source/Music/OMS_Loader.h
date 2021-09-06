#pragma once

//struct MusicInfo;
class MusicInfoEP;
class MusicInfoVRShoot;
enum class OMS_RESULT;

/********************************************/
//	OMS�t�@�C��(���킽���~���[�W�b�N�X�R�A)�ǂݍ���<static class>
/********************************************/
//namespace OMSLoader
//{
//	//===============================================
//	//	���킽���~���[�W�b�N�X�R�A�ǂݍ���
//	//===============================================
//	OMS_RESULT LoadOMS(MusicInfo *pOut, LPCSTR lpcFileName);
//}

namespace EditOMSLoader
{
	//===============================================
	//	���킽���~���[�W�b�N�X�R�A�ǂݍ���
	//===============================================
	OMS_RESULT LoadBaseOMS(MusicInfoEP *pOut, std::ifstream *ifs);
	OMS_RESULT LoadVRShootOMS(MusicInfoVRShoot *pOut, std::ifstream *ifs);

	OMS_RESULT LoadBaseOMT(MusicInfoEP *pOut, std::ifstream *ifs);
	OMS_RESULT LoadVRShootOMT(MusicInfoVRShoot *pOut, std::ifstream *ifs);
}