
#include "stdafx.h"
#include "sub.h"
#include "GraphicsEngine.h"

namespace{
	const int FRAME_BUFFER_W = 1280;
	const int FRAME_BUFFER_H = 720;
	const int NUM_FRAME_BUFFER = 2;
}
GraphicsEngine::GraphicsEngine()
{
}
GraphicsEngine::~GraphicsEngine()
{
}
HRESULT GraphicsEngine::Initialyze(HINSTANCE hInstance, int nCmdShow)
{
	// �E�B���h�E�̏������B
	m_hwnd = InitWindow(hInstance, nCmdShow);
	
	// D3D�f�o�C�X�̏������B
	HRESULT hr = CreateD3DDevice(m_d3dDevice);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �R�}���h�A���P�[�^�[�B
	hr = CreateCommandAllocater(m_d3dDevice, m_commandAllocator);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �R�}���h�L���[�̍쐬�B
	hr = CreateCommandQueue(m_d3dDevice, m_commandQueue);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �R�}���h���X�g�̍쐬�B
	hr = CreateCommandList(
		m_d3dDevice,
		m_commandAllocator, m_commandList
	);
	if (FAILED(hr)) {
		return E_FAIL;
	}


	// �����I�u�W�F�N�g���쐬�B
	hr = CreateFenceEx(
		m_d3dDevice, 
		m_fence, 
		m_fenceValue, 
		m_fenceEventHandle
	);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// DXGI�t�@�N�g���[���쐬�B
	UINT dxgiFactoryFlags = 0;
	ComPtr< IDXGIFactory4 > dxgiFactory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		MessageBox(m_hwnd, L"DXGI�t�@�N�g���[�̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// �X���b�v�`�F�C�����쐬�B
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = NUM_FRAME_BUFFER;					// �t���[���o�b�t�@�̐��B
	swapChainDesc.Width = FRAME_BUFFER_W;							// �t���[���o�b�t�@�̕��B	
	swapChainDesc.Height = FRAME_BUFFER_H;							// �t���[���o�b�t�@�̍����B
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
																	// true�Ȃ�E�B���h�E���[�h�ŋN��
	hr = dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		m_hwnd,
		&swapChainDesc,
		&fullScreenDesc,
		nullptr,
		&m_swapChain
	);
	if (FAILED(hr)) {
		// �X���b�v�`�F�C���̍쐬�Ɏ��s�����B
		MessageBox(m_hwnd, L"�X���b�v�`�F�C���̍쐬�Ɏ��s�����B\n", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// �t���[���o�b�t�@�̂��߂̃f�B�X�N���v�^�q�[�v���쐬����B
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = NUM_FRAME_BUFFER;			// �f�B�X�N���v�^�̐��B�t���[���o�b�t�@�̖����Ɠ������B
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// �����_�����O�^�[�Q�b�g�r���[�p�B
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// �f�B�X�N���v�^�q�[�v�̃I�v�V�����B
													// ����̓I�v�V�����Ȃ��B
	
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_frameBufferDescriptorHeap));
	if (FAILED(hr)) {
		MessageBox(m_hwnd, L"�����_�����O�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// ���\�[�X�̏����쐬�����f�B�X�N���v�^�ɏ������ށB
	// �t���[���o�b�t�@�p�̃f�B�X�N���v�^�̃T�C�Y���v�Z����B
	int rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// �t���[���o�b�t�@�p�̃f�B�X�N���v�^�̏������ݐ�n���h�����擾����B
	D3D12_CPU_DESCRIPTOR_HANDLE rtvDescritporWriteHandle = m_frameBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < NUM_FRAME_BUFFER; i++) {
		// �X���b�v�`�F�C�����ɍ���Ă���t���[���o�b�t�@���擾�B
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_frameBuffer[i]));
		// �f�B�X�N���v�^�q�[�v�Ƀ����_�����O�^�[�Q�b�g�r���[�̏����������ށB
		m_d3dDevice->CreateRenderTargetView(
			m_frameBuffer[i].Get(),				// �t���[���o�b�t�@
			nullptr,					// �����nullptr�ł悢�B
			rtvDescritporWriteHandle	// �������ݐ�ƂȂ�f�B�X�N���v�^�n���h���B
		);
		// �������񂾃f�B�X�N���v�^��CPU�n���h�����L�����Ă����B
		m_frameBufferDescriptorCPUHandles[i] = rtvDescritporWriteHandle;
		// �������ݐ��i�߂�B
		rtvDescritporWriteHandle.ptr += rtvDescriptorSize;
	}
	return S_OK;
}
#include <comdef.h>
void GraphicsEngine::BeginRender()
{
	// �E�B���h�E�Y���b�Z�[�W���Ȃ��Ȃ����̂ŃQ�[���̍X�V�������s���B
	// �P�t���[���̕`��J�n�̏����������B
	// �R�}���h�A���P�[�^�����Z�b�g�B

	m_commandAllocator->Reset();
		
	// �R�}���h���X�g�����Z�b�g
	m_commandList->Reset(
		m_commandAllocator.Get(),	// �R�}���h�쐬�Ɏg�p����A���P�[�^�B
		nullptr					// �f�t�H���g�p�C�v���C���X�e�[�g�B
								// nullptr���w�肵�Ă���ꍇ�A�h���C�o���Ƃ̃f�t�H���g�p�C�v���C���X�e�[�g�Ƀ��Z�b�g�����̂ŁA
								// ����͕s��ƂȂ�B
	);
	// step-6 �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肷��B
	//�@�o�b�N�o�b�t�@�������_�����O�Ƃ��Đݒ�ł���悤�ɂȂ�܂Ń��\�[�X�o���A������B
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_frameBuffer[m_backBufferNo].Get(), 
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_commandList->ResourceBarrier(1, &barrier);
	// �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肷��B
	m_commandList->OMSetRenderTargets(
		1, 
		&m_frameBufferDescriptorCPUHandles[m_backBufferNo], 
		FALSE, 
		nullptr
	);
	D3D12_VIEWPORT viewport = { 0, 0, FRAME_BUFFER_W, FRAME_BUFFER_H, 0.0f, 1.0f };
	m_commandList->RSSetViewports(1, &viewport);
	D3D12_RECT rect = { 0, 0, FRAME_BUFFER_W, FRAME_BUFFER_H };
	m_commandList->RSSetScissorRects(1, &rect);

	// �o�b�N�o�b�t�@�̃J���[���N���A�B
	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_commandList->ClearRenderTargetView(
		m_frameBufferDescriptorCPUHandles[m_backBufferNo], 
		clearColor, 
		0, 
		nullptr
	);
}
void GraphicsEngine::EndRender()
{
	// �쐬���ꂽ�R�}���h��GPU�ɑ��M�B
	SubmitCommandToGPU(m_commandList, m_commandQueue);
	// GPU�̏����̏I���҂��B
	WaitUntiFinishedGPUProcessing(
		m_commandQueue, 
		m_fence, 
		m_fenceValue, 
		m_fenceEventHandle
	);

	// step-7 �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ��B
	m_swapChain->Present(1, 0);
	// �o�b�N�o�b�t�@�̔ԍ������ւ���B
	m_backBufferNo ^= 1;
}