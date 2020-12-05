#ifndef __DX12_RENDERER_HPP__
#define __DX12_RENDERER_HPP__
#include "Kronos/Renderer.hpp"
#include "Windows/stdafx.hpp"
#include "Windows/DirectX12/Dx12Structs.hpp"

namespace Kronos {
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;

	class DxRenderer : public Renderer {
	private:
		bool m_useWarpDevice;

		void GetHardWareAdapter(
			_In_ IDXGIFactory1* pFactory,
			_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter = false
		);

	public:
		DxRenderer(unsigned int width, unsigned int height, std::string title);
		~DxRenderer();

		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnDestroy() override;

		void GetAssetsPath() override;

		void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);

	private:
		static const UINT FrameCount = 2;

		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		// Pipeline objects.
		CD3DX12_VIEWPORT m_viewport;
		CD3DX12_RECT m_scissorRect;
		ComPtr<IDXGISwapChain3> m_swapChain;
		ComPtr<ID3D12Device> m_device;
		ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		UINT m_rtvDescriptorSize;

		// App resources.
		ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;

		void LoadPipeline();
		void LoadAssets();
		void PopulateCommandList();
		void WaitForPreviousFrame();
	};
}
#endif