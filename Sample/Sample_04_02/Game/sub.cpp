#include "stdafx.h"
#include "sub.h"
#include "d3dcompiler.h"
#include "d3dx12.h"

using namespace Microsoft::WRL;

//メッセージプロシージャを実装する。
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
	case WM_DESTROY:
		//　削除メッセージが来たので終了させる。
		PostQuitMessage(0);
		break;

	default:
		// それ以外はデフォルトの処理に飛ばす。
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ。
		CS_CLASSDC,				// ウィンドウのスタイル。
								// ここの指定でスクロールバーをつけたりできるが、ゲームではほぼ不要なのでCS_CLASSDCでよい。
		MsgProc,				// メッセージプロシージャ。
		0,						// 0でいい。
		0,						// 0でいい。
		GetModuleHandle(nullptr),	// このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								// 何も気にしなくてよい。
		nullptr,				// アイコンのハンドル。今回はnullptrでいい。
		nullptr,				// マウスカーソルのハンドル。今回はnullptrでいい。
		nullptr,				// ウィンドウの背景色。今回はnullptrでいい。
		nullptr,				// メニュー名。今回はnullptrでいい。
		L"MyGame",				// ウィンドウクラスに付ける名前。
		nullptr					// NULLでいい。
	};

	// ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	// ウィンドウの作成
	HWND hwnd = CreateWindow(
		L"MyGame",				// 使用するウィンドウクラスの名前。
								// 先ほど作成したウィンドウクラスと同じ名前にする。
		L"MyGame",				// ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
		0,						// ウィンドウの初期X座標。
		0,						// ウィンドウの初期Y座標。
		1280,					// ウィンドウの幅。
		720,					// ウィンドウの高さ。
		nullptr,    			// 親ウィンドウ。今回はnullptrでいい。
		nullptr,				// メニュー。今回はnullptrでいい。
		hInstance,				// アプリケーションのインスタンス。
		nullptr                 // WM_CREATEメッセージのlParamパラメータとして渡される引数。
	);
	// 作成したウィンドウを表示状態にする。
	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}

HRESULT CreateD3DDevice(ComPtr<ID3D12Device5>& d3dDevice)
{
	HRESULT hr = D3D12CreateDevice(
		nullptr,					// 使用するGPUアダプタを指定する。
									// nullptrを指定するとデフォルトのGPUアダプタが使用される。
									// 例えば「NVIDIA製のGPUが刺さっている場合にそれを最優先で使いたい」
									// といった処理を実装したい場合は、このアダプタを明示的に指定する。
									// パソコンに搭載されいてるGPUアダプタはIDXGIFactory::EnumAdapters()を使えば列挙できる。
		D3D_FEATURE_LEVEL_12_1,		// DirectX12のバージョンを指定する。今回は12.1を指定している。
		IID_PPV_ARGS(&d3dDevice)	// D3Dデバイスにアクセスするインターフェース。
	);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"D3Dデバイスの作成に失敗しました。\n", L"エラー", MB_OK);
	}
	return hr;
}
HRESULT CreateCommandAllocater(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator
)
{
	HRESULT hr = d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドアロケータの作成に失敗しました。\n", L"エラー", MB_OK);
	}

	return hr;
}
HRESULT CreateCommandList(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	const Microsoft::WRL::ComPtr< ID3D12CommandAllocator >& commandAllocator,
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList
)
{
	HRESULT hr = d3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&commandList)
	);
	// コマンドリストをいったん閉じる。
	commandList->Close();
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドリストの作成に失敗しました。\n", L"エラー", MB_OK);
	}
	return hr;
}
HRESULT CreateCommandQueue(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
)
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr = d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr)) {
		MessageBox(nullptr, L"コマンドキューの作成に失敗した。\n", L"エラー", MB_OK);
	}
	return hr;
}
HRESULT CreateFenceEx(
	const Microsoft::WRL::ComPtr<ID3D12Device5>& d3dDevice,
	ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE& feceEventHandle
)
{
	fenceValue = 1;
	HRESULT hr = d3dDevice->CreateFence(
		0,						// 初期値
		D3D12_FENCE_FLAG_NONE,	// フェンスの動作に関するオプション。今回はオプションなし。
		IID_PPV_ARGS(&fence)
	);
	if (FAILED(hr)) {
		// フェンスの作成に失敗した。
		MessageBox(nullptr, L"フェンスの作成に失敗した。\n", L"エラー", MB_OK);
	}
	// 同期を行うためのイベントハンドラを作成する。
	feceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (feceEventHandle == nullptr) {
		// イベントハンドルの作成に失敗した。
		MessageBox(nullptr, L"同期を行うためのイベントハンドルの作成に失敗した。\n", L"エラー", MB_OK);
	}
	return hr;
}

bool ProcessWindowsMessage(MSG& msg)
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// 仮想キーメッセージを文字メッセージに変換する。
		// ユーザーからの文字入力を取得する必要がある場合は、本関数を呼び出す。
		TranslateMessage(&msg);
		// メッセージを処理する。
		// この関数の中でウィドウクラスで指定したメッセージプロシージャー関数が呼ばれる。
		DispatchMessage(&msg);
		// メッセージを処理した。
		return true;
	}
	// もうメッセージがない。
	return false;
}
void SubmitCommandToGPU(
	Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList4>& commandList,
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue
)
{
	// コマンドリストを閉じる。
	commandList->Close();
	// コマンドリストを配列にまとめて、コマンドキューに積んでGPUを走らせる。
	ID3D12CommandList* commandListArray[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandListArray);
}

void WaitUntiFinishedGPUProcessing(
	Microsoft::WRL::ComPtr< ID3D12CommandQueue >& commandQueue,
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence,
	UINT& fenceValue,
	HANDLE fenceEventHandle
)
{
	// フェンスにフェンスバリューを設定する。
	commandQueue->Signal(fence.Get(), fenceValue);
	// フェンスの値がfenceValueになったときにイベントが起きるように設定。
	fence->SetEventOnCompletion(fenceValue, fenceEventHandle);
	// イベントの発生を待つ。
	WaitForSingleObject(fenceEventHandle, INFINITE);
	// フェンスバリューをインクリメント。
	fenceValue++;
}
void LoadVertexShader(ComPtr<ID3DBlob>& vsBlob)
{
	ComPtr<ID3DBlob> errorBlob;
	HRESULT hr = D3DCompileFromFile(
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
			L"頂点シェーダーのロードに失敗した。",
			L"エラー",
			MB_OK
		);
	}
}
void LoadPixelShader(ComPtr<ID3DBlob>& psBlob)
{
	ComPtr<ID3DBlob> errorBlob;
	auto hr = D3DCompileFromFile(
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
			L"ピクセルシェーダーのロードに失敗した。",
			L"エラー",
			MB_OK
		);
	}
}
void CreatePipelineState(
	ComPtr< ID3D12PipelineState>& pipelineState,
	ComPtr<ID3D12Device5> d3dDevice,
	ComPtr< ID3D12RootSignature> rootSignature,
	ComPtr< ID3DBlob>& vsBlob,
	ComPtr< ID3DBlob>& psBlob
)
{
	// まずはパイプラインステートのデータを設定する。
	// 入力頂点定義
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = { 0 };
	// 入力頂点レイアウト
	pipelineStateDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	// ルートシグネチャ
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	// 頂点シェーダー。
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	// ピクセルシェーダー。
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	// ラスタライザステート。
	pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// ブレンドステート。
	pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// デプスステンシルステート。
	pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
	// デフォルトのサンプルマスク
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

	//設定されたデータをもとにパイプラインステートを作成する。
	auto hr = d3dDevice->CreateGraphicsPipelineState(
		&pipelineStateDesc,
		IID_PPV_ARGS(&pipelineState)
	);
	if (FAILED(hr)) {
		MessageBox(
			nullptr,
			L"パイプラインステートの作成に失敗した。",
			L"エラー",
			MB_OK
		);
	}
}
void CreateVertexBuffer(
	ComPtr< ID3D12Resource>& vertexBuffer,
	D3D12_VERTEX_BUFFER_VIEW& vertexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
)
{
	struct Vertex {
		float pos[3];	// 頂点座標
		float uv[2];	// UV座標
	};
	// 頂点配列
	Vertex vertexArray[] = {
		{
			{ -0.3f, -0.3f, 0.0f },
			{ 0.0f, 0.0f }
		},
		{
			{ 0.0f,  0.3f, 0.0f },
			{ 0.0f, 1.0f }
		},
		{
			{ 0.3f, -0.3f, 0.0f },
			{ 1.0f, 1.0f }
		}
	};
	// 頂点配列のサイズを変数に記憶する。
	int vertexArraySize = sizeof(vertexArray);

	// 頂点データを記憶するためのメモリをグラフィックメモリ上に確保する。

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexArraySize);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	// グラフィックメモリにデータをコピーする。
	uint8_t* pData;
	vertexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, vertexArray, vertexArraySize);
	vertexBuffer->Unmap(0, nullptr);

	// 頂点バッファビューを作成。ディスクリプタみたいなもの。
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertexArraySize;
	vertexBufferView.StrideInBytes = sizeof(vertexArray[0]);
}
void CreateIndexBuffer(
	ComPtr<ID3D12Resource>& indexBuffer,
	D3D12_INDEX_BUFFER_VIEW& indexBufferView,
	ComPtr<ID3D12Device5> d3dDevice
)
{
	// インデックスバッファの作成
	// インデックスの配列を定義する。
	int indexArray[] = { 0, 1, 2 };
	// インデックスの配列のサイズを計算する。
	int indexArraySize = sizeof(indexArray);
	// インデックスデータを記憶するためのメモリをグラフィックメモリ上に確保する。
	auto indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexArraySize);
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	// グラフィックメモリにデータをコピーする。
	uint8_t* pData;
	indexBuffer->Map(0, nullptr, (void**)&pData);
	memcpy(pData, indexArray, indexArraySize);
	indexBuffer->Unmap(0, nullptr);

	// インデックスバッファビューを作成。
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = indexArraySize;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}