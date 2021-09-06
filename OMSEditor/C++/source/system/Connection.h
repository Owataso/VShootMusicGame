#include	"Scene/SceneMain.h"

//extern "C" �́AC++�̎d�l�Ŋ֐������ς���Ă��܂��̂�h���B

#ifdef __cplusplus
extern "C"
{
#endif __cplusplus

	// �C���X�^���X�̐���
	SceneMain* NewSceneMain();
	// �C���X�^���X�̍폜
	void DeleteSceneMain(SceneMain *p);
	void Init(SceneMain *p, HWND i_hwnd);
	// �X�V����
	void Update(SceneMain *p);
	// �`�揈��
	void Render(SceneMain *p);	

	// MIDI�ǂݍ���
	int LoadMIDI(SceneMain *p, LPCSTR lpcFileName);

	// �v���W�F�N�g���[�h
	int LoadProject(SceneMain *p, LPCSTR lpcProjectDirectory);
	int LoadProjectByOMS(SceneMain *p, LPCSTR lpcFileName);
	// �v���W�F�N�g�Z�[�u
	void SaveProject(SceneMain *p);

	// OGG�C���|�[�g
	void InportOGG(SceneMain *p, LPCSTR lpcFileName);

	// OMS�AOGG�̏�Ԋm�F
	int GetOMSOpenType(SceneMain *p);
	int isMusicOpend(SceneMain *p);

	// ���ʂƋȂ̒����p
	short GetShift(SceneMain *p);
	void SetShift(SceneMain *p, short iShift);

	// �Đ��E��~
	int PlayMusic(SceneMain *p);
	void StopMusic(SceneMain *p);

	// �G�f�B�b�g�̊J�n�ƏI��
	int StartEdit(SceneMain *p);
	void EndEdit(SceneMain *p);

	// ���ݍ��W�̐ݒ�
	void SetCurrentPosition(SceneMain *p, DWORD dwCurrentPosition);

	// ���݂̃X�e�[�g�擾
	int GetState(SceneMain *p);

	// �m�[�g�̃f�[�^�擾(���X�g�ǉ��p)
	WORD GetNumNote(SceneMain *p);
	void GetNoteData(SceneMain *p, char* outBuf, int bufSize, WORD noteID, WORD DataType);
	void SetNoteData(SceneMain *p, LPCSTR value, WORD noteID, WORD DataType);

	// �x�[�X����VR�V���[�e�B���O�p����
	void BaseToVRShooting(SceneMain *p);

	// �G�f�B�b�g�m�[�g�I��
	void SelectEditNote(SceneMain *p, LPCSTR indicesString);
	WORD GetEditNoteIndex(SceneMain *p);

	void MouseLeftDown(SceneMain *p, int x, int y);
	void MouseLeftDrag(SceneMain *p, int x, int y);
	void MouseRightDown(SceneMain *p, int x, int y);
	void MouseRightUp(SceneMain *p, int x, int y);
	void MouseRightDrag(SceneMain *p, int x, int y);

	int GetMaxDegree(SceneMain *p);
	void SetMaxDegree(SceneMain *p, int degree);

	int GetDragFlagVRShoot(SceneMain *p);
	void SetDragFlagVRShoot(SceneMain *p, int iDragFlag);

	void CopyNote(SceneMain *p);
	void PasteNote(SceneMain *p);
	void ReverseX(SceneMain *p);
	void ReverseY(SceneMain *p);
#ifdef __cplusplus
}
#endif __cplusplus
