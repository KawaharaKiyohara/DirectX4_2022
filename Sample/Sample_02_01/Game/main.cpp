#include "stdafx.h"
#include <Windows.h>
#include "sub.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウの初期化
	InitWindow(hInstance, nCmdShow);

	// step-1 D3Dデバイスの作成。
	
	// step-2 コマンドキューの作成。
	
	// step-3 コマンドアロケーターの作成。
	
	// step-4 コマンドリストの作成。
	
	// step-5 フレームバッファの作成
	
	// step-6 GPUとCPUの同期をとるためのオブジェクトを作成。

	// メッセージループを実装する。
	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		// PeekMessage()関数を利用してウィンドウからのメッセージを受け取る。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 仮想キーメッセージを文字メッセージに変換する。
			// ユーザーからの文字入力を取得する必要がある場合は、本関数を呼び出す。
			TranslateMessage(&msg);
			// メッセージを処理する。
			// この関数の中でウィドウクラスで指定したメッセージプロシージャー関数が呼ばれる。
			DispatchMessage(&msg);
		}
		else {
			// メッセージがなくなった。
		}
	}

	return 0;
}

