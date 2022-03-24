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
		// D3D�f�o�C�X�̍쐬�Ɏ��s�����B
		MessageBox(nullptr, L"D3D�f�o�C�X�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-2 �R�}���h�A���P�[�^�[�̍쐬�B
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		// �R�}���h�A���P�[�^�[�̍쐬�Ɏ��s�����B
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
		// �R�}���h���X�g�̍쐬�Ɏ��s�����B
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
		// �R�}���h�L���[�̍쐬�Ɏ��s�����B
		MessageBox(hwnd, L"�R�}���h�L���[�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-5 DXGI�t�@�N�g���[���쐬�B
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"DXGI�t�@�N�g���[�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}
	// step-6 �X���b�v�`�F�C�����쐬�B
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;						// �t���[���o�b�t�@�̐��B
	swapChainDesc.Width = 1280;							// �t���[���o�b�t�@�̕��B	
	swapChainDesc.Height = 720;							// �t���[���o�b�t�@�̍����B
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �t���[���o�b�t�@�̃J���[�t�H�[�}�b�g�B
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �t���[���o�b�t�@�̗��p���@�B
																	// ����̓����_�����O�^�[�Q�b�g�Ƃ��ė��p����B
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̐؂�ւ����@�B
																	// DirectX12�ŃT�|�[�g����Ă���̂́A���L�̓�̂݁B
																	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL 
																	// DXGI_SWAP_EFFECT_FLIP_DISCARD 
																	// �ڍׂ͉��LURL���Q�ƁB
																	// https://docs.microsoft.com/en-us/windows/win32/api/dxgi/ne-dxgi-dxgi_swap_effect
	swapChainDesc.SampleDesc.Count = 1;								// MSAA(Multi sampling anti aliasing)�p�̃p�����[�^�B
																	// �����MSAA���s��Ȃ��̂ŁA�T���v������1�B
	// �t���X�N���[���p�̃p�����[�^�[��ݒ肷��B
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	// RefreshRate�̓o�b�t�@�̐؂�ւ��^�C�~���O�B���̐ݒ�̏ꍇ1/60�b�̃^�C�~���O�Ńo�b�t�@���؂�ւ��B
	fullScreenDesc.RefreshRate.Denominator = 1;						// �o�b�t�@�؂�ւ��̃^�C�~���O�̕��q�B
	fullScreenDesc.RefreshRate.Numerator = 60;						// �o�b�t�@�؂�ւ��̃^�C�~���O�̕���B
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// �X�L�������C���̏��ԁB
																			// DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED�͏ڍוs���B
																			// �f�B�X�v���C�̎d�l�ɏ����B
	fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;			// �t���[���o�b�t�@�����j�^�[�̃X�P�[���ɃA�b�v�X�P�[������ۂ̃X�P�[�����O���@�B
																	// DXGI_MODE_SCALING_UNSPECIFIED�͏ڍוs���B
																	// �f�B�X�v���C�̎d�l�ɏ����B
	fullScreenDesc.Windowed = true;									// �E�B���h�E���[�h�̃t���O�B
																	// true�Ȃ�E�B���h�E���[�h�ŋN���B
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
		// �X���b�v�`�F�C���̍쐬�Ɏ��s�����B
		MessageBox(hwnd, L"�X���b�v�`�F�C���̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}
	
	// step-7 GPU��CPU�̓������Ƃ邽�߂̃I�u�W�F�N�g���쐬�B


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
		}
	}

	return 0;
}

