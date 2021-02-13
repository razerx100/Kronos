#ifndef __DX12_RENDERER_HPP__
#define __DX12_RENDERER_HPP__
#include <vector>
#include "Kronos/Renderer/Renderer.hpp"
#include "Windows/stdafx.hpp"

namespace Kronos {
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;
	class DxShader;

	class Dx12Renderer : public Renderer {
	private:
		// App resources.
		ComPtr<ID3D12Resource> m_vertexBuffer;
		ComPtr<ID3D12Resource> m_texture;

		// Note: ComPtr's are CPU objects but this resource needs to stay in scope until
		// the command list that references it has finished executing on the GPU.
		// We will flush the GPU at the end of this method to ensure the resource is not
		// prematurely destroyed.
		ComPtr<ID3D12Resource> m_textureUploadHeap;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		void CreateVertexBuffer();
		void CreateTexture();

		std::vector<UINT8> GenerateTextureData();


		float m_aspectRatio;
		static const UINT TextureWidth = 256;
		static const UINT TextureHeight = 256;
		static const UINT TexturePixelSize = 4; // The number of bytes used to represent a pixel in the texture.

	public:
		Dx12Renderer(unsigned int width, unsigned int height);
		~Dx12Renderer();

		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnDestroy() override;
	};
}
#endif