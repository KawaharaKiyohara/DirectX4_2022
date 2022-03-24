#include "stdafx.h"
#include <Windows.h>
#include "sub.h"

// step-1 DirectX関係のヘッダーファイルをインクルードする。
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
	// グラフィックカードにアクセスするためのインターフェースを作成する。
	ComPtr<ID3D12Device5> d3dDevice = nullptr;
	HRESULT hr = D3D12CreateDevice(
		nullptr,					// 使用するGPUアダプタを指定する。
									// nullptrを指定するとデフォルトのGPUアダプタが使用される。
									// 例えば「NVIDIA製のGPUが刺さっている場合にそれを最優先で使いたい」
									// といった処理を実装したい場合は、このアダプタを明示的に指定する。
									// パソコンに搭載されいてるGPUアダプタはIDXGIFactory::EnumAdapters()を使えば列挙できる。
		D3D_FEATURE_LEVEL_12_1,		// DirectX12のバージョンを指定する。今回は12.1を指定している。
		IID_PPV_ARGS(&d3dDevice)	// D3Dデバイスにアクセスするインターフェース。
	);
	if (FAILED(hr)) {
		// D3Dデバイスの作成に失敗した。
		MessageBox(nullptr, L"D3Dデバイスの作成に失敗しました。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-2 コマンドアロケーターの作成。
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		// コマンドアロケーターの作成に失敗した。
		MessageBox(hwnd, L"コマンドアロケータの作成に失敗しました。\n", L"エラー", MB_OK);
		return 1;
	}
	// step-3 コマンドリストの作成。
	ComPtr< ID3D12GraphicsCommandList4> commandList;
	hr = d3dDevice->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&commandList)
	);
	if (FAILED(hr)) {
		// コマンドリストの作成に失敗した。
		MessageBox(hwnd, L"コマンドリストの作成に失敗しました。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-4 コマンドキューの作成。
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ComPtr<ID3D12CommandQueue> commandQueue;
	hr = d3dDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS(&commandQueue) );
	if (FAILED(hr)) {
		// コマンドキューの作成に失敗した。
		MessageBox(hwnd, L"コマンドキューの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-5 DXGIファクトリーを作成。
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"DXGIファクトリーの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}
	// step-6 スワップチェインを作成。
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;						// フレームバッファの数。
	swapChainDesc.Width = 1280;							// フレームバッファの幅。	
	swapChainDesc.Height = 720;							// フレームバッファの高さ。
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// フレームバッファのカラーフォーマット。
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
	
	// step-7 GPUとCPUの同期をとるためのオブジェクトを作成。


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

