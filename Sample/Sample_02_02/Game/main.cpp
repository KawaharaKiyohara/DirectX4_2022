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
	HANDLE fenceEventHandle;
	hr = CreateFenceEx(d3dDevice, fence, fenceValue, fenceEventHandle);
	if (FAILED(hr)) {
		return 1;
	}

	// step-1 DXGI�t�@�N�g���[���쐬�B
	
	// step-2 �X���b�v�`�F�C�����쐬�B

	// step-3 �����_�����O�^�[�Q�b�g�r���[���쐬����B

	// step-4 �o�b�N�o�b�t�@�̔ԍ��̔ԍ���\���ϐ����`����B

	// ���b�Z�[�W���[�v����������B
	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		if(ProcessWindowsMessage(msg) == false){
			// �E�B���h�E�Y���b�Z�[�W���Ȃ��Ȃ����̂ŃQ�[���̍X�V�������s���B
			// 
			// �R�}���h�A���P�[�^�����Z�b�g�B
			commandAllocator->Reset();
			// �R�}���h���X�g�����Z�b�g
			commandList->Reset(
				commandAllocator.Get(),	// �R�}���h�쐬�Ɏg�p����A���P�[�^�B
				nullptr					// �f�t�H���g�p�C�v���C���X�e�[�g�B
										// nullptr���w�肵�Ă���ꍇ�A�h���C�o���Ƃ̃f�t�H���g�p�C�v���C���X�e�[�g�Ƀ��Z�b�g�����̂ŁA
										// ����͕s��ƂȂ�B
			);
			// step-5 �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肷��B
			
			// step-6 �o�b�N�o�b�t�@�̃J���[���N���A�B
			
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
		}
	}

	return 0;
}

