#include "Dx12Renderer.hpp"
#include "Windows/DirectX12/Dx12Context.hpp"
#include "Windows/DirectX12/DxShader.hpp"
#include "Kronos/Renderer/GraphicsContext.hpp"
#include "Windows/DirectX12/DxHelper.hpp"
#include "Kronos/Renderer/Shader.hpp"
#include "Windows/WindowsWindow.hpp"
#include "Kronos/Library/Color.hpp"

#pragma comment(lib, "DXGI.lib") // DXGI Lib link
#pragma comment(lib, "d3d12.lib") // Dx12 Lib link

namespace Kronos {
	Dx12Renderer::Dx12Renderer(unsigned int width, unsigned int height)
		: Renderer(width, height, "DirectX12") {
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		m_triangleVertices.point1 = {
			{ 0.15f, 0.50f * m_aspectRatio, 0.0f }, { 0.5f, 0.0f }
		};
		m_triangleVertices.point2 = {
			{ 0.65f, 0.50f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f }
		};
		m_triangleVertices.point3 = {
			{ 0.65f, 0.00f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f }
		};
	}
	Renderer* Renderer::Create(unsigned int width, unsigned int height) {
		return new Dx12Renderer(width, height);
	}
	Dx12Renderer::~Dx12Renderer() { }
	void Dx12Renderer::OnInit() {
		GraphicsContext::CreateContext(m_width, m_height);
		GraphicsContext::GetShaderAsset().Init();
		CreateVertexBuffer();
		CreateTexture();
		GraphicsContext::GetDirectXContext()->BeginInitialGPUSetup();
	}
	void Dx12Renderer::OnUpdate() {

	}
	void Dx12Renderer::OnRender() {
		if (GraphicsContext::GetGraphicsContext()) {
			GraphicsContext::GetDirectXContext()->SetVertexBufferView(
				&m_vertexBufferView
			);
			GraphicsContext::SwapBuffers();
		}
	}
	void Dx12Renderer::OnDestroy() {
        GraphicsContext::CleanUpContext();
	}
	void Dx12Renderer::CreateVertexBuffer() {
		// Create vertex buffer
		// Define the geometry for a triangle.
		/*Vertex triangleVertices[] =
		{
			{ { 0.4f, 0.50f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.65f, 0.00f * m_aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.15f, 0.00f * m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};*/

		const UINT vertexBufferSize = sizeof(m_triangleVertices);

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
		memcpy(pVertexDataBegin, &m_triangleVertices, sizeof(m_triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(VertexT);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}

	// Generate a simple black and white checkboard texture.
	std::vector<UINT8> Dx12Renderer::GenerateTextureData() {
		const UINT rowPitch = TextureWidth * TexturePixelSize;
		const UINT cellPitch = rowPitch >> 3; // The width of a cell in the checkboard texture.
		const UINT cellHeight = TextureWidth >> 3; // The height of a cell in the checkboard texture.
		const UINT textureSize = rowPitch * TextureHeight;

		std::vector<UINT8> data(textureSize);
		UINT8* pData = &data[0];

		for (UINT n = 0; n < textureSize; n += TexturePixelSize) {
			UINT x = n % rowPitch;
			UINT y = n / rowPitch;
			UINT i = x / cellPitch;
			UINT j = y / cellHeight;

			if (i % 2 == j % 2) {
				pData[n] = 0x00;		// R
				pData[n + 1] = 0x00;	// G
				pData[n + 2] = 0x00;	// B
				pData[n + 3] = 0xff;	// A
			}
			else {
				pData[n] = 0xff;		// R
				pData[n + 1] = 0xff;	// G
				pData[n + 2] = 0xff;	// B
				pData[n + 3] = 0xff;	// A
			}
		}

		return data;
	}

	void Dx12Renderer::CreateTexture() {
		// Describe and create a Texture2D.
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = TextureWidth;
		textureDesc.Height = TextureHeight;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		ThrowIfFailed(
			GraphicsContext::GetDirectXContext()->GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&textureDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_texture)
			)
		);

		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

		// Create the GPU upload buffer.
		ThrowIfFailed(
			GraphicsContext::GetDirectXContext()->GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_textureUploadHeap)
			)
		);

		// Copy data to the intermediate upload heap and then schedule a copy
		// from the upload heap to Texture2D.
		std::vector<UINT8> texture = GenerateTextureData();

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &texture[0];
		textureData.RowPitch = TextureWidth * TexturePixelSize;
		textureData.SlicePitch = textureData.RowPitch * TextureHeight;

		UpdateSubresources(
			GraphicsContext::GetDirectXContext()->GetCommandList().Get(),
			m_texture.Get(),
			m_textureUploadHeap.Get(),
			0, 0, 1,
			&textureData
		);
		GraphicsContext::GetDirectXContext()->GetCommandList()->ResourceBarrier(
			1, &CD3DX12_RESOURCE_BARRIER::Transition(
				m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
				)
		);

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		GraphicsContext::GetDirectXContext()->GetDevice()->CreateShaderResourceView(
			m_texture.Get(), &srvDesc,
			GraphicsContext::GetDirectXContext()->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
	}
}