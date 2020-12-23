#ifndef __DX12_RENDERER_HPP__
#define __DX12_RENDERER_HPP__
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
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		void CreateVertexBuffer();


		float m_aspectRatio;

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