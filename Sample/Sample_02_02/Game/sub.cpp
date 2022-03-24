#include "stdafx.h"
#include "sub.h"

using namespace Microsoft::WRL;

//メッセージプロシージャを実装する。
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		// マウスの左ボタンが押された。
		MessageBox(hWnd, L"マウスの左ボタンが押された!", L"通知", MB_OK);
		break;
	case WM_DESTROY:
		//　削除メッセージが来たので終了させる。
		PostQuitMessage(0);
		break;

	default:
		// それ以外はデフォルトの処理に飛ばす。
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ。
		CS_CLASSDC,				// ウィンドウのスタイル。
								// ここの指定でスクロールバーをつけたりできるが、ゲームではほぼ不要なのでCS_CLASSDCでよい。
		MsgProc,				// メッセージプロシージャ。
		0,						// 0でいい。
		0,						// 0でいい。
		GetModuleHandle(nullptr),	// このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								// 何も気にしなくてよい。
		nullptr,				// アイコンのハンドル。今回はnullptrでいい。
		nullptr,				// マウスカーソルのハンドル。今回はnullptrでいい。
		nullptr,				// ウィンドウの背景色。今回はnullptrでいい。
		nullptr,				// メニュー名。今回はnullptrでいい。
		L"MyGame",				// ウィンドウクラスに付ける名前。
		nullptr					// NULLでいい。
	};

	// ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	// ウィンドウの作成
	HWND hwnd = CreateWindow(
		L"MyGame",				// 使用するウィンドウクラスの名前。
								// 先ほど作成したウィンドウクラスと同じ名前にする。
		L"MyGame",				// ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
		0,						// ウィンドウの初期X座標。
		0,						// ウィンドウの初期Y座標。
		1280,					// ウィンドウの幅。
		720,					// ウィンドウの高さ。
		nullptr,    			// 親ウィンドウ。今回はnullptrでいい。
		nullptr,				// メニュー。今回はnullptrでいい。
		hInstance,				// アプリケーションのインスタンス。
		nullptr                 // WM_CREATEメッセージのlParamパラメータとして渡される引数。
	);
	// 作成したウィンドウを表示状態にする。
	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}

HRESULT CreateD3DDevice(ComPtr<ID3D12Device5>& d3dDevice)
{
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
	}
	return hr;
}
HRESULT CreateCommandAllocater(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator
)
{
	HRESULT hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドアロケータの作成に失敗しました。\n", L"エラー", MB_OK);
	}

	return hr;
}
HRESULT CreateCommandList(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	const Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator,
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList
)
{
	HRESULT hr = d3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドリストの作成に失敗しました。\n", L"エラー", MB_OK);
	}
	return hr;
}
HRESULT CreateCommandQueue(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
)
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr = d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドキューの作成に失敗した。\n", L"エラー", MB_OK);
	}
	return hr;
}
HRESULT CreateFenceEx(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE& feceEventHandle
)
{
	fenceValue = 1;
	HRESULT hr = d3dDevice->CreateFence(
		0,						// 初期値
		D3D12_FENCE_FLAG_NONE,	// フェンスの動作に関するオプション。今回はオプションなし。
		IID_PPV_ARGS(&fence)
	);
	if (FAILED(hr)) {
		// フェンスの作成に失敗した。
		MessageBox(nullptr, L"フェンスの作成に失敗した。\n", L"エラー", MB_OK);
	}
	// 同期を行うためのイベントハンドラを作成する。
	feceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (feceEventHandle == nullptr) {
		// イベントハンドルの作成に失敗した。
		MessageBox(nullptr, L"同期を行うためのイベントハンドルの作成に失敗した。\n", L"エラー", MB_OK);
	}
	return hr;
}

bool ProcessWindowsMessage(MSG& msg)
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// 仮想キーメッセージを文字メッセージに変換する。
		// ユーザーからの文字入力を取得する必要がある場合は、本関数を呼び出す。
		TranslateMessage(&msg);
		// メッセージを処理する。
		// この関数の中でウィドウクラスで指定したメッセージプロシージャー関数が呼ばれる。
		DispatchMessage(&msg);
		// メッセージを処理した。
		return true;
	}
	// もうメッセージがない。
	return false;
}
void SubmitCommandToGPU(
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
)
{
	// コマンドリストを閉じる。
	commandList->Close();
	// コマンドリストを配列にまとめて、コマンドキューに積んでGPUを走らせる。
	ID3D12CommandList* commandListArray[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandListArray);
}

void WaitUntiFinishedGPUProcessing(
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE fenceEventHandle
)
{
	// フェンスにフェンスバリューを設定する。
	commandQueue->Signal(fence.Get(), fenceValue);
	// フェンスの値がfenceValueになったときにイベントが起きるように設定。
	fence->SetEventOnCompletion(fenceValue, fenceEventHandle);
	// イベントの発生を待つ。
	WaitForSingleObject(fenceEventHandle, INFINITE);
	// フェンスバリューをインクリメント。
	fenceValue++;
}