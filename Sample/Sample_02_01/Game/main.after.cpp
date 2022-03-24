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
		MessageBox(nullptr, L"D3Dデバイスの作成に失敗しました。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-2 コマンドアロケーターの作成。
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
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
		MessageBox(hwnd, L"コマンドキューの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}

	// step-5 GPUとCPUの同期をとるためのオブジェクトを作成。
	ComPtr<ID3D12Fence> fence;
	hr = d3dDevice->CreateFence(
		0,						// 初期値
		D3D12_FENCE_FLAG_NONE,	// フェンスの動作に関するオプション。今回はオプションなし。
		IID_PPV_ARGS(&fence)
	);
	if (FAILED(hr)) {
		// フェンスの作成に失敗した。
		MessageBox(hwnd, L"フェンスの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}
	int fenceValue = 1;
	// 同期を行うためのイベントハンドラを作成する。
	HANDLE feceEWventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (feceEWventHandle == nullptr) {
		// イベントハンドルの作成に失敗した。
		MessageBox(hwnd, L"同期を行うためのイベントハンドルの作成に失敗した。\n", L"エラー", MB_OK);
		return 1;
	}
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
			// コマンドアロケータをリセット。
			commandAllocator->Reset();
			// コマンドリストもリセット
			commandList->Reset(
				commandAllocator.Get(),	// コマンド作成に使用するアロケータ。
				nullptr					// デフォルトパイプラインステート。
										// nullptrを指定している場合、ドライバごとのデフォルトパイプラインステートにリセットされるので、
										// 動作は不定となる。
			);
			
			// ここから描画処理を書いていくこととなる。

			// step-7 １フレームの描画終了の処理を実装。
			// コマンドリストを閉じる。
			commandList->Close();
			// コマンドリストを配列にまとめて、コマンドキューに積んでGPUを走らせる。
			ID3D12CommandList* commandListArray[] = { commandList.Get() };
			commandQueue->ExecuteCommandLists(1, commandListArray);
			// GPUの処理の終了待ち。
			
			// フェンスにフェンスバリューを設定する。
			commandQueue->Signal(fence.Get(), fenceValue);
			// フェンスの値がfenceValueになったときにイベントが起きるように設定。
			fence->SetEventOnCompletion(fenceValue, feceEWventHandle);
			// イベントの発生を待つ。
			WaitForSingleObject(feceEWventHandle, INFINITE);
			// フェンスバリューをインクリメント。
			fenceValue++;
		}
	}

	return 0;
}

