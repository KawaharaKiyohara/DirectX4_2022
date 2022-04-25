#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "GraphicsEngine.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);

	// メッセージループを実装する。
	MSG msg = { 0 };
	

	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// シーンの描画処理開始。
			graphicsEngine.BeginRender();

			// シーンの描画処理終了。
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

