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
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GraphicsEngine graphicsEngine;
	graphicsEngine.Initialyze(hInstance, nCmdShow);
	auto d3dDevice = graphicsEngine.GetD3DDevice();
	
	// 頂点シェーダーのロード
	ComPtr<ID3DBlob> vsBlob;
	LoadVertexShader(vsBlob);

	// ピクセルシェーダーのロード
	ComPtr<ID3DBlob> psBlob;
	LoadPixelShader(psBlob);

	// 頂点バッファの作成。
	ComPtr< ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	CreateVertexBuffer(vertexBuffer, vertexBufferView, d3dDevice);

	// インデックスバッファの作成。
	ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	CreateIndexBuffer(indexBuffer, indexBufferView, d3dDevice);

	// step-1 ルートシグネチャの作成
	enum {
		enDescriptorHeap_CB,	// 定数バッファ用のディスクリプタヒープ。
		enDescriptorHeap_SRV,	// シェーダーリソース用のディスクリプタヒープ。
		enDescriptorHeap_Num,	// ディスクリプタヒープの数。
	};
	// ルートシグネチャにアクセスしなくていいシェーダーステージを設定する。
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	// ディスクリプタとレジスタの割り当て範囲に関するデータ。
	// ディスクリプタヒープのどこからのディスクリプタをどのレジスタから割り当てるかを設定するためのもの。
	CD3DX12_DESCRIPTOR_RANGE1 rangeArray[enDescriptorHeap_Num];
	rangeArray[enDescriptorHeap_CB].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
		/*numDescriptors=*/1,						// ディスクリプタはいくつ？
		/*baseShaderRegister=*/0,					// どのレジスタ番号から割り当てスタートする？
		/*registerSpace=*/0,						// 通常０でいい。
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC,
		/*offsetInDescriptorsFromTableStart=*/0		// ディスクリプタの開始位置のオフセット。
	);
	rangeArray[enDescriptorHeap_SRV].Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		/*numDescriptors=*/1,
		/*baseShaderRegister=*/0,
		/*registerSpace=*/0,
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC,
		/*offsetInDescriptorsFromTableStart=*/0
	);
	// ルートパラメータの各種パラメータ。
	// これがルートシグネチャのデータになる。
	CD3DX12_ROOT_PARAMETER1 rootParameters[enDescriptorHeap_Num];
	// パラメータ0番目は定数バッファ用のディスクリプタテーブルとして初期化する。
	rootParameters[enDescriptorHeap_CB].InitAsDescriptorTable(
		/*numDescritorRange=*/1,
		&rangeArray[enDescriptorHeap_CB]
	);
	rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(
		/*numDescritorRange=*/1,
		&rangeArray[enDescriptorHeap_SRV]
	);
	
	// ルートシグネチャを作るためのデータを設定する。
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.Init_1_1(
		/*numParameters=*/2,
		rootParameters,
		/*numStaticSamplers=*/0,
		/*_pStaticSamplers=*/nullptr,
		rootSignatureFlags
	);
	// 定義されたルートシグネチャを作成するためのメモリを構築する。
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	D3DX12SerializeVersionedRootSignature(
		&rootDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1, 
		&signature, 
		&error
	);
	// ルートシグネチャを作成する。
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
			L"ルートシグネチャの作成に失敗した。",
			L"エラー",
			MB_OK
		);
	}
	
	// パイプラインステートの作成
	ComPtr< ID3D12PipelineState> pipelineState;
	CreatePipelineState(
		pipelineState, 
		d3dDevice,
		rootSignature,
		vsBlob,
		psBlob );
	
	// step-2 テクスチャをロード。
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
		//テクスチャの作成に失敗しました。
		MessageBox(
			nullptr,
			L"テクスチャの作成に失敗した。",
			L"エラー",
			MB_OK
		);
	}
	// step-3 ディスクリプタヒープを作成して、ディスクリプタ(リソース情報)を書き込むする。
	
	// ディスクリプタヒープを作成するためのデータを設定する。
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// ディスクリプタヒープを作成する。
	ComPtr<ID3D12DescriptorHeap> descritorHeap;
	hr = d3dDevice->CreateDescriptorHeap(
		&srvHeapDesc, 
		IID_PPV_ARGS(&descritorHeap)
	);

	// ディスクリプタを書き込む。
	// 書き込み先のCPUハンドルを取得。
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = descritorHeap->GetCPUDescriptorHandleForHeapStart();
	// テクスチャの情報を取得する。
	D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	d3dDevice->CreateShaderResourceView(
		texture.Get(),	// リソースのアドレス
		&srvDesc,		// リソースの情報
		cpuHandle		// ディスクリプタを書き込むCPUハンドル。
	);

	
	MSG msg = { 0 };
	// 終了メッセージが送られてくるまでループを回す。
	while (WM_QUIT != msg.message) {
		
		if(ProcessWindowsMessage(msg) == false){
			// シーンの描画処理開始。
			graphicsEngine.BeginRender();
			// step-7 ドローコールを実行する
			auto commandList = graphicsEngine.GetCommandList();
			// ルートシグネチャを設定。
			commandList->SetGraphicsRootSignature(rootSignature.Get());
			// パイプラインステートを設定。
			commandList->SetPipelineState(pipelineState.Get());
			// 頂点バッファを設定。
			commandList->IASetVertexBuffers(
				0,
				1,
				&vertexBufferView
			);
			// インデックスバッファを設定。
			commandList->IASetIndexBuffer(&indexBufferView);
			// ディスクリプタヒープを設定。
			ID3D12DescriptorHeap* descriptorHeapArray[] = {
				descritorHeap.Get()
			};
			commandList->SetDescriptorHeaps(1, descriptorHeapArray);
			// ディスクリプタテーブルを設定。
			// ドロー。
			commandList->DrawIndexedInstanced(
				3,	// インデックスの数
				1,	// インスタンスの数
				0,
				0,
				0
			);
			// シーンの描画処理終了。
			graphicsEngine.EndRender();
		}
	}

	return 0;
}

