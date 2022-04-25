#pragma once


LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ウィンドウを初期化
HWND InitWindow(HINSTANCE hInstance, int nCmdShow);

// ウィンドウメッセージを処理する。
bool ProcessWindowsMessage(MSG& msg);

// D3Dデバイスの作成。
HRESULT CreateD3DDevice(Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice);

// コマンドアロケータの作成。
HRESULT CreateCommandAllocater(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice, 
	Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator
);
// コマンドリストの作成。
HRESULT CreateCommandList(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	const Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator,
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList
);
// コマンドキューの作成。
HRESULT CreateCommandQueue(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
);

// 同期オブジェクトを作成。
HRESULT CreateFenceEx(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE& feceEventHandle
);

// コマンドをGPUに送信
void SubmitCommandToGPU(
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
);

// GPUの処理が終了するまで待つ。
void WaitUntiFinishedGPUProcessing(
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE fenceEventHandle
);
