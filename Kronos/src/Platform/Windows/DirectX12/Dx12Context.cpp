#include "Dx12Context.hpp"
#include "DxHelper.hpp"
#include "Kronos/Application.hpp"
#include "Windows/WindowsWindow.hpp"
#include "DxShader.hpp"
#include "Kronos/Library/Color.hpp"

#pragma comment(lib, "DXGI.lib") // DXGI Lib link
#pragma comment(lib, "d3d12.lib") // Dx12 Lib link


namespace Kronos {
	Dx12Context::Dx12Context(UINT width, UINT height)
		: m_width(width), m_height(height),
		m_frameIndex(0),
		m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
		m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
		m_rtvDescriptorSize(0), m_vertexBuffferView(nullptr),
		m_backgroundColor{0.1f, 0.1f, 0.1f, 1.0f},
		m_primitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		LoadPipelineResources();
		CreateRootSignature();
		CreateCommandList();
		CreateSyncObjects();
	}
	Dx12Context::~Dx12Context() {
		// Ensure that the GPU is no longer referencing resources that are about to be
		// cleaned up by the destructor.
		WaitForPreviousFrame();

		CloseHandle(m_fenceEvent);
	}
	void Dx12Context::swapBuffers() {
		//Record all the commands we need to render the scene into command list.
		PopulateCommandList();

		// Execute the command list.
		ID3D12CommandList* ppCommandLists[] = {m_commandList.Get()};
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame
		ThrowIfFailed(m_swapChain->Present(1, 0));

		WaitForPreviousFrame();
	}
	void GraphicsContext::CreateContext(unsigned int width, unsigned int height) {
		s_Context = new Dx12Context(width, height);
		s_shaderAsset = new DxShader();
	}


	_Use_decl_annotations_
	void Dx12Context::GetHardWareAdapter(
			IDXGIFactory1* pFactory,
			IDXGIAdapter1** ppAdapter,
			bool requestHighPerformanceAdapter) {
		*ppAdapter = nullptr;

		ComPtr<IDXGIAdapter1> adapter;

		ComPtr<IDXGIFactory6> factory6;
		if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
			for (
				UINT adapterIndex = 0;
				DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
					adapterIndex,
					requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
					IID_PPV_ARGS(&adapter)
				);
				++adapterIndex
				) {
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					// Don't select the Basic Render Driver adapter.
					// If you want a software adapter, pass in "/warp" on the command line.
					continue;

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
					break;
			}
		}
		else {
			for (
				UINT adapterIndex = 0;
				DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter);
				++adapterIndex
				) {
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					// Don't select the Basic Render Driver adapter.
					// If you want a software adapter, pass in "/warp" on the command line.
					continue;

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
					break;
			}
		}

		*ppAdapter = adapter.Detach();
	}
	void Dx12Context::LoadPipelineResources() {
		UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
		// Enable the debug layer (requires the graphics tools "optional features").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
				debugController->EnableDebugLayer();

				// Enable additional layers
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif
		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

		if (m_useWarpDevice) {
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

			ThrowIfFailed(
				D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_device)
				)
			);
		}
		else {
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardWareAdapter(factory.Get(), &hardwareAdapter);

			ThrowIfFailed(
				D3D12CreateDevice(
					hardwareAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_device)
				)
			);
		}

		// Describe and create the command queue.
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

		// Describe and create swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = FrameCount;
		swapChainDesc.Width = m_width;
		swapChainDesc.Height = m_height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		HWND hwnd = WindowsWindow::GetHWND();

		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(
			factory->CreateSwapChainForHwnd(
				m_commandQueue.Get(),
				hwnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				&swapChain
			)
		);

		// Doesn't support full screen transitions.
		ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(swapChain.As(&m_swapChain));
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		// Create descriptor heaps.
		{
			// Describe and create a render target view (RTV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

			m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		// Create frame resources.
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each frame.
			for (UINT n = 0; n < FrameCount; n++) {
				ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
				m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_rtvDescriptorSize);
			}
		}

		ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
	}
	void Dx12Context::WaitForPreviousFrame() {
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
		// sample illustrates how to use fences for efficient resource usage and to
		// maximize GPU utilization.

		// Signal and increment the fence value.
		const UINT64 fence = m_fenceValue;
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
		m_fenceValue++;

		// Wait untill the previous frame is finished.
		if (m_fence->GetCompletedValue() < fence) {
			ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	}
	void Dx12Context::PopulateCommandList() {
		// Command list allocators can only be reset when the associated
		// command lists have finished execution on the GPU; apps should use
		// fences to determine GPU execution progress.
		ThrowIfFailed(m_commandAllocator->Reset());

		// However, when ExecuteCommandList() is called on a particular command
		// list, that command list can then be reset at any time and must be before
		// re-recording
		ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

		// Set necessary state.
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate that the back buffer will be used as a render target.
		m_commandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				m_renderTargets[m_frameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET
				)
		);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_frameIndex, m_rtvDescriptorSize
		);
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		// Record Commands
		m_commandList->ClearRenderTargetView(
			rtvHandle, reinterpret_cast<float*>(&m_backgroundColor), 0, nullptr
		);
		m_commandList->IASetPrimitiveTopology(m_primitive);
		m_commandList->IASetVertexBuffers(0, 1, m_vertexBuffferView);
		m_commandList->DrawInstanced(3, 1, 0, 0);
		// Indicate that the back buffer will now be used to present.
		m_commandList->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(
					m_renderTargets[m_frameIndex].Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PRESENT
				)
			);

		ThrowIfFailed(m_commandList->Close());
	}
	void Dx12Context::CreateRootSignature() {
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(
			&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature, &error
		));
		ThrowIfFailed(m_device->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&m_rootSignature)
		));
	}
	void Dx12Context::CreateCommandList() {
		ThrowIfFailed(m_device->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_commandAllocator.Get(), m_pipelineState.Get(),
			IID_PPV_ARGS(&m_commandList)
		));

		// Command lists are created in the recording state, but there is nothing to
		// record yet. The main loop expects it to be closed, so close it now.
		ThrowIfFailed(m_commandList->Close());
	}
	void Dx12Context::CreateSyncObjects() {
		ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValue = 1;

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));

		// Wait for the command list to execute; we are reusing the same command
		// list in our main loop but for now, we just want to wait for setup to
		// complete before continuing.
		WaitForPreviousFrame();
	}
	void Dx12Context::CreatePipelineState(
		D3D12_INPUT_ELEMENT_DESC* vertexInputLayout,
		UINT layoutElements,
		ComPtr<ID3DBlob> vertexShader,
		ComPtr<ID3DBlob> pixelShader
	) {
		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { vertexInputLayout, layoutElements };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
	}
	_Use_decl_annotations_
	void Dx12Context::ParseCommandLineArgs(WCHAR* argv[], int argc) {
		for (int i = 0; i < argc; ++i) {
			if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
				_wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0) {
				m_useWarpDevice = true;
			}
		}
	}
}