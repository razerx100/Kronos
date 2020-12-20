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
		void Initialize() override;

	private:
		void InitializePipeLineState();
	};
}
#endif