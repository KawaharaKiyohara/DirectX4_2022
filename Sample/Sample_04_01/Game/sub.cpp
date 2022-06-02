#include "stdafx.h"
#include "sub.h"
#include "d3dcompiler.h"
#include "d3dx12.h"

using namespace Microsoft::WRL;

//���b�Z�[�W�v���V�[�W������������B
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
	switch (msg)
	{
	case WM_DESTROY:
		//�@�폜���b�Z�[�W�������̂ŏI��������B
		PostQuitMessage(0);
		break;

	default:
		// ����ȊO�̓f�t�H���g�̏����ɔ�΂��B
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// �E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// �\���̂̃T�C�Y�B
		CS_CLASSDC,				// �E�B���h�E�̃X�^�C���B
								// �����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͂قڕs�v�Ȃ̂�CS_CLASSDC�ł悢�B
		MsgProc,				// ���b�Z�[�W�v���V�[�W���B
		0,						// 0�ł����B
		0,						// 0�ł����B
		GetModuleHandle(nullptr),	// ���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
								// �����C�ɂ��Ȃ��Ă悢�B
		nullptr,				// �A�C�R���̃n���h���B�����nullptr�ł����B
		nullptr,				// �}�E�X�J�[�\���̃n���h���B�����nullptr�ł����B
		nullptr,				// �E�B���h�E�̔w�i�F�B�����nullptr�ł����B
		nullptr,				// ���j���[���B�����nullptr�ł����B
		L"MyGame",				// �E�B���h�E�N���X�ɕt���閼�O�B
		nullptr					// NULL�ł����B
	};

	// �E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);

	// �E�B���h�E�̍쐬
	HWND hwnd = CreateWindow(
		L"MyGame",				// �g�p����E�B���h�E�N���X�̖��O�B
								// ��قǍ쐬�����E�B���h�E�N���X�Ɠ������O�ɂ���B
		L"MyGame",				// �E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢�B
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł����A
		0,						// �E�B���h�E�̏���X���W�B
		0,						// �E�B���h�E�̏���Y���W�B
		1280,					// �E�B���h�E�̕��B
		720,					// �E�B���h�E�̍����B
		nullptr,    			// �e�E�B���h�E�B�����nullptr�ł����B
		nullptr,				// ���j���[�B�����nullptr�ł����B
		hInstance,				// �A�v���P�[�V�����̃C���X�^���X�B
		nullptr                 // WM_CREATE���b�Z�[�W��lParam�p�����[�^�Ƃ��ēn���������B
	);
	// �쐬�����E�B���h�E��\����Ԃɂ���B
	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}

HRESULT CreateD3DDevice(ComPtr<ID3D12Device5>& d3dDevice)
{
	HRESULT hr = D3D12CreateDevice(
		nullptr,					// �g�p����GPU�A�_�v�^���w�肷��B
									// nullptr���w�肷��ƃf�t�H���g��GPU�A�_�v�^���g�p�����B
									// �Ⴆ�΁uNVIDIA����GPU���h�����Ă���ꍇ�ɂ�����ŗD��Ŏg�������v
									// �Ƃ����������������������ꍇ�́A���̃A�_�v�^�𖾎��I�Ɏw�肷��B
									// �p�\�R���ɓ��ڂ��ꂢ�Ă�GPU�A�_�v�^��IDXGIFactory::EnumAdapters()���g���Η񋓂ł���B
		D3D_FEATURE_LEVEL_12_1,		// DirectX12�̃o�[�W�������w�肷��B�����12.1���w�肵�Ă���B
		IID_PPV_ARGS(&d3dDevice)	// D3D�f�o�C�X�ɃA�N�Z�X����C���^�[�t�F�[�X�B
	);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D3D�f�o�C�X�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
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
		MessageBox(nullptr, L"�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
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
	// �R�}���h���X�g�������������B
	commandList->Close();
	if (FAILED(hr)) {
		MessageBox(nullptr, L"�R�}���h���X�g�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
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
		MessageBox(nullptr, L"�R�}���h�L���[�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
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
		0,						// �����l
		D3D12_FENCE_FLAG_NONE,	// �t�F���X�̓���Ɋւ���I�v�V�����B����̓I�v�V�����Ȃ��B
		IID_PPV_ARGS(&fence)
	);
	if (FAILED(hr)) {
		// �t�F���X�̍쐬�Ɏ��s�����B
		MessageBox(nullptr, L"�t�F���X�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
	}
	// �������s�����߂̃C�x���g�n���h�����쐬����B
	feceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (feceEventHandle == nullptr) {
		// �C�x���g�n���h���̍쐬�Ɏ��s�����B
		MessageBox(nullptr, L"�������s�����߂̃C�x���g�n���h���̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
	}
	return hr;
}

bool ProcessWindowsMessage(MSG& msg)
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�����B
		// ���[�U�[����̕������͂��擾����K�v������ꍇ�́A�{�֐����Ăяo���B
		TranslateMessage(&msg);
		// ���b�Z�[�W����������B
		// ���̊֐��̒��ŃE�B�h�E�N���X�Ŏw�肵�����b�Z�[�W�v���V�[�W���[�֐����Ă΂��B
		DispatchMessage(&msg);
		// ���b�Z�[�W�����������B
		return true;
	}
	// �������b�Z�[�W���Ȃ��B
	return false;
}
void SubmitCommandToGPU(
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
)
{
	// �R�}���h���X�g�����B
	commandList->Close();
	// �R�}���h���X�g��z��ɂ܂Ƃ߂āA�R�}���h�L���[�ɐς��GPU�𑖂点��B
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
	// �t�F���X�Ƀt�F���X�o�����[��ݒ肷��B
	commandQueue->Signal(fence.Get(), fenceValue);
	// �t�F���X�̒l��fenceValue�ɂȂ����Ƃ��ɃC�x���g���N����悤�ɐݒ�B
	fence->SetEventOnCompletion(fenceValue, fenceEventHandle);
	// �C�x���g�̔�����҂B
	WaitForSingleObject(fenceEventHandle, INFINITE);
	// �t�F���X�o�����[���C���N�������g�B
	fenceValue++;
}
void LoadVertexShader(ComPtr<ID3DBlob>& vsBlob)
{
	ComPtr<ID3DBlob> errorBlob;
	HRESULT hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&vsBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"���_�V�F�[�_�[�̃��[�h�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
}
void LoadPixelShader(ComPtr<ID3DBlob>& psBlob)
{
	ComPtr<ID3DBlob> errorBlob;
	auto hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain",
		"ps_5_0",
		0,
		0,
		&psBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"�s�N�Z���V�F�[�_�[�̃��[�h�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
}
void CreatePipelineState(
	ComPtr< ID3D12PipelineState>& pipelineState,
	ComPtr<ID3D12Device5> d3dDevice,
	ComPtr< ID3D12RootSignature> rootSignature,
	ComPtr< ID3DBlob>& vsBlob,
	ComPtr< ID3DBlob>& psBlob
)
{
	// �܂��̓p�C�v���C���X�e�[�g�̃f�[�^��ݒ肷��B
	// ���͒��_��`
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = { 0 };
	// ���͒��_���C�A�E�g
	pipelineStateDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	// ���[�g�V�O�l�`��
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	// ���_�V�F�[�_�[�B
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	// �s�N�Z���V�F�[�_�[�B
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	// ���X�^���C�U�X�e�[�g�B
	pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �u�����h�X�e�[�g�B
	pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g�B
	pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
	// �f�t�H���g�̃T���v���}�X�N
	pipelineStateDesc.SampleMask = UINT_MAX;
	// �v���~�e�B�u�g�|���W�[�B
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
	pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����_�����O�^�[�Q�b�g�̖����B
	pipelineStateDesc.NumRenderTargets = 1;
	// �f�v�X�X�e���V���t�H�[�}�b�g�B
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	// MASS�̐ݒ�B
	pipelineStateDesc.SampleDesc.Count = 1;

	//�ݒ肳�ꂽ�f�[�^�����ƂɃp�C�v���C���X�e�[�g���쐬����B
	auto hr = d3dDevice->CreateGraphicsPipelineState(
		&pipelineStateDesc,
		IID_PPV_ARGS(&pipelineState)
	);
	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
}
void CreateVertexBuffer(
	ComPtr< ID3D12Resource>& vertexBuffer,
	D3D12_VERTEX_BUFFER_VIEW& vertexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
)
{
	struct Vertex {
		float pos[3];	// ���_���W
		float uv[2];	// UV���W
	};
	// ���_�z��
	Vertex vertexArray[] = {
		{
			{ -0.3f, -0.3f, 0.0f },
			{ 0.0f, 0.0f }
		},
		{
			{ 0.0f,  0.3f, 0.0f },
			{ 0.0f, 1.0f }
		},
		{
			{ 0.3f, -0.3f, 0.0f },
			{ 1.0f, 1.0f }
		}
	};
	// ���_�z��̃T�C�Y��ϐ��ɋL������B
	int vertexArraySize = sizeof(vertexArray);

	// ���_�f�[�^���L�����邽�߂̃��������O���t�B�b�N��������Ɋm�ۂ���B

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexArraySize);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	// �O���t�B�b�N�������Ƀf�[�^���R�s�[����B
	uint8_t* pData;
	vertexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, vertexArray, vertexArraySize);
	vertexBuffer->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[���쐬�B�f�B�X�N���v�^�݂����Ȃ��́B
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertexArraySize;
	vertexBufferView.StrideInBytes = sizeof(vertexArray[0]);
}
void CreateIndexBuffer(
	ComPtr<ID3D12Resource>& indexBuffer,
	D3D12_INDEX_BUFFER_VIEW& indexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	// �C���f�b�N�X�̔z����`����B
	int indexArray[] = { 0, 1, 2 };
	// �C���f�b�N�X�̔z��̃T�C�Y���v�Z����B
	int indexArraySize = sizeof(indexArray);
	// �C���f�b�N�X�f�[�^���L�����邽�߂̃��������O���t�B�b�N��������Ɋm�ۂ���B
	auto indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexArraySize);
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	// �O���t�B�b�N�������Ƀf�[�^���R�s�[����B
	uint8_t* pData;
	indexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, indexArray, indexArraySize);
	indexBuffer->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[���쐬�B
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = indexArraySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}