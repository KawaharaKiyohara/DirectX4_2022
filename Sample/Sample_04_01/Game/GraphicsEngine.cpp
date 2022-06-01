
#include "stdafx.h"
#include "sub.h"
#include "GraphicsEngine.h"

namespace{
	const int FRAME_BUFFER_W = 1280;
	const int FRAME_BUFFER_H = 720;
	const int NUM_FRAME_BUFFER = 2;
}
GraphicsEngine::GraphicsEngine()
{
}
GraphicsEngine::~GraphicsEngine()
{
}
HRESULT GraphicsEngine::Initialyze(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウの初期化。
	m_hwnd = InitWindow(hInstance, nCmdShow);
	
	// D3Dデバイスの初期化。
	HRESULT hr = CreateD3DDevice(m_d3dDevice);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// コマンドアロケーター。
	hr = CreateCommandAllocater(m_d3dDevice, m_commandAllocator);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// コマンドキューの作成。
	hr = CreateCommandQueue(m_d3dDevice, m_commandQueue);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// コマンドリストの作成。
	hr = CreateCommandList(
		m_d3dDevice,
		m_commandAllocator, m_commandList
	);
	if (FAILED(hr)) {
		return E_FAIL;
	}


	// 同期オブジェクトを作成。
	hr = CreateFenceEx(
		m_d3dDevice, 
		m_fence, 
		m_fenceValue, 
		m_fenceEventHandle
	);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// DXGIファクトリーを作成。
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(m_hwnd, L"DXGIファクトリーの作成に失敗しました。\n", L"エラー", MB_OK);
		return E_FAIL;
	}

	// スワップチェインを作成。
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = NUM_FRAME_BUFFER;					// フレームバッファの数。
	swapChainDesc.Width = FRAME_BUFFER_W;							// フレームバッファの幅。	
	swapChainDesc.Height = FRAME_BUFFER_H;							// フレームバッファの高さ。
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
																	// trueならウィンドウモードで起動
	hr = dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		m_hwnd,
		&swapChainDesc,
		&fullScreenDesc,
		nullptr,
		&m_swapChain
	);
	if (FAILED(hr)) {
		// スワップチェインの作成に失敗した。
		MessageBox(m_hwnd, L"スワップチェインの作成に失敗した。\n", L"エラー", MB_OK);
		return E_FAIL;
	}

	// フレームバッファのためのディスクリプタヒープを作成する。
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = NUM_FRAME_BUFFER;			// ディスクリプタの数。フレームバッファの枚数と同じ数。
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダリングターゲットビュー用。
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// ディスクリプタヒープのオプション。
													// 今回はオプションなし。
	
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_frameBufferDescriptorHeap));
	if (FAILED(hr)) {
		MessageBox(m_hwnd, L"レンダリングターゲットビュー用のディスクリプタヒープの作成に失敗しました。", L"エラー", MB_OK);
		return E_FAIL;
	}

	// リソースの情報を作成したディスクリプタに書き込む。
	// フレームバッファ用のディスクリプタのサイズを計算する。
	int rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// フレームバッファ用のディスクリプタの書き込み先ハンドルを取得する。
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescritporWriteHandle = m_frameBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < NUM_FRAME_BUFFER; i++) {
		// スワップチェイン内に作られているフレームバッファを取得。
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_frameBuffer[i]));
		// ディスクリプタヒープにレンダリングターゲットビューの情報を書き込む。
		m_d3dDevice->CreateRenderTargetView(
			m_frameBuffer[i].Get(),				// フレームバッファ
			nullptr,					// 今回はnullptrでよい。
			rtvDescritporWriteHandle	// 書き込み先となるディスクリプタハンドル。
		);
		// 書き込んだディスクリプタのCPUハンドルを記憶しておく。
		m_frameBufferDescriptorCPUHandles[i] = rtvDescritporWriteHandle;
		// 書き込み先を進める。
		rtvDescritporWriteHandle.ptr += rtvDescriptorSize;
	}
	return S_OK;
}
#include <comdef.h>
void GraphicsEngine::BeginRender()
{
	// ウィンドウズメッセージがなくなったのでゲームの更新処理を行う。
	// １フレームの描画開始の処理を実装。
	// コマンドアロケータをリセット。

	m_commandAllocator->Reset();
		
	// コマンドリストもリセット
	m_commandList->Reset(
		m_commandAllocator.Get(),	// コマンド作成に使用するアロケータ。
		nullptr					// デフォルトパイプラインステート。
								// nullptrを指定している場合、ドライバごとのデフォルトパイプラインステートにリセットされるので、
								// 動作は不定となる。
	);
	// step-6 バックバッファをレンダリングターゲットとして設定する。
	//　バックバッファがレンダリングとして設定できるようになるまでリソースバリアを入れる。
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_frameBuffer[m_backBufferNo].Get(), 
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_commandList->ResourceBarrier(1, &barrier);
	// バックバッファをレンダリングターゲットとして設定する。
	m_commandList->OMSetRenderTargets(
		1, 
		&m_frameBufferDescriptorCPUHandles[m_backBufferNo], 
		FALSE, 
		nullptr
	);
	D3D12_VIEWPORT viewport = { 0, 0, FRAME_BUFFER_W, FRAME_BUFFER_H, 0.0f, 1.0f };
	m_commandList->RSSetViewports(1, &viewport);
	D3D12_RECT rect = { 0, 0, FRAME_BUFFER_W, FRAME_BUFFER_H };
	m_commandList->RSSetScissorRects(1, &rect);

	// バックバッファのカラーをクリア。
	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_commandList->ClearRenderTargetView(
		m_frameBufferDescriptorCPUHandles[m_backBufferNo], 
		clearColor, 
		0, 
		nullptr
	);
}
void GraphicsEngine::EndRender()
{
	// 作成されたコマンドをGPUに送信。
	SubmitCommandToGPU(m_commandList, m_commandQueue);
	// GPUの処理の終了待ち。
	WaitUntiFinishedGPUProcessing(
		m_commandQueue, 
		m_fence, 
		m_fenceValue, 
		m_fenceEventHandle
	);

	// step-7 バックバッファとフロントバッファの入れ替え。
	m_swapChain->Present(1, 0);
	// バックバッファの番号を入れ替える。
	m_backBufferNo ^= 1;
}