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

	// ���[�g�V�O�l�`���̍쐬
	enum {
		enDescriptorHeap_SRV,		// �V�F�[�_�[���\�[�X�p�̃f�B�X�N���v�^�q�[�v�B
		enDescriptorHeap_Sampler,	// �T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v
		enDescriptorHeap_Num,		// �f�B�X�N���v�^�q�[�v�̐��B
	};
	// ���[�g�V�O�l�`���ɃA�N�Z�X���Ȃ��Ă����V�F�[�_�[�X�e�[�W��ݒ肷��B
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	// �f�B�X�N���v�^�ƃ��W�X�^�̊��蓖�Ĕ͈͂Ɋւ���f�[�^�B
	// �f�B�X�N���v�^�q�[�v�̂ǂ�����̃f�B�X�N���v�^���ǂ̃��W�X�^���犄�蓖�Ă邩��ݒ肷�邽�߂̂��́B
	CD3DX12_DESCRIPTOR_RANGE1 rangeArray[enDescriptorHeap_Num];
	
	rangeArray[enDescriptorHeap_SRV].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		/*numDescriptors=*/1,
		/*baseShaderRegister=*/0,
		/*registerSpace=*/0,
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC,
		/*offsetInDescriptorsFromTableStart=*/0
	);
	rangeArray[enDescriptorHeap_Sampler].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
		/*numDescriptors=*/1,
		/*baseShaderRegister=*/0,
		/*registerSpace=*/0,
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC,
		/*offsetInDescriptorsFromTableStart=*/0
	);
	// ���[�g�p�����[�^�̊e��p�����[�^�B
	// ���ꂪ���[�g�V�O�l�`���̃f�[�^�ɂȂ�B
	CD3DX12_ROOT_PARAMETER1 rootParameters[enDescriptorHeap_Num];
	
	rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(
		/*numDescritorRange=*/1,
		&rangeArray[enDescriptorHeap_SRV]
	);
	rootParameters[enDescriptorHeap_Sampler].InitAsDescriptorTable(
		/*numDescritorRange=*/1,
		&rangeArray[enDescriptorHeap_Sampler]
	);
	
	// ���[�g�V�O�l�`������邽�߂̃f�[�^��ݒ肷��B
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.Init_1_1(
		/*numParameters=*/enDescriptorHeap_Num,
		rootParameters,
		/*numStaticSamplers=*/0,
		/*_pStaticSamplers=*/nullptr,
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
	
	// �p�C�v���C���X�e�[�g�̍쐬
	ComPtr< ID3D12PipelineState> pipelineState;
	CreatePipelineState(
		pipelineState, 
		d3dDevice,
		rootSignature,
		vsBlob,
		psBlob );
	
	// �e�N�X�`�������[�h�B
	ComPtr<ID3D12Resource> texture;
	DirectX::ResourceUploadBatch re(d3dDevice.Get());
	re.Begin();
	hr = DirectX::CreateDDSTextureFromFileEx(
		d3dDevice.Get(),
		re,
		L"Assets/texture/utc_all2.DDS",
		/*maxSize=*/0,
		D3D12_RESOURCE_FLAG_NONE,
		/*loadFlag=*/0,
		&texture,
		/*alphaMode=*/nullptr,
		/*isCubeMap=*/nullptr
	);
	re.End(graphicsEngine.GetCommandQueue().Get());

	if (FAILED(hr)) {
		//�e�N�X�`���̍쐬�Ɏ��s���܂����B
		MessageBox(
			nullptr,
			L"�e�N�X�`���̍쐬�Ɏ��s�����B",
			L"�G���[",
			MB_OK
		);
	}
	// �e�N�X�`���p�̃f�B�X�N���v�^�q�[�v���쐬���āA�f�B�X�N���v�^(���\�[�X���)���������ށB
	
	// �f�B�X�N���v�^�q�[�v���쐬���邽�߂̃f�[�^��ݒ肷��B
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// �f�B�X�N���v�^�q�[�v���쐬����B
	ComPtr<ID3D12DescriptorHeap> srvDescritorHeap;
	hr = d3dDevice->CreateDescriptorHeap(
		&srvHeapDesc, 
		IID_PPV_ARGS(&srvDescritorHeap)
	);

	// �f�B�X�N���v�^���������ށB
	// �������ݐ��CPU�n���h�����擾�B
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvDescritorHeap->GetCPUDescriptorHandleForHeapStart();
	// �e�N�X�`���̏����擾����B
	D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	d3dDevice->CreateShaderResourceView(
		texture.Get(),	// ���\�[�X�̃A�h���X
		&srvDesc,		// ���\�[�X�̏��
		cpuHandle		// �f�B�X�N���v�^����������CPU�n���h���B
	);

	// �T���v���X�e�[�g���쐬����B
	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

	// �T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v���쐬���āA�f�B�X�N���v�^(���\�[�X���)���������ށB
	D3D12_DESCRIPTOR_HEAP_DESC samplerDescriptorHeapDesc = {};

	samplerDescriptorHeapDesc.NumDescriptors = 1;
	samplerDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// �f�B�X�N���v�^�q�[�v���쐬����B
	ComPtr<ID3D12DescriptorHeap> samplerDescritorHeap;
	hr = d3dDevice->CreateDescriptorHeap(
		&samplerDescriptorHeapDesc,
		IID_PPV_ARGS(&samplerDescritorHeap)
	);
	// �������ݐ��CPU�n���h�����擾�B
	cpuHandle = samplerDescritorHeap->GetCPUDescriptorHandleForHeapStart();
	// �f�B�X�N���v�^���������ށB
	d3dDevice->CreateSampler(
		&samplerDesc,	// �������ރT���v���̏��B
		cpuHandle		// �������ݐ��CPU�n���h���B
	);

	MSG msg = { 0 };
	// �I�����b�Z�[�W�������Ă���܂Ń��[�v���񂷁B
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// �V�[���̕`�揈���J�n�B
			graphicsEngine.BeginRender();

			auto commandList = graphicsEngine.GetCommandList();

			// ���[�g�V�O�l�`����ݒ�B
			commandList->SetGraphicsRootSignature(rootSignature.Get());

			// �f�B�X�N���v�^�q�[�v��ݒ�B
			ID3D12DescriptorHeap* descriptorHeapArray[] = {
				srvDescritorHeap.Get(),
				samplerDescritorHeap.Get()
			};
			commandList->SetDescriptorHeaps(2, descriptorHeapArray);

			// �f�B�X�N���v�^�e�[�u����ݒ�B
			commandList->SetGraphicsRootDescriptorTable(
				enDescriptorHeap_SRV,
				srvDescritorHeap->GetGPUDescriptorHandleForHeapStart()
			);
			commandList->SetGraphicsRootDescriptorTable(
				enDescriptorHeap_Sampler,
				samplerDescritorHeap->GetGPUDescriptorHandleForHeapStart()
			);			
			
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

