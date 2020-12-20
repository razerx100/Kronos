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

	void DxShader::Initialize() {
		InitializePipeLineState();
	}

	void DxShader::InitializePipeLineState() {
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

		Dx12Context* context = GraphicsContext::GetDirectXContext();

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
}

