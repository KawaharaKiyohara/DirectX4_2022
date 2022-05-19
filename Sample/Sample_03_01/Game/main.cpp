#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "d3dcompiler.h"
#include "GraphicsEngine.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// step-1 ���[�g�V�O�l�`���̍쐬
	
	// step-2 ���_�V�F�[�_�[�̃��[�h
	
	// step-3 �s�N�Z���V�F�[�_�[�̃��[�h
	
	// step-4 �p�C�v���C���X�e�[�g�̍쐬
	
	// step-5 ���_�o�b�t�@�̍쐬�B
	
	// step-6 �C���f�b�N�X�o�b�t�@�̍쐬
	
	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// �V�[���̕`�揈���J�n�B
			graphicsEngine.BeginRender();

			// step-7 �h���[�R�[�������s����
			
			// �V�[���̕`�揈���I���B
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

