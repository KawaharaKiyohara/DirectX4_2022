#include "stdafx.h"
#include "sub.h"

// step-3 メッセージプロシージャを実装する。
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		// マウスの左ボタンが押された。
		MessageBox(hWnd, L"マウスの左ボタンが押された!", L"通知", MB_OK);
		break;
	case WM_DESTROY:
		//　削除メッセージが来たので終了させる。
		PostQuitMessage(0);
		break;

	default:
		// それ以外はデフォルトの処理に飛ばす。
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ。
		CS_CLASSDC,				// ウィンドウのスタイル。
								// ここの指定でスクロールバーをつけたりできるが、ゲームではほぼ不要なのでCS_CLASSDCでよい。
		MsgProc,				// メッセージプロシージャ。
		0,						// 0でいい。
		0,						// 0でいい。
		GetModuleHandle(nullptr),	// このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								// 何も気にしなくてよい。
		nullptr,				// アイコンのハンドル。今回はnullptrでいい。
		nullptr,				// マウスカーソルのハンドル。今回はnullptrでいい。
		nullptr,				// ウィンドウの背景色。今回はnullptrでいい。
		nullptr,				// メニュー名。今回はnullptrでいい。
		L"MyGame",				// ウィンドウクラスに付ける名前。
		nullptr					// NULLでいい。
	};

	// ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	// ウィンドウの作成
	HWND hwnd = CreateWindow(
		L"MyGame",				// 使用するウィンドウクラスの名前。
								// 先ほど作成したウィンドウクラスと同じ名前にする。
		L"MyGame",				// ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
		0,						// ウィンドウの初期X座標。
		0,						// ウィンドウの初期Y座標。
		1280,					// ウィンドウの幅。
		720,					// ウィンドウの高さ。
		nullptr,    			// 親ウィンドウ。今回はnullptrでいい。
		nullptr,				// メニュー。今回はnullptrでいい。
		hInstance,				// アプリケーションのインスタンス。
		nullptr                 // WM_CREATEメッセージのlParamパラメータとして渡される引数。
	);
	// 作成したウィンドウを表示状態にする。
	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}