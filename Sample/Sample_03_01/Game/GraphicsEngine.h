#pragma once

#include <Windows.h>
#include "d3dx12.h"

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();
	HRESULT Initialyze(HINSTANCE hInstance, int nCmdShow);
	// １フレームの描画開始時に呼び出す処理。
	void BeginRender();	
	// 1フレームの描画終了時に呼び出す処理。
	void EndRender();
	ComPtr<ID3D12Device5> GetD3DDevice()
	{
		return m_d3dDevice;
	}
	ComPtr< ID3D12GraphicsCommandList4> GetCommandList()
	{
		return m_commandList;
	}
private:
	ComPtr<ID3D12Device5> m_d3dDevice;									// D3Dデバイス。
	ComPtr< ID3D12CommandAllocator > m_commandAllocator;				// コマンドアロケータ。
	ComPtr< ID3D12GraphicsCommandList4> m_commandList;					// コマンドリスト。
	ComPtr<ID3D12CommandQueue> m_commandQueue;							// コマンドキュー。
	UINT m_fenceValue = 0;												// フェンスバリュー。
	ComPtr<ID3D12Fence> m_fence;										// フェンスオブジェクト。
	ComPtr< IDXGISwapChain1> m_swapChain;								// スワップチェイン。
	ComPtr< ID3D12DescriptorHeap> m_frameBufferDescriptorHeap;			// フレームバッファ用のディスクリプタヒープ。
	D3D12_CPU_DESCRIPTOR_HANDLE m_frameBufferDescriptorCPUHandles[2];	// フレームバッファ用のディスクリプタのCPUハンドル。
	ComPtr<ID3D12Resource> m_frameBuffer[2];							// フレームバッファ。
	int m_backBufferNo = 0;												// バックバッファの番号。
	HANDLE m_fenceEventHandle;											// フェンスのイベントハンドル。
	HWND m_hwnd;														// ウィンドウハンドル。
};