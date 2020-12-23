#ifndef __DX_SHADER_HPP__
#define __DX_SHADER_HPP__
#include "Windows/stdafx.hpp"
#include "Windows/DirectX12/Dx12Structs.hpp"
#include "Kronos/Renderer/Shader.hpp"

namespace Kronos {
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;

	class DxShader : public Shader {
	public:
		DxShader();
		~DxShader();

		void GetAssetsPath() override;
		void Init() override;

	private:
		UINT m_compilerFlags;

		// Shaders
		ComPtr<ID3DBlob> m_vertexShader;
		ComPtr<ID3DBlob> m_pixelShader;

		// Vertex input layout
		const static UINT inputElements = 2;
		D3D12_INPUT_ELEMENT_DESC m_inputElementDescs[inputElements];

		void SetVertexInputLayout();
		void CompileShaders();
	};
}
#endif