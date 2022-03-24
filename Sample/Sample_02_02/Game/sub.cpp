#include "stdafx.h"
#include "sub.h"

using namespace Microsoft::WRL;

//���b�Z�[�W�v���V�[�W������������B
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		// �}�E�X�̍��{�^���������ꂽ�B
		MessageBox(hWnd, L"�}�E�X�̍��{�^���������ꂽ!", L"�ʒm", MB_OK);
		break;
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