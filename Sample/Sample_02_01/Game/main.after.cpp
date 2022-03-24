#include "stdafx.h"
#include <Windows.h>
#include "sub.h"

// step-1 DirectX�֌W�̃w�b�_�[�t�@�C�����C���N���[�h����B
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
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E�̏�����
	HWND hwnd = InitWindow(hInstance, nCmdShow);

	// step-1 D3D�f�o�C�X�̍쐬�B
	// �O���t�B�b�N�J�[�h�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X���쐬����B
	ComPtr<ID3D12Device5> d3dDevice = nullptr;
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
		return 1;
	}

	// step-2 �R�}���h�A���P�[�^�[�̍쐬�B
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-3 �R�}���h���X�g�̍쐬�B
	ComPtr< ID3D12GraphicsCommandList4> commandList;
	hr = d3dDevice->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&commandList)
	);
	if (FAILED(hr)) {
		MessageBox(hwnd, L"�R�}���h���X�g�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-4 �R�}���h�L���[�̍쐬�B
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ComPtr<ID3D12CommandQueue> commandQueue;
	hr = d3dDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS(&commandQueue) );
	if (FAILED(hr)) {
		MessageBox(hwnd, L"�R�}���h�L���[�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-5 GPU��CPU�̓������Ƃ邽�߂̃I�u�W�F�N�g���쐬�B
	ComPtr<ID3D12Fence> fence;
	hr = d3dDevice->CreateFence(
		0,						// �����l
		D3D12_FENCE_FLAG_NONE,	// �t�F���X�̓���Ɋւ���I�v�V�����B����̓I�v�V�����Ȃ��B
		IID_PPV_ARGS(&fence)
	);
	if (FAILED(hr)) {
		// �t�F���X�̍쐬�Ɏ��s�����B
		MessageBox(hwnd, L"�t�F���X�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}
	int fenceValue = 1;
	// �������s�����߂̃C�x���g�n���h�����쐬����B
	HANDLE feceEWventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (feceEWventHandle == nullptr) {
		// �C�x���g�n���h���̍쐬�Ɏ��s�����B
		MessageBox(hwnd, L"�������s�����߂̃C�x���g�n���h���̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}
	// ���b�Z�[�W���[�v����������B
	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		// PeekMessage()�֐��𗘗p���ăE�B���h�E����̃��b�Z�[�W���󂯎��B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�����B
			// ���[�U�[����̕������͂��擾����K�v������ꍇ�́A�{�֐����Ăяo���B
			TranslateMessage(&msg);
			// ���b�Z�[�W����������B
			// ���̊֐��̒��ŃE�B�h�E�N���X�Ŏw�肵�����b�Z�[�W�v���V�[�W���[�֐����Ă΂��B
			DispatchMessage(&msg);
		}
		else {
			// ���b�Z�[�W���Ȃ��Ȃ����B
			// ���̕����ɃQ�[���̃R�[�h�������Ă����B
			
			// step-6 �P�t���[���̕`��J�n�̏����������B
			// �R�}���h�A���P�[�^�����Z�b�g�B
			commandAllocator->Reset();
			// �R�}���h���X�g�����Z�b�g
			commandList->Reset(
				commandAllocator.Get(),	// �R�}���h�쐬�Ɏg�p����A���P�[�^�B
				nullptr					// �f�t�H���g�p�C�v���C���X�e�[�g�B
										// nullptr���w�肵�Ă���ꍇ�A�h���C�o���Ƃ̃f�t�H���g�p�C�v���C���X�e�[�g�Ƀ��Z�b�g�����̂ŁA
										// ����͕s��ƂȂ�B
			);
			
			// ��������`�揈���������Ă������ƂƂȂ�B

			// step-7 �P�t���[���̕`��I���̏����������B
			// �R�}���h���X�g�����B
			commandList->Close();
			// �R�}���h���X�g��z��ɂ܂Ƃ߂āA�R�}���h�L���[�ɐς��GPU�𑖂点��B
			ID3D12CommandList* commandListArray[] = { commandList.Get() };
			commandQueue->ExecuteCommandLists(1, commandListArray);
			// GPU�̏����̏I���҂��B
			
			// �t�F���X�Ƀt�F���X�o�����[��ݒ肷��B
			commandQueue->Signal(fence.Get(), fenceValue);
			// �t�F���X�̒l��fenceValue�ɂȂ����Ƃ��ɃC�x���g���N����悤�ɐݒ�B
			fence->SetEventOnCompletion(fenceValue, feceEWventHandle);
			// �C�x���g�̔�����҂B
			WaitForSingleObject(feceEWventHandle, INFINITE);
			// �t�F���X�o�����[���C���N�������g�B
			fenceValue++;
		}
	}

	return 0;
}

