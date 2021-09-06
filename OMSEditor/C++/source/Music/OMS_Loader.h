#pragma once

//struct MusicInfo;
class MusicInfoEP;
class MusicInfoVRShoot;
enum class OMS_RESULT;

/********************************************/
//	OMSファイル(おわたそミュージックスコア)読み込み<static class>
/********************************************/
//namespace OMSLoader
//{
//	//===============================================
//	//	おわたそミュージックスコア読み込み
//	//===============================================
//	OMS_RESULT LoadOMS(MusicInfo *pOut, LPCSTR lpcFileName);
//}

namespace EditOMSLoader
{
	//===============================================
	//	おわたそミュージックスコア読み込み
	//===============================================
	OMS_RESULT LoadBaseOMS(MusicInfoEP *pOut, std::ifstream *ifs);
	OMS_RESULT LoadVRShootOMS(MusicInfoVRShoot *pOut, std::ifstream *ifs);

	OMS_RESULT LoadBaseOMT(MusicInfoEP *pOut, std::ifstream *ifs);
	OMS_RESULT LoadVRShootOMT(MusicInfoVRShoot *pOut, std::ifstream *ifs);
}