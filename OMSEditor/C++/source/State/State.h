#pragma once

/*
	継承用
	純粋仮想関数で
*/

enum class MESSAGE_TYPE
{
	MSG_ERROR,
	PLAY_MUSIC,	// 譜面と曲の再生
	STOP_MUSIC,	// 譜面と曲の停止
	START_EDIT,	// エディット開始
	END_EDIT,	// エディット終了
	EDIT_SELECT_NOTE,// ノーツ選択
	MOUSE_LEFT_DOWN,	// マウスクリックした瞬間
	MOUSE_LEFT_UP,		// マウス離した瞬間
	MOUSE_LEFT_DRAG,	// マウスドラッグ
	MOUSE_RIGHT_DOWN,	// マウスクリックした瞬間
	MOUSE_RIGHT_UP,		// マウス離した瞬間
	MOUSE_RIGHT_DRAG,	// マウスドラッグ
	COPY_NOTE,			// ノーツ位置コピー
	PASTE_NOTE,			// ノーツ位置ペースト
	REVERSE_X,			// X反転
	REVERSE_Y			// Y反転
};

/*電報構造体*/
struct Message
{
	//// このTelegramを送ったエンティティ
	//ENTITY_ID          Sender;

	//// このTelegramを受け取るエンティティ
	//ENTITY_ID          Receiver;

	// メッセージの内容。　（列挙型に設定されているもの）
	MESSAGE_TYPE          Msg;

	// メッセージの追加情報 (キャラクターが動く→ [追加情報] Xに２移動 )
	// 追加情報を事前に知ることが不可能なのでVoid*で提供
	void*        ExtraInfo;

	// メッセージの遅延時間
	int       DispatchTime;

	Message() :DispatchTime(-1),
		//Sender(ENTITY_ID::ID_ERROR),
		//Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::MSG_ERROR)
	{}

	Message(int time,
		MESSAGE_TYPE    msg,
		void*  info = nullptr) : DispatchTime(time),
		Msg(msg),
		ExtraInfo(info)
	{}

};

// State基底クラスをキャラクタ毎に再利用可能に
// クラステンプレートにすると人生が楽になる。
template <class entity_type>
class State
{
public:
	virtual ~State() {};

	// 入る
	virtual void Enter(entity_type*) = 0;

	// 実行します
	virtual void Execute(entity_type*) = 0;

	// 帰る
	virtual void Exit(entity_type*) = 0;

	
	// [追加]描画
	virtual void Render(entity_type*){}

	// 画像とかの描画が必要になったので、2Dの描画も作りました
	virtual void Render2D(entity_type*){}

	/*
		BaseGameEntityのステートがメッセージを受け取り
		処理を選択できるように修正する必要がある
	*/
	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(entity_type*, const Message&) = 0;

};
