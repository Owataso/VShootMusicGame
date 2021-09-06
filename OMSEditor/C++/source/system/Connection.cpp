#include "TDNLIB.h"
#include "stdafx.h"
#include "Connection.h"

// インスタンスの生成
SceneMain* NewSceneMain()
{
	return new SceneMain();
}

// インスタンスの削除
void DeleteSceneMain(SceneMain *p)
{
	delete p;
}

void Init(SceneMain *p, HWND i_hwnd)
{
	p->Initialize(i_hwnd);
}

// 更新処理
void Update(SceneMain *p)
{
	p->Update();
}

// 描画処理
void Render(SceneMain *p)
{
	tdnSystem::BeginScene();
	p->Render();
	tdnSystem::EndScene();
}


int LoadMIDI(SceneMain *p, LPCSTR lpcFileName)
{
	return p->DLL_LoadMIDI(lpcFileName);
}

int LoadProject(SceneMain *p, LPCSTR lpcPorjctDirectory)
{
	return p->DLL_LoadProject(lpcPorjctDirectory);
}

int LoadProjectByOMS(SceneMain * p, LPCSTR lpcFileName)
{
	return p->DLL_LoadProejctByOMS(lpcFileName);
}

void SaveProject(SceneMain * p)
{
	p->DLL_SaveProject();
}

void InportOGG(SceneMain * p, LPCSTR lpcFileName)
{
	p->DLL_InportOGG(lpcFileName);
}

int GetOMSOpenType(SceneMain * p)
{
	return p->DLL_GetOMSOpenType();
}

int isMusicOpend(SceneMain * p)
{
	return p->DLL_isMusicOpend();
}

short GetShift(SceneMain * p)
{
	return p->DLL_GetShift();
}

void SetShift(SceneMain *p, short sShift)
{
	p->DLL_SetShift(sShift);
}

int PlayMusic(SceneMain * p)
{
	return p->DLL_PlayMusic();
}

void StopMusic(SceneMain * p)
{
	p->DLL_StopMusic();
}

int StartEdit(SceneMain * p)
{
	return p->DLL_StartEdit();
}

void EndEdit(SceneMain * p)
{
	p->DLL_EndEdit();
}

void SetCurrentPosition(SceneMain * p, DWORD dwCurrentPosition)
{
	p->DLL_SetCurrentPosition(dwCurrentPosition);
}

int GetState(SceneMain * p)
{
	return p->DLL_GetState();
}

WORD GetNumNote(SceneMain * p)
{
	return p->DLL_GetNumNote();
}

void GetNoteData(SceneMain *p, char* outBuf, int bufSize, WORD noteID, WORD DataType)
{
	p->DLL_GetNoteData(outBuf, bufSize, noteID, DataType);
}

void SetNoteData(SceneMain * p, LPCSTR value, WORD noteID, WORD DataType)
{
	p->DLL_SetNoteData(value, noteID, DataType);
}

void BaseToVRShooting(SceneMain * p)
{
	p->DLL_BaseToVRShooting();
}

void SelectEditNote(SceneMain * p, LPCSTR indicesString)
{
	p->DLL_SelectEditNote(indicesString);
}

WORD GetEditNoteIndex(SceneMain * p)
{
	return p->DLL_GetEditNoteIndex();
}

void MouseLeftDown(SceneMain * p, int x, int y)
{
	p->DLL_MouseLeftDown(x, y);
}

void MouseLeftDrag(SceneMain * p, int x, int y)
{
	p->DLL_MouseLeftDrag(x, y);
}

void MouseRightDown(SceneMain * p, int x, int y)
{
	p->DLL_MouseRightDown(x, y);
}

void MouseRightUp(SceneMain * p, int x, int y)
{
	p->DLL_MouseRightUp(x, y);
}

void MouseRightDrag(SceneMain * p, int x, int y)
{
	p->DLL_MouseRightDrag(x, y);
}

int GetMaxDegree(SceneMain * p)
{
	return p->DLL_GetMaxDegree();
}

void SetMaxDegree(SceneMain * p, int degree)
{
	p->DLL_SetMaxDegree(degree);
}

int GetDragFlagVRShoot(SceneMain * p)
{
	return p->DLL_GetDragFlagVRShoot();
}

void SetDragFlagVRShoot(SceneMain * p, int iDragFlag)
{
	p->DLL_SetDragFlagVRShoot(iDragFlag);
}

void CopyNote(SceneMain * p)
{
	p->DLL_CopyNote();
}

void PasteNote(SceneMain * p)
{
	p->DLL_PasteNote();
}

void ReverseX(SceneMain * p)
{
	p->DLL_ReverseX();
}

void ReverseY(SceneMain * p)
{
	p->DLL_ReverseY();
}