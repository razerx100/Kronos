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
	private:
		static const UINT FrameCount = 3;
		UINT m_width, m_height;
		bool m_useWarpDevice;

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

		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;

		DxShader* shaderAsset;


		void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);

		void GetHardWareAdapter(
			_In_ IDXGIFactory1* pFactory,
			_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter = false
		);
		void LoadPipeline();
		void swapBuffers() override;
		void PopulateCommandList();
		void WaitForPreviousFrame();
	public:
		Dx12Context(UINT width, UINT height);
		~Dx12Context();

		void CreateRootSignature();
		void CreateCommandList();
		void CreateSyncObjects();
		void CreatePipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc);

		inline DxShader* GetShaderObject() { return shaderAsset; }
		inline ComPtr<ID3D12Device> GetDevice() { return m_device; }
		inline ComPtr<ID3D12RootSignature> GetRootSignature() { return m_rootSignature; }
	};
}
#endif