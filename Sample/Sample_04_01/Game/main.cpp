#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "d3dcompiler.h"
#include "GraphicsEngine.h"
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteBatch.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/ResourceUploadBatch.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// ���_�V�F�[�_�[�̃��[�h
	ComPtr<ID3DBlob> vsBlob;
	LoadVertexShader(vsBlob);

	// �s�N�Z���V�F�[�_�[�̃��[�h
	ComPtr<ID3DBlob> psBlob;
	LoadPixelShader(psBlob);

	// ���_�o�b�t�@�̍쐬�B
	ComPtr< ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	CreateVertexBuffer(vertexBuffer, vertexBufferView, d3dDevice);

	// �C���f�b�N�X�o�b�t�@�̍쐬�B
	ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	CreateIndexBuffer(indexBuffer, indexBufferView, d3dDevice);

	// step-1 ���[�g�V�O�l�`���̍쐬
	
	// �p�C�v���C���X�e�[�g�̍쐬
	ComPtr< ID3D12PipelineState> pipelineState;
	CreatePipelineState(
		pipelineState, 
		d3dDevice,
		rootSignature,
		vsBlob,
		psBlob );
	
	// step-2 �e�N�X�`�������[�h�B
	
	// step-3 �e�N�X�`���p�̃f�B�X�N���v�^�q�[�v���쐬���āA�f�B�X�N���v�^(���\�[�X���)���������ށB
	
	// step-4 �T���v���X�e�[�g���쐬����B
	
	// step-5 �T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v���쐬���āA�f�B�X�N���v�^(���\�[�X���)���������ށB

	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// �V�[���̕`�揈���J�n�B
			graphicsEngine.BeginRender();

			auto commandList = graphicsEngine.GetCommandList();

			// step-6 ���[�g�V�O�l�`����ݒ�B
			
			// step-7 �f�B�X�N���v�^�q�[�v��ݒ�B

			// step-8 �f�B�X�N���v�^�e�[�u����ݒ�B
			
			// �p�C�v���C���X�e�[�g��ݒ�B
			commandList->SetPipelineState(pipelineState.Get());

			// ���_�o�b�t�@��ݒ�B
			commandList->IASetVertexBuffers(
				0,
				1,
				&vertexBufferView
			);
			// �C���f�b�N�X�o�b�t�@��ݒ�B
			commandList->IASetIndexBuffer(&indexBufferView);
			
			// �h���[�B
			commandList->DrawIndexedInstanced(
				3,	// �C���f�b�N�X�̐�
				1,	// �C���X�^���X�̐�
				0,
				0,
				0
			);
			// �V�[���̕`�揈���I���B
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

