#include "stdafx.h"
#include "sub.h"

// step-3 ���b�Z�[�W�v���V�[�W������������B
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