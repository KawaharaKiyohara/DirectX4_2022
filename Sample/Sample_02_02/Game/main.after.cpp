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
	HANDLE feceEventHandle;
	hr = CreateFenceEx(d3dDevice, fence, fenceValue, feceEventHandle);
	if (FAILED(hr)) {
		return 1;
	}

	// step-1 DXGIファクトリーを作成。
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"DXGIファクトリーの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-2 スワップチェインを作成。
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;									// フレームバッファの数。
	swapChainDesc.Width = 1280;										// フレームバッファの幅。	
	swapChainDesc.Height = 720;										// フレームバッファの高さ。
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// フレームバッファのカラーフォーマット。
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// フレームバッファの利用方法。
																	// 今回はレンダリングターゲットとして利用する。
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// バックバッファとフロントバッファの切り替え方法。
																	// DirectX12でサポートされているのは、下記の二つのみ。
																	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL 
																	// DXGI_SWAP_EFFECT_FLIP_DISCARD 
																	// 詳細は下記URLを参照。
																	// https://docs.microsoft.com/en-us/windows/win32/api/dxgi/ne-dxgi-dxgi_swap_effect
	swapChainDesc.SampleDesc.Count = 1;								// MSAA(Multi sampling anti aliasing)用のパラメータ。
																	// 今回はMSAAを行わないので、サンプル数は1。
	// フルスクリーン用のパラメーターを設定する。
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	// RefreshRateはバッファの切り替えタイミング。次の設定の場合1/60秒のタイミングでバッファが切り替わる。
	fullScreenDesc.RefreshRate.Denominator = 1;						// バッファ切り替えのタイミングの分子。
	fullScreenDesc.RefreshRate.Numerator = 60;						// バッファ切り替えのタイミングの分母。
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// スキャンラインの順番。
																			// DXGI_MODE_SCANLINE_ORDER_UNSPECIFIEDは詳細不明。
																			// ディスプレイの仕様に準拠。
	fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;			// フレームバッファをモニターのスケールにアップスケールする際のスケーリング方法。
																	// DXGI_MODE_SCALING_UNSPECIFIEDは詳細不明。
																	// ディスプレイの仕様に準拠。
	fullScreenDesc.Windowed = true;									// ウィンドウモードのフラグ。
																	// trueならウィンドウモードで起動。
	IDXGISwapChain1* swapChain;
	hr = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(),
		hwnd,
		&swapChainDesc,
		&fullScreenDesc,
		nullptr,
		&swapChain
	);
	if (FAILED(hr)) {
		// スワップチェインの作成に失敗した。
		MessageBox(hwnd, L"スワップチェインの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-3 リソースの情報を書き込むためのディスクリプタを作成する。
	// レンダリングターゲットビューのためのディスクリプタヒープを作成する。
	// RTV用のディスクリプタヒープを確保する。
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = 2;	// ディスクリプタの数。フレームバッファの枚数と同じ数。
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダリングターゲットビュー用。
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// ディスクリプタヒープのオプション。
													// 今回はオプションなし。
	ComPtr< ID3D12DescriptorHeap> rtvDescriptorHeap;
	hr = d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rtvDescriptorHeap));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"レンダリングターゲットビュー用のディスクリプタヒープの作成に失敗しました。", L"エラー", MB_OK);
		return 1;
	}

	// step-4 リソースの情報を作成したディスクリプタに書き込む。
	// RTV用のディスクリプタのサイズを計算する。
	int rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RTV用のディスクリプタの書き込み先ハンドルを取得する。
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescritporWriteHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorCPUHandles[2];
	ID3D12Resource* frameBuffer[2];
	for (int i = 0; i < 2; i++) {
		// スワップチェイン内に作られているフレームバッファを取得。
		swapChain->GetBuffer(i, IID_PPV_ARGS(&frameBuffer[i]));
		// ディスクリプタヒープにレンダリングターゲットビューの情報を書き込む。
		d3dDevice->CreateRenderTargetView(
			frameBuffer[i],				// フレームバッファ
			nullptr,					// 今回はnullptrでよい。
			rtvDescritporWriteHandle	// 書き込み先となるディスクリプタハンドル。
		);
		// 書き込んだディスクリプタのCPUハンドルを記憶しておく。
		rtvDescriptorCPUHandles[i] = rtvDescritporWriteHandle;
		// 書き込み先を進める。
		rtvDescritporWriteHandle.ptr += rtvDescriptorSize;
	}
	// step-5 バックバッファの番号の番号を表す変数を定義する。
	int backBufferNo = 0;

	// メッセージループを実装する。
	MSG msg = { 0 };
	

	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// ウィンドウズメッセージがなくなったのでゲームの更新処理を行う。
			// １フレームの描画開始の処理を実装。
			// コマンドアロケータをリセット。
			commandAllocator->Reset();
			// コマンドリストもリセット
			commandList->Reset(
				commandAllocator.Get(),	// コマンド作成に使用するアロケータ。
				nullptr					// デフォルトパイプラインステート。
										// nullptrを指定している場合、ドライバごとのデフォルトパイプラインステートにリセットされるので、
										// 動作は不定となる。
			);
			// step-6 バックバッファをレンダリングターゲットとして設定する。
			//　バックバッファがレンダリングとして設定できるようになるまでリソースバリアを入れる。
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(frameBuffer[backBufferNo], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandList->ResourceBarrier(1, &barrier);
			// バックバッファをレンダリングターゲットとして設定する。
			commandList->OMSetRenderTargets(1, &rtvDescriptorCPUHandles[backBufferNo], FALSE, nullptr);

			// バックバッファのカラーをクリア。
			const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			commandList->ClearRenderTargetView(rtvDescriptorCPUHandles[backBufferNo], clearColor, 0, nullptr);

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
			swapChain->Present(1, 0);
			// バックバッファの番号を入れ替える。
			backBufferNo ^= 1;
		}
	}

	return 0;
}

