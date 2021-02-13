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
	DxShader::DxShader() {
		GetAssetsPath();
	}

	void DxShader::GetAssetsPath() {
		WCHAR assetsPath[512];
		Kronos::GetAssetsPath(assetsPath, _countof(assetsPath));
		m_assetsPath = assetsPath;
	}

	DxShader::~DxShader() { }

	void DxShader::Init() {
#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		m_compilerFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		m_compilerFlags = 0;
#endif
		CompileShaders();
		SetVertexInputLayout();
		GraphicsContext::GetDirectXContext()->CreatePipelineState(
			m_inputElementDescs, inputElements, m_vertexShader, m_pixelShader
		);
	}

	void DxShader::CompileShaders() {
		ThrowIfFailed(
			D3DCompileFromFile(GetAssetFullPath(L"VertexShader.hlsl").c_str(),
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"VertexMainT", "vs_5_0", m_compilerFlags, 0,
				&m_vertexShader, nullptr)
		);
		ThrowIfFailed(
			D3DCompileFromFile(GetAssetFullPath(L"PixelShader.hlsl").c_str(),
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"PixelMainT", "ps_5_0", m_compilerFlags, 0,
				&m_pixelShader, nullptr)
		);
	}
	void DxShader::SetVertexInputLayout() {
		// Define the vertex input layout.
		m_inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		//m_inputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		m_inputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	}
}

