#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "d3dcompiler.h"
#include "GraphicsEngine.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// step-1 ルートシグネチャの作成
	
	// step-2 頂点シェーダーのロード
	
	// step-3 ピクセルシェーダーのロード
	
	// step-4 パイプラインステートの作成
	
	// step-5 頂点バッファの作成。
	
	// step-6 インデックスバッファの作成
	
	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// シーンの描画処理開始。
			graphicsEngine.BeginRender();

			// step-7 ドローコールを実行する
			
			// シーンの描画処理終了。
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

