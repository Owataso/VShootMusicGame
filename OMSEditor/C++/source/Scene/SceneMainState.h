#pragma once

#include "State/State.h"

class SceneMain;

/********************************************/
//	メインシーンのステート
/********************************************/
namespace SceneMainState
{
	enum class STATE
	{
		PROJECT_SELECT,
		PROJECT_LOADED,
		EDIT_EP,
		EDIT_VRSHOOT,
		PLAYING_EP,
		PLAYING_VRSHOOT
	};

	//------------------------------------------------------
	//	ステート基底クラス
	//------------------------------------------------------
	//class Base : public State<SceneMain>
	//{
	//public:
	//	Base() {}
	//	virtual ~Base(){}
	//	virtual int GetState() = 0;
	//};

	//------------------------------------------------------
	//	プロジェクト選択
	//------------------------------------------------------
	class ProjectSelect : public State<SceneMain>
	{
	public:
		static ProjectSelect *GetInstance() { static ProjectSelect i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::PROJECT_SELECT; }
	private:
		ProjectSelect() {}
	};

	//------------------------------------------------------
	//	プロジェクト読み込んだ後
	//------------------------------------------------------
	class ProjectLoaded :public State<SceneMain>
	{
	public:
		static ProjectLoaded *GetInstance() { static ProjectLoaded i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::PROJECT_LOADED; }
	private:
		ProjectLoaded() {}
	};

	//------------------------------------------------------
	//	編集モード
	//------------------------------------------------------
	class EditEP :public State<SceneMain>
	{
	public:
		static EditEP *GetInstance() { static EditEP i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::EDIT_EP; }
	private:
		EditEP() {}
	};
	class EditVRShoot :public State<SceneMain>
	{
	public:
		static EditVRShoot *GetInstance() { static EditVRShoot i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::EDIT_VRSHOOT; }
	private:
		EditVRShoot() :m_vLeftClickPosition(0, 0), m_vRightClickPosition(0, 0), m_vRightDragPosition(0, 0), m_bRightClick(false){}

		Vector2 m_vLeftClickPosition;
		Vector2 m_vRightClickPosition;
		Vector2 m_vRightDragPosition;
		bool m_bRightClick;
	};

	//------------------------------------------------------
	//	曲を流して譜面を再生する状態
	//------------------------------------------------------
	class PlayingEP :public State<SceneMain>
	{
	public:
		static PlayingEP *GetInstance() { static PlayingEP i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::PLAYING_EP; }
	private:
		void Stop(SceneMain *pMain);
		PlayingEP() {}
	};

	class PlayingVRShoot :public State<SceneMain>
	{
	public:
		static PlayingVRShoot *GetInstance() { static PlayingVRShoot i; return &i; }

		void Enter(SceneMain *pMain);
		void Execute(SceneMain *pMain);
		void Exit(SceneMain *pMain);
		void Render(SceneMain *pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		bool OnMessage(SceneMain* pMain, const Message& msg);

		// ステート取得
		int GetState() { return (int)STATE::PLAYING_VRSHOOT; }
	private:
		void Stop(SceneMain *pMain);
		PlayingVRShoot() {}
	};
};