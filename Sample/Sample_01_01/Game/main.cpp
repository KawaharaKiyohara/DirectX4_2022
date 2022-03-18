#include "stdafx.h"
#include <Windows.h>

// step-3 メッセージプロシージャを実装する。
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
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

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// step-1 ウィンドウの初期化
	// ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ。
		CS_CLASSDC,				// ウィンドウのスタイル。
								// ここの指定でスクロールバーをつけたりできるが、ゲームでは不要なのでCS_CLASSDCでよい。
		MsgProc,				// メッセージプロシージャ(後述)
		0,						// 0でいい。
		0,						// 0でいい。
		GetModuleHandle(NULL),	// このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								// 何も気にしなくてよい。
		NULL,					// アイコンのハンドル。アイコンを変えたい場合ここを変更する。とりあえずこれでいい。
		NULL,					// マウスカーソルのハンドル。NULLの場合はデフォルト。
		NULL,					// ウィンドウの背景色。NULLの場合はデフォルト。
		NULL,					// メニュー名。NULLでいい。
		L"MyGame",				// ウィンドウクラスに付ける名前。
		NULL					// NULLでいい。
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
		NULL,					// 親ウィンドウ。ゲームでは基本的にNULLでいい。
		NULL,					// メニュー。今はNULLでいい。
		hInstance,				// アプリケーションのインスタンス。
		NULL
	);
	// 作成したウィンドウを表示状態にする。
	ShowWindow( hwnd, nCmdShow );

	// step-2 メッセージループを実装する。
	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		// PeekMessage()関数を利用してウィンドウからのメッセージを受け取る。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			// メッセージを処理する。
			// この関数の中でウィドウクラスで指定したメッセージプロシージャー関数が呼ばれる。
			DispatchMessage(&msg);
		}
	}

	return 0;
}

