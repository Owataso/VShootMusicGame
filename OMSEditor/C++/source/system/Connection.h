#include	"Scene/SceneMain.h"

//extern "C" は、C++の仕様で関数名が変わってしまうのを防ぐ。

#ifdef __cplusplus
extern "C"
{
#endif __cplusplus

	// インスタンスの生成
	SceneMain* NewSceneMain();
	// インスタンスの削除
	void DeleteSceneMain(SceneMain *p);
	void Init(SceneMain *p, HWND i_hwnd);
	// 更新処理
	void Update(SceneMain *p);
	// 描画処理
	void Render(SceneMain *p);	

	// MIDI読み込み
	int LoadMIDI(SceneMain *p, LPCSTR lpcFileName);

	// プロジェクトロード
	int LoadProject(SceneMain *p, LPCSTR lpcProjectDirectory);
	int LoadProjectByOMS(SceneMain *p, LPCSTR lpcFileName);
	// プロジェクトセーブ
	void SaveProject(SceneMain *p);

	// OGGインポート
	void InportOGG(SceneMain *p, LPCSTR lpcFileName);

	// OMS、OGGの状態確認
	int GetOMSOpenType(SceneMain *p);
	int isMusicOpend(SceneMain *p);

	// 譜面と曲の調整用
	short GetShift(SceneMain *p);
	void SetShift(SceneMain *p, short iShift);

	// 再生・停止
	int PlayMusic(SceneMain *p);
	void StopMusic(SceneMain *p);

	// エディットの開始と終了
	int StartEdit(SceneMain *p);
	void EndEdit(SceneMain *p);

	// 現在座標の設定
	void SetCurrentPosition(SceneMain *p, DWORD dwCurrentPosition);

	// 現在のステート取得
	int GetState(SceneMain *p);

	// ノートのデータ取得(リスト追加用)
	WORD GetNumNote(SceneMain *p);
	void GetNoteData(SceneMain *p, char* outBuf, int bufSize, WORD noteID, WORD DataType);
	void SetNoteData(SceneMain *p, LPCSTR value, WORD noteID, WORD DataType);

	// ベースからVRシューティング用譜面
	void BaseToVRShooting(SceneMain *p);

	// エディットノート選択
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
