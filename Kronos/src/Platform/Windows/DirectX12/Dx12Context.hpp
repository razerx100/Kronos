#ifndef __DX12_CONTEXT_HPP__
#define __DX12_CONTEXT_HPP__
#include "Kronos/Renderer/GraphicsContext.hpp"
#include "Windows/DirectX12/DxShader.hpp"
#include "Windows/stdafx.hpp"
#include "Windows/DirectX12/Dx12Structs.hpp"

namespace Kronos {
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;
	class Dx12Context : public GraphicsContext {
	public:
		Dx12Context(UINT width, UINT height);
		~Dx12Context();

		void CreatePipelineState(
			D3D12_INPUT_ELEMENT_DESC* vertexInputLayout,
			UINT layoutElements,
			ComPtr<ID3DBlob> vertexShader,
			ComPtr<ID3DBlob> pixelShader
		);

		inline void SetVertexBufferView(
			D3D12_VERTEX_BUFFER_VIEW* vertexBufferView
		) { m_vertexBuffferView = vertexBufferView; }
		inline void SetBackgroundColor(XMFLOAT4 color)
		{ m_backgroundColor = color; }
		// Set Background color

		inline ComPtr<ID3D12Device> GetDevice() { return m_device; }

	private:
		static const UINT FrameCount = 3;
		UINT m_width, m_height;
		bool m_useWarpDevice;
		XMFLOAT4 m_backgroundColor;

		// Pipeline objects.
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;
		ComPtr<IDXGISwapChain3> m_swapChain;
		ComPtr<ID3D12Device> m_device;
		ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator> m_commandAllocator[FrameCount];
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		UINT m_rtvDescriptorSize;

		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValues[FrameCount];

		// Vertex buffer objects.
		D3D12_VERTEX_BUFFER_VIEW* m_vertexBuffferView;
		D3D_PRIMITIVE_TOPOLOGY m_primitive;

		void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);

		void GetHardWareAdapter(
			_In_ IDXGIFactory1* pFactory,
			_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter = false
		);
		void LoadPipelineResources();
		void swapBuffers() override;
		void PopulateCommandList();
		void MoveToNextFrame();
		void WaitForGpu();

		void CreateRootSignature();
		void CreateCommandList();
		void CreateSyncObjects();
	};
}
#endif