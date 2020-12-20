#include "Dx12Renderer.hpp"
#include "Windows/DirectX12/Dx12Context.hpp"
#include "Windows/DirectX12/DxShader.hpp"
#include "Kronos/Renderer/GraphicsContext.hpp"
#include "Windows/DirectX12/DxHelper.hpp"
#include "Kronos/Library/Vertex.hpp"
#include "Kronos/Renderer/Shader.hpp"

#pragma comment(lib, "DXGI.lib") // DXGI Lib link
#pragma comment(lib, "d3d12.lib") // Dx12 Lib link

namespace Kronos {
	Dx12Renderer::Dx12Renderer(unsigned int width, unsigned int height)
		: Renderer(width, height, "DirectX12") {
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	Dx12Renderer::~Dx12Renderer() { }
	void Dx12Renderer::OnInit() {
		GraphicsContext::CreateContext(m_width, m_height);
		GraphicsContext::GetShaderAsset()->Initialize();
		CreateVertexBuffer();
	}
	void Dx12Renderer::OnUpdate() {

	}
	void Dx12Renderer::OnRender() {
		if (GraphicsContext::GetGraphicsContext()) {
			Dx12Context* context = GraphicsContext::GetDirectXContext();
			context->SetPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->SetVertexBufferView(&m_vertexBufferView);
			GraphicsContext::SwapBuffers();
		}
	}
	void Dx12Renderer::OnDestroy() {
        GraphicsContext::CleanUpContext();
	}
	void Dx12Renderer::CreateVertexBuffer() {
		// Create vertex buffer
		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not
		// recommended. Every time the GPU needs it, the upload heap will be marshalled
		// over. Please read up on Default Heap usage. An upload heap is used here for
		// code simplicity and because there are very few verts to actually transfer.
		ThrowIfFailed(
				GraphicsContext::GetDirectXContext()->GetDevice()->CreateCommittedResource(
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
}