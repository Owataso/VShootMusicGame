
/// 前方インクルード
//#include <Windows.h>

#pragma once

namespace DRAG_AND_DROP
{
	// windowにドラッグ＆ドロップを許可
	// hwnd : window の ハンドル
	void Initialize(HWND hwnd);

	/*******************************************************************
	  window の コールバック関数の中で case WM_DROPFILES: のときに呼ぶ  
	*******************************************************************/

	// ドロップされたファイルの絶対パスを取得
	// out_buf  : <out> ファイルパス <なかったらNULL>
	// buf_size : <in>  out_bufのサイズ
	// param    : <in>  windowが受け取った情報
	void Get_file_path(LPSTR out_buf, UINT buf_size, WPARAM param);

	// ドロップされた複数のファイルの絶対パスを取得
	// out_buf  : <out> 複数のファイルパス <２次元配列のポインター>
	// num_buf  : <in>  out_bufの数     第一配列長
	// buf_size : <in>  out_bufのサイズ 第二配列長
	// param    : <in>  windowが受け取った情報
	void Get_some_file_path(LPSTR out_buf, UINT num_buf, UINT buf_size, WPARAM param);

	// ドロップされた全ファイルの絶対パスを取得
	// 戻り値   : ファイルの数
	// out_buf  : <out> 複数のファイルパス <１次元配列でnewする>
	// buf_size : <in>  out_bufを区切るサイズ <２次元配列の第２配列長にあたる>
	// param    : <in>  windowが受け取った情報
	UINT Get_all_file_path(LPSTR &out_buf, UINT buf_size, WPARAM param);
}
