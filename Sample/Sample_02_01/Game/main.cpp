#include "stdafx.h"
#include <Windows.h>
#include "sub.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E�̏�����
	InitWindow(hInstance, nCmdShow);

	// step-1 D3D�f�o�C�X�̍쐬�B
	
	// step-2 �R�}���h�L���[�̍쐬�B
	
	// step-3 �R�}���h�A���P�[�^�[�̍쐬�B
	
	// step-4 �R�}���h���X�g�̍쐬�B
	
	// step-5 �t���[���o�b�t�@�̍쐬
	
	// step-6 GPU��CPU�̓������Ƃ邽�߂̃I�u�W�F�N�g���쐬�B

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

