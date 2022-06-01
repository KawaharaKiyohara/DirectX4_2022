#pragma once

#include <Windows.h>
#include "d3dx12.h"

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();
	HRESULT Initialyze(HINSTANCE hInstance, int nCmdShow);
	// �P�t���[���̕`��J�n���ɌĂяo�������B
	void BeginRender();	
	// 1�t���[���̕`��I�����ɌĂяo�������B
	void EndRender();
	ComPtr<ID3D12Device5> GetD3DDevice()
	{
		return m_d3dDevice;
	}
	ComPtr< ID3D12GraphicsCommandList4> GetCommandList()
	{
		return m_commandList;
	}
private:
	ComPtr<ID3D12Device5> m_d3dDevice;									// D3D�f�o�C�X�B
	ComPtr< ID3D12CommandAllocator > m_commandAllocator;				// �R�}���h�A���P�[�^�B
	ComPtr< ID3D12GraphicsCommandList4> m_commandList;					// �R�}���h���X�g�B
	ComPtr<ID3D12CommandQueue> m_commandQueue;							// �R�}���h�L���[�B
	UINT m_fenceValue = 0;												// �t�F���X�o�����[�B
	ComPtr<ID3D12Fence> m_fence;										// �t�F���X�I�u�W�F�N�g�B
	ComPtr< IDXGISwapChain1> m_swapChain;								// �X���b�v�`�F�C���B
	ComPtr< ID3D12DescriptorHeap> m_frameBufferDescriptorHeap;			// �t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v�B
	D3D12_CPU_DESCRIPTOR_HANDLE m_frameBufferDescriptorCPUHandles[2];	// �t���[���o�b�t�@�p�̃f�B�X�N���v�^��CPU�n���h���B
	ComPtr<ID3D12Resource> m_frameBuffer[2];							// �t���[���o�b�t�@�B
	int m_backBufferNo = 0;												// �o�b�N�o�b�t�@�̔ԍ��B
	HANDLE m_fenceEventHandle;											// �t�F���X�̃C�x���g�n���h���B
	HWND m_hwnd;														// �E�B���h�E�n���h���B
};