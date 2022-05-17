#include "stdafx.h"
#include <Windows.h>
#include "sub.h"
#include "d3dx12.h"
#include "d3dcompiler.h"
#include "GraphicsEngine.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// ルートシグネチャの作成。

	// ルートシグネチャにアクセスしなくていいシェーダーステージを設定する。
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	ComPtr< ID3D12RootSignature> rootSignature;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.Init_1_1(
		/*numParameters=*/0,
		/*_pParameters=*/nullptr,
		0,
		nullptr,
		rootSignatureFlags
	);
	// ルートシグネチャを作成する。
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	D3DX12SerializeVersionedRootSignature(
		&rootDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		&signature, 
		&error
	);
	auto hr = d3dDevice->CreateRootSignature(
		0, 
		signature->GetBufferPointer(), 
		signature->GetBufferSize(), 
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr)) {
		MessageBox(
			/*hwnd=*/nullptr,
			L"ルートシグネチャの作成に失敗した。",
			L"エラー",
			MB_OK
		);
	}
	// 頂点シェーダーのロード。
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx", 
		/*pInclude=*/nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		/*pEntrypoint=*/"VSMain", 
		/*pTarget=*/"vs_5_0", 
		/*Flags1=*/D3DCOMPILE_DEBUG,
		/*Flags2=*/0, 
		&vsBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			/*hwnd=*/nullptr,
			L"頂点シェーダーのロードに失敗した。",
			L"エラー",
			MB_OK
		);
	}
	// ピクセルシェーダーのロード。
	ComPtr<ID3DBlob> psBlob;
	hr = D3DCompileFromFile(
		L"Assets/shader/sample.fx",
		/*pInclude=*/nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		/*pEntrypoint=*/"PSMain",
		/*pTarget=*/"ps_5_0",
		/*Flags1=*/D3DCOMPILE_DEBUG,
		/*Flags2=*/0,
		&psBlob,
		&errorBlob);

	if (FAILED(hr)) {
		MessageBox(
			/*hwnd=*/nullptr,
			L"ピクセルシェーダーのロードに失敗した。",
			L"エラー",
			MB_OK
		);
	}
	// パイプラインステートの作成。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {0};
	pipelineStateDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	// ラスタライザステート。
	pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// ブレンドステート。
	pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// デプスステンシルステート。
	pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
	// 
	pipelineStateDesc.SampleMask = UINT_MAX;
	// プリミティブトポロジー。
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// レンダリングターゲットのフォーマット。
	pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// レンダリングターゲットの枚数。
	pipelineStateDesc.NumRenderTargets = 1;
	// デプスステンシルフォーマット。
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	// MASSの設定。
	pipelineStateDesc.SampleDesc.Count = 1;

	ComPtr< ID3D12PipelineState> pipelineState;
	hr = d3dDevice->CreateGraphicsPipelineState(
		&pipelineStateDesc,
		IID_PPV_ARGS(&pipelineState)
	);
	if (FAILED(hr)) {
		MessageBox(
			/*hwnd=*/nullptr,
			L"パイプラインステートの作成に失敗した。",
			L"エラー",
			MB_OK
		);
	}
	// 頂点バッファの作成。
	struct Vertex {
		float pos[3];	// 頂点座標
	};
	// 頂点配列
	Vertex vertexArray[] = {
		{ -1.0f, -1.0f, 0.0f },
		{  0.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },
	};
	int vertexArraySize = sizeof(vertexArray);
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
	// データをコピー。
	uint8_t* pData;
	vertexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, vertexArray, vertexArraySize);
	vertexBuffer->Unmap(0, nullptr);
	// 頂点バッファビューを作成。
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertexArraySize;
	vertexBufferView.StrideInBytes = sizeof(vertexArray[0]);


	// インデックスバッファの作成。
	int indexArray[] = { 0, 1, 2 };
	int indexArraySize = sizeof(indexArray);
	ComPtr< ID3D12Resource> indexBuffer;
	auto indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexArraySize);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));
	// データをコピー。
	pData;
	indexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, indexArray, indexArraySize);
	indexBuffer->Unmap(0, nullptr);

	// インデックスバッファビューを作成。
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = indexArraySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// シーンの描画処理開始。
			graphicsEngine.BeginRender();

			auto commandList = graphicsEngine.GetCommandList();
			// ルートシグネチャを設定。
			commandList->SetGraphicsRootSignature(rootSignature.Get());
			// パイプラインステートを設定。
			commandList->SetPipelineState(pipelineState.Get());
			// 頂点バッファを設定。
			commandList->IASetVertexBuffers(
				/*StartSlot=*/0,
				/*NumViews=*/1,
				&vertexBufferView);
			// インデックスバッファを設定。
			commandList->IASetIndexBuffer(
				&indexBufferView);

			// ドロー。
			commandList->DrawIndexedInstanced(
				/*IndexCountPerInstance=*/3,
				/*InstanceCount=*/1,
				/*StartIndexLocation=*/0,
				/*BaseVertexLocation=*/0,
				/*StartInstanceLocation=*/0
			);
			// シーンの描画処理終了。
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

