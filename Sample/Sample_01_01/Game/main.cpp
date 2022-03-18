#include "stdafx.h"
#include <Windows.h>

// step-3 ���b�Z�[�W�v���V�[�W������������B
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

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// step-1 �E�B���h�E�̏�����
	// �E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// �\���̂̃T�C�Y�B
		CS_CLASSDC,				// �E�B���h�E�̃X�^�C���B
								// �����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͕s�v�Ȃ̂�CS_CLASSDC�ł悢�B
		MsgProc,				// ���b�Z�[�W�v���V�[�W��(��q)
		0,						// 0�ł����B
		0,						// 0�ł����B
		GetModuleHandle(NULL),	// ���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
								// �����C�ɂ��Ȃ��Ă悢�B
		NULL,					// �A�C�R���̃n���h���B�A�C�R����ς������ꍇ������ύX����B�Ƃ肠��������ł����B
		NULL,					// �}�E�X�J�[�\���̃n���h���BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					// �E�B���h�E�̔w�i�F�BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					// ���j���[���BNULL�ł����B
		L"MyGame",				// �E�B���h�E�N���X�ɕt���閼�O�B
		NULL					// NULL�ł����B
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
		NULL,					// �e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł����B
		NULL,					// ���j���[�B����NULL�ł����B
		hInstance,				// �A�v���P�[�V�����̃C���X�^���X�B
		NULL
	);
	// �쐬�����E�B���h�E��\����Ԃɂ���B
	ShowWindow( hwnd, nCmdShow );

	// step-2 ���b�Z�[�W���[�v����������B
	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		// PeekMessage()�֐��𗘗p���ăE�B���h�E����̃��b�Z�[�W���󂯎��B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			// ���b�Z�[�W����������B
			// ���̊֐��̒��ŃE�B�h�E�N���X�Ŏw�肵�����b�Z�[�W�v���V�[�W���[�֐����Ă΂��B
			DispatchMessage(&msg);
		}
	}

	return 0;
}

