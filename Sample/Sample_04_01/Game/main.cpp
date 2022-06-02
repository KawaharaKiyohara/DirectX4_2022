#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "d3dcompiler.h"
#include "GraphicsEngine.h"
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteBatch.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/ResourceUploadBatch.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// 頂点シェーダーのロード
	ComPtr<ID3DBlob> vsBlob;
	LoadVertexShader(vsBlob);

	// ピクセルシェーダーのロード
	ComPtr<ID3DBlob> psBlob;
	LoadPixelShader(psBlob);

	// 頂点バッファの作成。
	ComPtr< ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	CreateVertexBuffer(vertexBuffer, vertexBufferView, d3dDevice);

	// インデックスバッファの作成。
	ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	CreateIndexBuffer(indexBuffer, indexBufferView, d3dDevice);

	// step-1 ルートシグネチャの作成
	
	// パイプラインステートの作成
	ComPtr< ID3D12PipelineState> pipelineState;
	CreatePipelineState(
		pipelineState, 
		d3dDevice,
		rootSignature,
		vsBlob,
		psBlob );
	
	// step-2 テクスチャをロード。
	
	// step-3 テクスチャ用のディスクリプタヒープを作成して、ディスクリプタ(リソース情報)を書き込む。
	
	// step-4 サンプラステートを作成する。
	
	// step-5 サンプラステート用のディスクリプタヒープを作成して、ディスクリプタ(リソース情報)を書き込む。

	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// シーンの描画処理開始。
			graphicsEngine.BeginRender();

			auto commandList = graphicsEngine.GetCommandList();

			// step-6 ルートシグネチャを設定。
			
			// step-7 ディスクリプタヒープを設定。

			// step-8 ディスクリプタテーブルを設定。
			
			// パイプラインステートを設定。
			commandList->SetPipelineState(pipelineState.Get());

			// 頂点バッファを設定。
			commandList->IASetVertexBuffers(
				0,
				1,
				&vertexBufferView
			);
			// インデックスバッファを設定。
			commandList->IASetIndexBuffer(&indexBufferView);
			
			// ドロー。
			commandList->DrawIndexedInstanced(
				3,	// インデックスの数
				1,	// インスタンスの数
				0,
				0,
				0
			);
			// シーンの描画処理終了。
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

