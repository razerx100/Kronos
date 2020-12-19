#include "DxShader.hpp"
#include "DxHelper.hpp"
#include "Kronos/Application.hpp"
#include "Windows/WindowsWindow.hpp"
#include "Windows/DirectX12/Dx12Context.hpp"
#include "Kronos/Renderer/GraphicsContext.hpp"

#pragma comment(lib, "DXGI.lib") // DXGI Lib link
#pragma comment(lib, "d3d12.lib") // Dx12 Lib link
#pragma comment(lib, "d3dcompiler.lib") // DxCompiler Lib link

namespace Kronos {
	DxShader::DxShader(unsigned int width, unsigned int height) {
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		GetAssetsPath();
	}

	void DxShader::GetAssetsPath() {
		WCHAR assetsPath[512];
		Kronos::GetAssetsPath(assetsPath, _countof(assetsPath));
		m_assetsPath = assetsPath;
	}

	void DxShader::RecordCommands(
		ComPtr<ID3D12GraphicsCommandList> commandList,
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle) {
		const float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		commandList->DrawInstanced(3, 1, 0, 0);
	}

	DxShader::~DxShader() {
	}

	// Load sample assets
	void DxShader::LoadAssets() {
		// Create an empty root signature.
		Dx12Context* context = GraphicsContext::GetDirectXContext();
		context->CreateRootSignature();

		// Create the pipeline state, which includes compiling and loading shaders.
		{
			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
			// Enable better shader debugging with the graphics debugging tools.
			UINT compilerFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			UINT compilerFlags = 0;
#endif

			ThrowIfFailed(
				D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(),
				nullptr, nullptr,
				"VSMain", "vs_5_0", compilerFlags, 0,
				&vertexShader, nullptr)
			);
			ThrowIfFailed(
				D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(),
				nullptr, nullptr,
				"PSMain", "ps_5_0", compilerFlags, 0,
				&pixelShader, nullptr)
			);

			// Define the vertex input layout.
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
			};

			// Describe and create the graphics pipeline state object (PSO).
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = context->GetRootSignature().Get();
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
			// Create pso
			context->CreatePipelineState(&psoDesc);
		}

		// Create the command list.
		context->CreateCommandList();

		// Create the vertex buffer.
		{
			// Define the geometry for a triangle.
			Vertex triangleVertices[] =
			{
				{ { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
				{ { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
			};

			const UINT vertexBufferSize = sizeof(triangleVertices);

			// Note: using upload heaps to transfer static data like vert buffers is not
			// recommended. Every time the GPU needs it, the upload heap will be marshalled
			// over. Please read up on Default Heap usage. An upload heap is used here for
			// code simplicity and because there are very few verts to actually transfer.
			ThrowIfFailed(
				context->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_vertexBuffer)
				)
			);

			// Copy the triangle data to the vertex buffer.
			UINT8* pVertexDataBegin;
			CD3DX12_RANGE readRange(0, 0);			// We dont intend to read from this resource on the CPU.
			ThrowIfFailed(m_vertexBuffer->Map(
				0, &readRange,
				reinterpret_cast<void**>(&pVertexDataBegin)
			));
			memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
			m_vertexBuffer->Unmap(0, nullptr);

			// Initialize the vertex buffer view.
			m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			m_vertexBufferView.StrideInBytes = sizeof(Vertex);
			m_vertexBufferView.SizeInBytes = vertexBufferSize;
		}

		// Create synchronization objects and wait untill assets have been uploaded to the GPU.
		context->CreateSyncObjects();
	}
}

