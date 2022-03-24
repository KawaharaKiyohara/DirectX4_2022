#include "stdafx.h"
#include <Windows.h>
#include "sub.h"

// DirectX関係のヘッダーファイルをインクルードする。
#include <d3d12.h>
#include <dxcapi.h>
#include <atlbase.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include <dxgi1_4.h>

using namespace Microsoft::WRL;
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウの初期化
	HWND hwnd = InitWindow(hInstance, nCmdShow);

	// step-1 D3Dデバイスの作成。
	
	// step-2 コマンドアロケーターの作成。

	// step-3 コマンドリストの作成。

	// step-4 コマンドキューの作成。

	// step-5 GPUとCPUの同期をとるためのオブジェクトを作成。
	
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
			// この部分にゲームのコードを書いていく。
			
			// step-6 １フレームの描画開始の処理を実装。
			
			// この区間にキャラや背景のドローコールを書いていくこになる。

			// step-7 １フレームの描画終了の処理を実装。

		}
	}

	return 0;
}

