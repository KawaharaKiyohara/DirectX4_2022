#include "stdafx.h"
#include <Windows.h>
#include "sub.h"

// DirectX�֌W�̃w�b�_�[�t�@�C�����C���N���[�h����B
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
	
	// step-2 �R�}���h�A���P�[�^�[�̍쐬�B

	// step-3 �R�}���h���X�g�̍쐬�B

	// step-4 �R�}���h�L���[�̍쐬�B

	// step-5 GPU��CPU�̓������Ƃ邽�߂̃I�u�W�F�N�g���쐬�B
	
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
			
			// ���̋�ԂɃL������w�i�̃h���[�R�[���������Ă������ɂȂ�B

			// step-7 �P�t���[���̕`��I���̏����������B

		}
	}

	return 0;
}

