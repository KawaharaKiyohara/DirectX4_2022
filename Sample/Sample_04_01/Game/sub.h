#pragma once


LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �E�B���h�E��������
HWND InitWindow(HINSTANCE hInstance, int nCmdShow);

// �E�B���h�E���b�Z�[�W����������B
bool ProcessWindowsMessage(MSG& msg);

// D3D�f�o�C�X�̍쐬�B
HRESULT CreateD3DDevice(Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice);

// �R�}���h�A���P�[�^�̍쐬�B
HRESULT CreateCommandAllocater(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice, 
	Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator
);
// �R�}���h���X�g�̍쐬�B
HRESULT CreateCommandList(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	const Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator,
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList
);
// �R�}���h�L���[�̍쐬�B
HRESULT CreateCommandQueue(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
);

// �����I�u�W�F�N�g���쐬�B
HRESULT CreateFenceEx(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE& feceEventHandle
);

// �R�}���h��GPU�ɑ��M
void SubmitCommandToGPU(
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
);

// GPU�̏������I������܂ő҂B
void WaitUntiFinishedGPUProcessing(
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE fenceEventHandle
);
