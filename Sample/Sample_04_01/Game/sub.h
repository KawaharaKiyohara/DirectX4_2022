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

void LoadVertexShader(ComPtr<ID3DBlob>& vsBlob);
void LoadPixelShader(ComPtr<ID3DBlob>& psBlob);
void CreatePipelineState(
	ComPtr< ID3D12PipelineState>& pipelineState,
	ComPtr<ID3D12Device5> d3dDevice,
	ComPtr< ID3D12RootSignature> rootSignature,
	ComPtr< ID3DBlob>& vsBlob,
	ComPtr< ID3DBlob>& psBlob
);

void CreateVertexBuffer(
	ComPtr< ID3D12Resource>& vertexBuffer,
	D3D12_VERTEX_BUFFER_VIEW& vertexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
);

void CreateIndexBuffer(
	ComPtr<ID3D12Resource>& indexBuffer,
	D3D12_INDEX_BUFFER_VIEW& indexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
);