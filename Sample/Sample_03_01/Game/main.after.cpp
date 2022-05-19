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
	// ���[�g�V�O�l�`���ɃA�N�Z�X���Ȃ��Ă����V�F�[�_�[�X�e�[�W��ݒ肷��B
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	// ���[�g�V�O�l�`���̃f�[�^��ݒ肷��B
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.Init_1_1(
		0,
		nullptr,
		0,
		nullptr,
		rootSignatureFlags
	);
	// ��`���ꂽ���[�g�V�O�l�`�����쐬���邽�߂̃��������\�z����B
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	D3DX12SerializeVersionedRootSignature(
		&rootDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		&signature, 
		&error
	);
	// ���[�g�V�O�l�`�����쐬����B
	ComPtr< ID3D12RootSignature> rootSignature;
	auto hr = d3dDevice->CreateRootSignature(
		0, 
		signature->GetBufferPointer(), 
		signature->GetBufferSize(), 
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"���[�g�V�O�l�`���̍쐬�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
	// step-2 ���_�V�F�[�_�[�̃��[�h
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx", 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"VSMain", 
		"vs_5_0", 
		D3DCOMPILE_DEBUG,
		0, 
		&vsBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"���_�V�F�[�_�[�̃��[�h�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
	// step-3 �s�N�Z���V�F�[�_�[�̃��[�h
	ComPtr<ID3DBlob> psBlob;
	hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain",
		"ps_5_0",
		0,
		0,
		&psBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"�s�N�Z���V�F�[�_�[�̃��[�h�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
	// step-4 �p�C�v���C���X�e�[�g�̍쐬
	// �܂��̓p�C�v���C���X�e�[�g�̃f�[�^��ݒ肷��B
	// ���͒��_��`
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {0};
	// ���͒��_���C�A�E�g
	pipelineStateDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	// ���[�g�V�O�l�`��
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	// ���_�V�F�[�_�[�B
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	// �s�N�Z���V�F�[�_�[�B
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	// ���X�^���C�U�X�e�[�g�B
	pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �u�����h�X�e�[�g�B
	pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g�B
	pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
	// �f�t�H���g�̃T���v���}�X�N
	pipelineStateDesc.SampleMask = UINT_MAX;
	// �v���~�e�B�u�g�|���W�[�B
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
	pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����_�����O�^�[�Q�b�g�̖����B
	pipelineStateDesc.NumRenderTargets = 1;
	// �f�v�X�X�e���V���t�H�[�}�b�g�B
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	// MASS�̐ݒ�B
	pipelineStateDesc.SampleDesc.Count = 1;

	//�ݒ肳�ꂽ�f�[�^�����ƂɃp�C�v���C���X�e�[�g���쐬����B
	ComPtr< ID3D12PipelineState> pipelineState;
	hr = d3dDevice->CreateGraphicsPipelineState(
		&pipelineStateDesc,
		IID_PPV_ARGS(&pipelineState)
	);
	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
	// step-5 ���_�o�b�t�@�̍쐬�B
	struct Vertex {
		float pos[3];	// ���_���W
	};
	// ���_�z��
	Vertex vertexArray[] = {
		{ -1.0f, -1.0f, 0.0f },
		{  0.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },
	};
	// ���_�z��̃T�C�Y��ϐ��ɋL������B
	int vertexArraySize = sizeof(vertexArray);

	// ���_�f�[�^���L�����邽�߂̃��������O���t�B�b�N��������Ɋm�ۂ���B
	ComPtr< ID3D12Resource> vertexBuffer;
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexArraySize);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	// �O���t�B�b�N�������Ƀf�[�^���R�s�[����B
	uint8_t* pData;
	vertexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, vertexArray, vertexArraySize);
	vertexBuffer->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[���쐬�B�f�B�X�N���v�^�݂����Ȃ��́B
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertexArraySize;
	vertexBufferView.StrideInBytes = sizeof(vertexArray[0]);


	// step-6 �C���f�b�N�X�o�b�t�@�̍쐬
	// �C���f�b�N�X�̔z����`����B
	int indexArray[] = { 0, 1, 2 };
	// �C���f�b�N�X�̔z��̃T�C�Y���v�Z����B
	int indexArraySize = sizeof(indexArray);
	// �C���f�b�N�X�f�[�^���L�����邽�߂̃��������O���t�B�b�N��������Ɋm�ۂ���B
	ComPtr< ID3D12Resource> indexBuffer;
	auto indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexArraySize);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	// �O���t�B�b�N�������Ƀf�[�^���R�s�[����B
	pData;
	indexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, indexArray, indexArraySize);
	indexBuffer->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[���쐬�B
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = indexArraySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// �V�[���̕`�揈���J�n�B
			graphicsEngine.BeginRender();
			// step-7 �h���[�R�[�������s����
			auto commandList = graphicsEngine.GetCommandList();
			// ���[�g�V�O�l�`����ݒ�B
			commandList->SetGraphicsRootSignature(rootSignature.Get());
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

