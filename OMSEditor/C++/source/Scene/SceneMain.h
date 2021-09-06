#pragma once
#include "../system/BaseScene.h"
#include "../State/StateMachine.h"

class EditDataManager;
class Player;
class JudgeManager;
class PlayCursor;
class NoteManager;
class SoflanManager;
class BeatManager;
class UI;
class BeatBar;
class Camera;

struct NoteDataVRShoot;

class SceneMain : public BaseScene
{
public:
	std::unique_ptr<EditDataManager> m_pDataMgr;
	std::shared_ptr<StateMachine<SceneMain>> m_pState;
	std::unique_ptr<Camera> m_pCamera;
	//std::shared_ptr<iexMesh> m_pNote, m_pSelectNote, m_pSky;
	std::unique_ptr<tdn2DObj> m_pMap;

	std::vector<WORD> m_SelectNoteIndices;
	std::vector<float> m_CopyDegree, m_CopyY;
	void CopyNote();
	void PasteNote();
	void ReverseX();
	void ReverseY();
	UINT SetEditNoteVRShootString(LPCSTR indicesString);
	Vector2 GetScreenPositionVRShoot(WORD index);
	//void SetScreenPositionVRShoot(Vector2 *position, WORD index);
	void SetScreenPositionOnDegreeVRShoot(Vector2 *position, WORD index);
	void NoteRender(int index, RS rs = RS::COPY);
	void PlayNoteRender(NoteDataVRShoot *pNote, tdn2DObj *pImage, float alpha, WORD noteIndex);
	void ConvertCell(Vector2 * out);

	int m_iMaxDegree;
	DWORD m_dwCurrentPosition;

	// ���s�ړ��Ƃ��̕ۑ��p
	std::vector<Vector2> m_vSelectNotePositions;
	// �m�[�c�ʒu�ۑ�
	void SaveSelectPosition()
	{
		m_vSelectNotePositions.clear();
		for (UINT i = 0; i < m_SelectNoteIndices.size(); i++)
		{
			Vector2 pos = GetScreenPositionVRShoot(i);
			//tdnDebug::OutPutLog("%.1f, %.1f\n", pos.x, pos.y);
			m_vSelectNotePositions.push_back(pos);
		}
	}

	enum class DRAG_FLAG
	{
		LINE_PLOT,	// ���C���̃v���b�g
		X_SCALE,	// ���X�P�[��
		TRANSLATE	// ���s�ړ�
	}m_eDragFlag;

	//===============================================
	//	���Q�[�Ɏg������
	//===============================================
	std::shared_ptr<Player>			m_pPlayer;		// �v���C���[
	std::shared_ptr<JudgeManager>	m_pJudgeMgr;	// ����Ǘ�����
	std::shared_ptr<PlayCursor>		m_pPlayCursor;	// �Ȃ̍Đ��J�[�\��
	std::shared_ptr<NoteManager>	m_pNoteMgr;		// �m�[�c�Ǘ�����
	std::shared_ptr<SoflanManager>	m_pSoflanMgr;	// �\�t�����Ǘ�����
	std::shared_ptr<BeatManager>	m_pBeatMgr;		// ���q�Ǘ�����
	std::shared_ptr<UI>				m_pUI;			// UI�Ǘ�����
	std::shared_ptr<BeatBar>		m_pBeatBar;		// �h�D���h�D��

	//�������E���
	SceneMain();
	bool Initialize(HWND hWnd);
	~SceneMain();
	//����
	void Update();
	//�`��
	void Render();

	//===============================================
	//	DLL Export Zone.
	//===============================================
	int DLL_LoadMIDI(LPCSTR lpcFileName);

	int DLL_LoadProject(LPCSTR lpcProjectDirectory);
	int DLL_LoadProejctByOMS(LPCSTR lpcFileName);
	void DLL_SaveProject();

	void DLL_InportOGG(LPCSTR lpcFileName);

	int DLL_GetOMSOpenType();
	int DLL_isMusicOpend();

	short DLL_GetShift();
	void DLL_SetShift(short sShift);

	int DLL_PlayMusic();
	void DLL_StopMusic();

	int DLL_StartEdit();
	void DLL_EndEdit();

	void DLL_SetCurrentPosition(DWORD dwCurrentPosition);

	int DLL_GetState();
	WORD DLL_GetNumNote();
	void DLL_GetNoteData(char* outBuf, int bufSize, WORD noteID, WORD DataType);
	void DLL_SetNoteData(LPCSTR value, WORD noteID, WORD DataType);

	void DLL_BaseToVRShooting();

	void DLL_SelectEditNote(LPCSTR indicesString);
	WORD DLL_GetEditNoteIndex();

	void DLL_MouseLeftDown(int x, int y);
	void DLL_MouseLeftDrag(int x, int y);
	void DLL_MouseRightDown(int x, int y);
	void DLL_MouseRightUp(int x, int y);
	void DLL_MouseRightDrag(int x, int y);


	int DLL_GetMaxDegree();
	void DLL_SetMaxDegree(int degree);

	int DLL_GetDragFlagVRShoot();
	void DLL_SetDragFlagVRShoot(int iDragFlag);

	void DLL_CopyNote();
	void DLL_PasteNote();
	void DLL_ReverseX();
	void DLL_ReverseY();
private:
	bool m_bRender;
};