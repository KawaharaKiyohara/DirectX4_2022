#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"

using namespace Microsoft::WRL;
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E�̏�����
	HWND hwnd = InitWindow(hInstance, nCmdShow);

	// D3D�f�o�C�X�̏������B
	ComPtr<ID3D12Device5> d3dDevice = nullptr;
	HRESULT hr = CreateD3DDevice(d3dDevice);
	if (FAILED(hr)) {
		return 1;
	}

	// �R�}���h�A���P�[�^�[�̍쐬�B
	ComPtr< ID3D12CommandAllocator > commandAllocator;
	hr = CreateCommandAllocater(d3dDevice, commandAllocator);
	if (FAILED(hr)) {
		return 1;
	}

	// �R�}���h���X�g�̍쐬�B
	ComPtr< ID3D12GraphicsCommandList4> commandList;
	hr = CreateCommandList(d3dDevice, commandAllocator, commandList);
	if (FAILED(hr)) {
		return 1;
	}

	// �R�}���h�L���[�̍쐬�B
	ComPtr<ID3D12CommandQueue> commandQueue;
	hr = CreateCommandQueue(d3dDevice, commandQueue);
	if (FAILED(hr)) {
		return 1;
	}

	// �����I�u�W�F�N�g���쐬�B
	ComPtr<ID3D12Fence> fence;
	UINT fenceValue;
	HANDLE feceEventHandle;
	hr = CreateFenceEx(d3dDevice, fence, fenceValue, feceEventHandle);
	if (FAILED(hr)) {
		return 1;
	}

	// step-1 DXGI�t�@�N�g���[���쐬�B
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"DXGI�t�@�N�g���[�̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return 1;
	}

	// step-2 �X���b�v�`�F�C�����쐬�B
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;									// �t���[���o�b�t�@�̐��B
	swapChainDesc.Width = 1280;										// �t���[���o�b�t�@�̕��B	
	swapChainDesc.Height = 720;										// �t���[���o�b�t�@�̍����B
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �t���[���o�b�t�@�̃J���[�t�H�[�}�b�g�B
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

	// step-3 ���\�[�X�̏����������ނ��߂̃f�B�X�N���v�^���쐬����B
	// �����_�����O�^�[�Q�b�g�r���[�̂��߂̃f�B�X�N���v�^�q�[�v���쐬����B
	// RTV�p�̃f�B�X�N���v�^�q�[�v���m�ۂ���B
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = 2;	// �f�B�X�N���v�^�̐��B�t���[���o�b�t�@�̖����Ɠ������B
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�����O�^�[�Q�b�g�r���[�p�B
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// �f�B�X�N���v�^�q�[�v�̃I�v�V�����B
													// ����̓I�v�V�����Ȃ��B
	ComPtr< ID3D12DescriptorHeap> rtvDescriptorHeap;
	hr = d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rtvDescriptorHeap));
	if (FAILED(hr)) {
		MessageBox(hwnd, L"�����_�����O�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", L"�G���[", MB_OK);
		return 1;
	}

	// step-4 ���\�[�X�̏����쐬�����f�B�X�N���v�^�ɏ������ށB
	// RTV�p�̃f�B�X�N���v�^�̃T�C�Y���v�Z����B
	int rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RTV�p�̃f�B�X�N���v�^�̏������ݐ�n���h�����擾����B
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescritporWriteHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptorCPUHandles[2];
	ID3D12Resource* frameBuffer[2];
	for (int i = 0; i < 2; i++) {
		// �X���b�v�`�F�C�����ɍ���Ă���t���[���o�b�t�@���擾�B
		swapChain->GetBuffer(i, IID_PPV_ARGS(&frameBuffer[i]));
		// �f�B�X�N���v�^�q�[�v�Ƀ����_�����O�^�[�Q�b�g�r���[�̏����������ށB
		d3dDevice->CreateRenderTargetView(
			frameBuffer[i],				// �t���[���o�b�t�@
			nullptr,					// �����nullptr�ł悢�B
			rtvDescritporWriteHandle	// �������ݐ�ƂȂ�f�B�X�N���v�^�n���h���B
		);
		// �������񂾃f�B�X�N���v�^��CPU�n���h�����L�����Ă����B
		rtvDescriptorCPUHandles[i] = rtvDescritporWriteHandle;
		// �������ݐ��i�߂�B
		rtvDescritporWriteHandle.ptr += rtvDescriptorSize;
	}
	// step-5 �o�b�N�o�b�t�@�̔ԍ��̔ԍ���\���ϐ����`����B
	int backBufferNo = 0;

	// ���b�Z�[�W���[�v����������B
	MSG msg = { 0 };
	

	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// �E�B���h�E�Y���b�Z�[�W���Ȃ��Ȃ����̂ŃQ�[���̍X�V�������s���B
			// �P�t���[���̕`��J�n�̏����������B
			// �R�}���h�A���P�[�^�����Z�b�g�B
			commandAllocator->Reset();
			// �R�}���h���X�g�����Z�b�g
			commandList->Reset(
				commandAllocator.Get(),	// �R�}���h�쐬�Ɏg�p����A���P�[�^�B
				nullptr					// �f�t�H���g�p�C�v���C���X�e�[�g�B
										// nullptr���w�肵�Ă���ꍇ�A�h���C�o���Ƃ̃f�t�H���g�p�C�v���C���X�e�[�g�Ƀ��Z�b�g�����̂ŁA
										// ����͕s��ƂȂ�B
			);
			// step-6 �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肷��B
			//�@�o�b�N�o�b�t�@�������_�����O�Ƃ��Đݒ�ł���悤�ɂȂ�܂Ń��\�[�X�o���A������B
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(frameBuffer[backBufferNo], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandList->ResourceBarrier(1, &barrier);
			// �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肷��B
			commandList->OMSetRenderTargets(1, &rtvDescriptorCPUHandles[backBufferNo], FALSE, nullptr);

			// �o�b�N�o�b�t�@�̃J���[���N���A�B
			const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			commandList->ClearRenderTargetView(rtvDescriptorCPUHandles[backBufferNo], clearColor, 0, nullptr);

			/////////////////////////////////////////////////
			// ��������V�[���`�揈���J�n�B
			/////////////////////////////////////////////////
			





			/////////////////////////////////////////////////
			// �����ŃV�[���̕`�揈���I���
			/////////////////////////////////////////////////
			
			// �쐬���ꂽ�R�}���h��GPU�ɑ��M�B
			SubmitCommandToGPU(commandList, commandQueue);
			// GPU�̏����̏I���҂��B
			WaitUntiFinishedGPUProcessing(commandQueue, fence, fenceValue, fenceEventHandle);

			// step-7 �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ��B
			swapChain->Present(1, 0);
			// �o�b�N�o�b�t�@�̔ԍ������ւ���B
			backBufferNo ^= 1;
		}
	}

	return 0;
}

