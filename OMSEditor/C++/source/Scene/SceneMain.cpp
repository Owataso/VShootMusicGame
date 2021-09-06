#include "TDNLIB.h"
#include "SceneMain.h"
#include "SceneMainState.h"
#include "../Player/Player.h"
#include "../Judge/JudgeManager.h"
#include "../Music/PlayCursor.h"
#include "../Music/NoteManager.h"
#include "../Music/SoflanManager.h"
#include "../Music/BeatManager.h"
#include "../UI/ui.h"
#include "../Music/BeatBar.h"
#include "../Data/EditData.h"
#include "../Camera/camera.h"

void SceneMain::CopyNote()
{
	m_CopyDegree.clear();
	m_CopyY.clear();
	for (auto it : m_SelectNoteIndices)
	{
		m_CopyDegree.push_back(m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[it].fDegree);
		m_CopyY.push_back(m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[it].fRateY);
	}
}

void SceneMain::PasteNote()
{
	for (UINT i = 0; i < m_SelectNoteIndices.size(); i++)
	{
		if (i >= m_CopyDegree.size()) break;
		int index = m_SelectNoteIndices[i];
		m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[index].fDegree = m_CopyDegree[i];
		m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[index].fRateY = m_CopyY[i];
	}
}

void SceneMain::ReverseX()
{
	for each (WORD index in m_SelectNoteIndices)
	{
		m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[index].fDegree *= -1;
	}
}

void SceneMain::ReverseY()
{
	for each (WORD index in m_SelectNoteIndices)
	{
		m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[index].fRateY = 1 - m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[index].fRateY;
	}
}

UINT SceneMain::SetEditNoteVRShootString(LPCSTR indicesString)
{
	m_SelectNoteIndices.clear();

	// 要約すると、「,」で区切った文字列の数字達を格納していってる
	for (int i = 0; indicesString[i] != '\0'; i++)
	{
		std::string sIndexString = "";
		for (int j = i;; j++)
		{
			if (indicesString[j] == ',')
			{
				i = j;
				break;
			}
			sIndexString += indicesString[j];
		}

		m_SelectNoteIndices.push_back((WORD)std::stoi(sIndexString));
	}
	//for each(WORD i in m_SelectNoteIndices) tdnDebug::OutPutLog("%d\n", i);

	return m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[m_SelectNoteIndices[0]].iEventTime;

}

Vector2 SceneMain::GetScreenPositionVRShoot(WORD index)
{
	// 選んでなかったら出ていけぇ！！
	if (m_SelectNoteIndices.empty()) return Vector2(0, 0);

	return m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[m_SelectNoteIndices[index]].GetScreenPosition(m_iMaxDegree);
}

//void SceneMain::SetScreenPositionVRShoot(Vector2 * position, WORD index)
//{
//	// 選んでなかったら出ていけぇ！！
//	if (m_SelectNoteIndices.empty()) return;
//
//	m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[m_SelectNoteIndices[index]].SetScreenPosition(position);
//}

void SceneMain::SetScreenPositionOnDegreeVRShoot(Vector2 * position, WORD index)
{
	// 選んでなかったら出ていけぇ！！
	if (m_SelectNoteIndices.empty()) return;

	m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas[m_SelectNoteIndices[index]].SetScreenPosition(position, m_iMaxDegree);
}

void SceneMain::NoteRender(int index, RS rs)
{
	NoteDataVRShoot *pNotes = m_pDataMgr->GetMusicInfoVRShoot()->NoteDatas;

	Vector2 vNotePos = pNotes[index].GetScreenPosition(m_iMaxDegree);
	vNotePos -= Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);

	int srcY = 1792;
	if ((pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_START) != 0)
	{
		srcY = 896;

		// プロット
		WORD wCNEndIndex = pNotes[index].wCNEndIndex;
		for (int i = 0; i < 16; i++)
		{
			float fRate = i / 16.0f;
			Vector2 vPlotPos;
			Math::Blend(&vPlotPos, fRate, vNotePos, pNotes[wCNEndIndex].GetScreenPosition(m_iMaxDegree) - Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE));
			m_pMap->Render((int)vPlotPos.x, (int)vPlotPos.y, 256, 256, 1792 - 256, srcY, 256, 256);
		}
	}
	srcY = ((pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_START) != 0 || (pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CN_END) != 0) ? 896 : 1792;
	// ノーツ
	m_pMap->Render((int)vNotePos.x, (int)vNotePos.y, 256, 256, 1792, srcY, 256, 256, rs);
	// 同時判定
	if (pNotes[index].byNoteType & (BYTE)NOTE_TYPE::CHORDS)
	{
		m_pMap->Render((int)vNotePos.x - 64, (int)vNotePos.y - 64, 384, 384, 1664, 1152, 384, 384, rs);

		// 線
		if (pNotes[index - 1].iEventTime == pNotes[index].iEventTime)
		{
			Vector2 vNotePos2 = pNotes[index - 1].GetScreenPosition(m_iMaxDegree);
			vNotePos += Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);
			//vNotePos2 += Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);
			tdnPolygon::DrawLine(vNotePos.x, vNotePos.y, vNotePos2.x, vNotePos2.y, 0xffffffff, 3);
		}
	}
}

void SceneMain::PlayNoteRender(NoteDataVRShoot *pNote, tdn2DObj *pImage, float alpha, WORD noteIndex)
{
	Vector2 vNotePos = pNote->GetScreenPosition(m_iMaxDegree);
	vNotePos -= Vector2(EDIT_VR_NOTE_SIZE, EDIT_VR_NOTE_SIZE);

	int srcY = ((pNote->byNoteType & (BYTE)NOTE_TYPE::CN_START) != 0 || (pNote->byNoteType & (BYTE)NOTE_TYPE::CN_END) != 0) ? 896 : 1792;
	pImage->SetAlpha(alpha);
	// ノーツ
	pImage->Render((int)vNotePos.x, (int)vNotePos.y, 256, 256, 1792, srcY, 256, 256);
	// 同時判定
	if (pNote->byNoteType & (BYTE)NOTE_TYPE::CHORDS) pImage->Render((int)vNotePos.x - 64, (int)vNotePos.y - 64, 384, 384, 1664, 1152, 384, 384);

	pImage->SetAlpha(1.0f);

	tdnFont::RenderStringCentering(std::to_string(noteIndex).c_str(), 48, (int)vNotePos.x + 136, (int)vNotePos.y + 104, ((BYTE)(255 * alpha) << 24) | 0x00ff0000);
}

void SceneMain::ConvertCell(Vector2 * out)
{
	//tdnDebug::OutPutLog("%d, %d\n", (int)out->x % NUM_CELL_X, (int)out->y % NUM_CELL_Y);

	static const Vector2 vCellSize(tdnSystem::GetScreenSize().right / NUM_CELL_X, tdnSystem::GetScreenSize().bottom / NUM_CELL_Y);
	out->x = (float)(((int)out->x / (int)vCellSize.x) * vCellSize.x);
	out->x -= 640;
	out->x *= m_iMaxDegree / 360.0f;
	out->x += 640;
	out->y = (float)(((int)out->y / (int)vCellSize.y) * vCellSize.y);
}

//******************************************************************
//		初期化・解放
//******************************************************************
SceneMain::SceneMain() : 
	m_dwCurrentPosition(0),
	m_eDragFlag(DRAG_FLAG::LINE_PLOT),
	m_iMaxDegree(360),
	//m_pSky(nullptr),
	//m_pNote(nullptr),
	//m_pSelectNote(nullptr),
	m_pMap(nullptr),
	m_bRender(false), 
	m_pDataMgr(nullptr),
	m_pPlayer(nullptr),
	m_pJudgeMgr(nullptr),
	m_pPlayCursor(nullptr),
	m_pNoteMgr(nullptr), 
	m_pSoflanMgr(nullptr),
	m_pBeatMgr(nullptr),
	m_pUI(nullptr),
	m_pBeatBar(nullptr)
{}

bool SceneMain::Initialize(HWND hWnd)
{
	tdnDebug::ClearLog();

	// システム初期化
	tdnSystem::InitD3D(hWnd);
	tdnText::Init();

	// データ初期化
	m_pDataMgr = std::make_unique<EditDataManager>();
	m_pDataMgr->Initialize();

	// 3Dデータ
	//m_pSky			= std::make_shared<iexMesh>("DATA/Obj/Skydome.IMO");
	//m_pNote			= std::make_shared<iexMesh>("DATA/Obj/note.IMO");
	//m_pSelectNote	= std::make_shared<iexMesh>("DATA/Obj/note.IMO");
	//m_pSky->SetPos(0, -500, 0);
	//m_pSky->SetScale(5);
	//m_pSky->Update();
	//m_pSelectNote->SetTexture(tdnTexture::Load("DATA/Obj/red.bmp"), 0);

	// 2Dデータ
	m_pMap = std::make_unique<tdn2DObj>("DATA/Image/map0.png");

	// 音ゲーに使う実体
	m_pPlayer		= std::make_shared<Player>();
	m_pJudgeMgr		= std::make_shared<JudgeManager>();
	m_pPlayCursor	= std::make_shared<PlayCursor>();
	m_pNoteMgr		= std::make_shared<NoteManager>();
	m_pSoflanMgr	= std::make_shared<SoflanManager>();
	m_pBeatMgr		= std::make_shared<BeatManager>();
	m_pUI			= std::make_shared<UI>();
	m_pBeatBar		= std::make_shared<BeatBar>();

	m_pPlayCursor->Initialize();
	m_pNoteMgr->Initialize(m_pSoflanMgr.get(), m_pJudgeMgr.get());
	m_pBeatMgr->Initialize(m_pSoflanMgr.get());
	m_pPlayer->Initialize(m_pUI.get(), m_pNoteMgr.get(), m_pJudgeMgr.get(), Player::MODE::AUTO);
	m_pJudgeMgr->Initialize(this, m_pNoteMgr.get(), GAUGE_OPTION::NORMAL, false);
	m_pUI->Initialize(this, GAUGE_OPTION::NORMAL);

	// ステート初期化
	m_pState = std::make_shared<StateMachine<SceneMain>>(this);
	m_pState->SetCurrentState(SceneMainState::ProjectSelect::GetInstance());

	// 描画していいよフラグOFF
	m_bRender = false;

	// カメラ初期化
	m_pCamera = std::make_unique<Camera>();

	return true;
}

SceneMain::~SceneMain()
{
	tdnText::CleanUpModule();
}

//******************************************************************
//		処理
//******************************************************************

void SceneMain::Update()
{
	// ステート更新
	m_pState->Update();

	m_pCamera->Update();

	// 描画していいよフラグON
	m_bRender = true;
}

//******************************************************************
//		描画
//******************************************************************

void SceneMain::Render()
{
	// Updateを通った後じゃなかったらはじく
	if (!m_bRender)return;
	m_bRender = false;

	m_pCamera->Activate();

	//m_pNote->SetPos(0, 10, 0);
	//m_pNote->Update();
	//m_pNote->Render();

	m_pState->Render();

	//m_pSky->Render();

	// ライン
	//tdnPolygon::DrawLine3D(Vector3(0, 0, 0), Vector3(0, 0, 500), 0xffaaaaff, 3.0f);
	//tdnPolygon::DrawLine3D(Vector3(0, 0, 0), Vector3(-500, 0, 0), 0xffff0000, 3.0f);
	//tdnPolygon::DrawLine3D(Vector3(0, 0, 0), Vector3(0, 500, 0), 0xff00ff00, 3.0f);

	// 網
	//for (int i = 0; i < 11; i++)
	//{
	//	tdnPolygon::DrawLine3D(Vector3(-50.0f, 0.0f, (10.0f * i) - 50.0f), Vector3(50.0f, -0.0f, (10.0f * i) - 50.0f), 0xaaffffff, 3.0f);
	//	tdnPolygon::DrawLine3D(Vector3(((10.0f * i) - 50.0f), 0, -50.0f), Vector3((10.0f * i) - 50.0f, -0.0f, 50.0f), 0xaaffffff, 3.0f);
	//}
}


int SceneMain::DLL_LoadMIDI(LPCSTR lpcFileName)
{
	// MIDIファイル読み込み&結果を返す
	return (int)m_pDataMgr->LoadMIDI_MakeMusicScore(lpcFileName);
}

int SceneMain::DLL_LoadProject(LPCSTR lpcProjectDirectory)
{
	int ret = m_pDataMgr->LoadProject(lpcProjectDirectory);
	m_pState->ChangeState(SceneMainState::ProjectLoaded::GetInstance());

	return ret;
}

int SceneMain::DLL_LoadProejctByOMS(LPCSTR lpcFileName)
{
	int ret = m_pDataMgr->LoadProjectByOMS(lpcFileName);
	m_pState->ChangeState(SceneMainState::ProjectLoaded::GetInstance());

	return ret;
}

void SceneMain::DLL_SaveProject()
{
	m_pDataMgr->SaveProject();
}

void SceneMain::DLL_InportOGG(LPCSTR lpcFileName)
{
	m_pDataMgr->InportOGG(lpcFileName);
}

int SceneMain::DLL_GetOMSOpenType()
{
	return (int)m_pDataMgr->GetOMSOpenType();
}

int SceneMain::DLL_isMusicOpend()
{
	return m_pDataMgr->isMusicOpend() ? TRUE : FALSE;
}

short SceneMain::DLL_GetShift()
{
	return m_pDataMgr->GetShift();
}

void SceneMain::DLL_SetShift(short sShift)
{
	m_pDataMgr->SetShift(sShift);
	m_pPlayCursor->SetShift(sShift);
}

int SceneMain::DLL_PlayMusic()
{
	// メッセージ送信
	if (m_pState->HandleMessage(Message(0, MESSAGE_TYPE::PLAY_MUSIC, nullptr)))
	{
		return 1;
	}

	// うまく送信されなかった
	return 0;
}

void SceneMain::DLL_StopMusic()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::STOP_MUSIC, nullptr));
}

int SceneMain::DLL_StartEdit()
{
	// メッセージ送信
	return m_pState->HandleMessage(Message(0, MESSAGE_TYPE::START_EDIT, nullptr)) ? TRUE : FALSE;
}

void SceneMain::DLL_EndEdit()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::END_EDIT, nullptr));
}

void SceneMain::DLL_SetCurrentPosition(DWORD dwCurrentPosition)
{
	m_dwCurrentPosition = dwCurrentPosition;
}

int SceneMain::DLL_GetState()
{
	if (m_pState->isInState(*SceneMainState::ProjectSelect::GetInstance()))			return (int)SceneMainState::STATE::PROJECT_SELECT;
	else if (m_pState->isInState(*SceneMainState::ProjectLoaded::GetInstance()))	return (int)SceneMainState::STATE::PROJECT_LOADED;
	else if (m_pState->isInState(*SceneMainState::EditEP::GetInstance()))			return (int)SceneMainState::STATE::EDIT_EP;
	else if (m_pState->isInState(*SceneMainState::PlayingEP::GetInstance()))		return (int)SceneMainState::STATE::PLAYING_EP;
	
	return -1;
}

WORD SceneMain::DLL_GetNumNote()
{
	if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::BASE)				return m_pDataMgr->GetMusicInfoEP()->OMSInfo.wNumNotes;
	else if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::VR_SHOOT)	return m_pDataMgr->GetMusicInfoVRShoot()->OMSInfo.wNumNotes;
	else return 0;
}

void SceneMain::DLL_GetNoteData(char* outBuf, int bufSize, WORD noteID, WORD DataType)
{
	std::string str;
	if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::BASE)				str = m_pDataMgr->GetMusicInfoEP()->GetNoteData(noteID, (EP_NOTE_DATA_TYPE)DataType);
	else if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::VR_SHOOT)	str = m_pDataMgr->GetMusicInfoVRShoot()->GetNoteData(noteID, (VRSHOOT_NOTE_DATA_TYPE)DataType);
	else str = "なし";

	//outBuf = "なし";
	strcpy_s(outBuf, bufSize, str.c_str());
	//sprintf_s(outBuf, bufSize, "%s", str.c_str());
	//int i = 0;
	//while (true)
	//{
	//
	//	char c = str.c_str()[i];
	//	outBuf[i] = c;
	//	if (c == '\0')
	//	{
	//		break;
	//	}
	//	tdnDebug::OutPutLog("[%d]%c\n", i, c);
	//	i++;
	//}
	//tdnDebug::OutPutLog("キテルグマ\n");
	//tdnDebug::OutPutLog("%s\n", outBuf);
}

void SceneMain::DLL_SetNoteData(LPCSTR value, WORD noteID, WORD DataType)
{
	if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::BASE)				m_pDataMgr->GetMusicInfoEP()->SetNoteData(value, noteID, (EP_NOTE_DATA_TYPE)DataType);
	else if (m_pDataMgr->GetOMSOpenType() == OMS_TYPE::VR_SHOOT)	m_pDataMgr->GetMusicInfoVRShoot()->SetNoteData(value, noteID, (VRSHOOT_NOTE_DATA_TYPE)DataType);
}

void SceneMain::DLL_BaseToVRShooting()
{
	m_pDataMgr->BaseToVRShooting();
}

void SceneMain::DLL_SelectEditNote(LPCSTR indicesString)
{
	//std::vector<WORD> list;
	//for (int i = 0; i < numIndices; i++)list.push_back(indices[i]);

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::EDIT_SELECT_NOTE, (LPVOID)indicesString));
}

WORD SceneMain::DLL_GetEditNoteIndex()
{
	return m_SelectNoteIndices[0];
}

void SceneMain::DLL_MouseLeftDown(int x, int y)
{
	Vector2 pos(Math::Clamp((float)x, 0, (float)tdnSystem::GetScreenSize().right), Math::Clamp((float)y, 0, (float)tdnSystem::GetScreenSize().bottom));

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::MOUSE_LEFT_DOWN, &pos));
}

void SceneMain::DLL_MouseLeftDrag(int x, int y)
{
	Vector2 pos(Math::Clamp((float)x, 0, (float)tdnSystem::GetScreenSize().right), Math::Clamp((float)y, 0, (float)tdnSystem::GetScreenSize().bottom));

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::MOUSE_LEFT_DRAG, &pos));
}

void SceneMain::DLL_MouseRightDown(int x, int y)
{
	Vector2 pos(Math::Clamp((float)x, 0, (float)tdnSystem::GetScreenSize().right), Math::Clamp((float)y, 0, (float)tdnSystem::GetScreenSize().bottom));

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::MOUSE_RIGHT_DOWN, &pos));
}

void SceneMain::DLL_MouseRightUp(int x, int y)
{
	Vector2 pos(Math::Clamp((float)x, 0, (float)tdnSystem::GetScreenSize().right), Math::Clamp((float)y, 0, (float)tdnSystem::GetScreenSize().bottom));

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::MOUSE_RIGHT_UP, &pos));
}

void SceneMain::DLL_MouseRightDrag(int x, int y)
{
	Vector2 pos(Math::Clamp((float)x, 0, (float)tdnSystem::GetScreenSize().right), Math::Clamp((float)y, 0, (float)tdnSystem::GetScreenSize().bottom));

	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::MOUSE_RIGHT_DRAG, &pos));
}

int SceneMain::DLL_GetMaxDegree()
{
	return m_iMaxDegree;
}

void SceneMain::DLL_SetMaxDegree(int degree)
{
	m_iMaxDegree = degree;

	// ★ノーツ位置セーブ
	SaveSelectPosition();
}

int SceneMain::DLL_GetDragFlagVRShoot()
{
	return (int)m_eDragFlag;
}

void SceneMain::DLL_SetDragFlagVRShoot(int iDragFlag)
{
	m_eDragFlag = (DRAG_FLAG)iDragFlag;

	// ★ノーツ位置セーブ
	SaveSelectPosition();
}

void SceneMain::DLL_CopyNote()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::COPY_NOTE, nullptr));
}

void SceneMain::DLL_PasteNote()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::PASTE_NOTE, nullptr));
}

void SceneMain::DLL_ReverseX()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::REVERSE_X, nullptr));
}

void SceneMain::DLL_ReverseY()
{
	// メッセージ送信
	m_pState->HandleMessage(Message(0, MESSAGE_TYPE::REVERSE_Y, nullptr));
}