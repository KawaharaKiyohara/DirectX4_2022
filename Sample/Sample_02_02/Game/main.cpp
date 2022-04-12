#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"



using namespace Microsoft::WRL;
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウの初期化
	HWND hwnd = InitWindow(hInstance, nCmdShow);

	// D3Dデバイスの初期化。
	ComPtr<ID3D12Device5> d3dDevice = nullptr;
	HRESULT hr = CreateD3DDevice(d3dDevice);
	if (FAILED(hr)) {
		return 1;
	}

	// コマンドアロケーターの作成。
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = CreateCommandAllocater(d3dDevice, commandAllocator);
	if (FAILED(hr)) {
		return 1;
	}

	// コマンドリストの作成。
	ComPtr< ID3D12GraphicsCommandList4> commandList;
	hr = CreateCommandList(d3dDevice, commandAllocator, commandList);
	if (FAILED(hr)) {
		return 1;
	}

	// コマンドキューの作成。
	ComPtr<ID3D12CommandQueue> commandQueue;
	hr = CreateCommandQueue(d3dDevice, commandQueue);
	if (FAILED(hr)) {
		return 1;
	}

	// 同期オブジェクトを作成。
	ComPtr<ID3D12Fence> fence;
	UINT fenceValue;
	HANDLE fenceEventHandle;
	hr = CreateFenceEx(d3dDevice, fence, fenceValue, fenceEventHandle);
	if (FAILED(hr)) {
		return 1;
	}

	// step-1 DXGIファクトリーを作成。
	
	// step-2 スワップチェインを作成。

	// step-3 レンダリングターゲットビューを作成する。

	// step-4 バックバッファの番号の番号を表す変数を定義する。

	// メッセージループを実装する。
	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		if(ProcessWindowsMessage(msg) == false){
			// ウィンドウズメッセージがなくなったのでゲームの更新処理を行う。
			// 
			// コマンドアロケータをリセット。
			commandAllocator->Reset();
			// コマンドリストもリセット
			commandList->Reset(
				commandAllocator.Get(),	// コマンド作成に使用するアロケータ。
				nullptr					// デフォルトパイプラインステート。
										// nullptrを指定している場合、ドライバごとのデフォルトパイプラインステートにリセットされるので、
										// 動作は不定となる。
			);
			// step-5 バックバッファをレンダリングターゲットとして設定する。
			
			// step-6 バックバッファのカラーをクリア。
			
			/////////////////////////////////////////////////
			// ここからシーン描画処理開始。
			/////////////////////////////////////////////////
			


			/////////////////////////////////////////////////
			// ここでシーンの描画処理終わり
			/////////////////////////////////////////////////
			// 作成されたコマンドをGPUに送信。
			SubmitCommandToGPU(commandList, commandQueue);
			// GPUの処理の終了待ち。
			WaitUntiFinishedGPUProcessing(commandQueue, fence, fenceValue, fenceEventHandle);


			// step-7 バックバッファとフロントバッファの入れ替え。
		}
	}

	return 0;
}

