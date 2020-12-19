#ifndef __DX12_RENDERER_HPP__
#define __DX12_RENDERER_HPP__
#include "Windows/stdafx.hpp"
#include "Windows/DirectX12/Dx12Structs.hpp"

namespace Kronos {
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;

	class DxShader {
		public:
		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		DxShader(unsigned int width, unsigned int height);
		~DxShader();

		void LoadAssets();

		void GetAssetsPath();

		inline std::wstring GetAssetFullPath(wchar_t* assetName) {
			return m_assetsPath + assetName;
		}
		void RecordCommands(
			ComPtr<ID3D12GraphicsCommandList> commandList,
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle
			);
	private:
		std::wstring m_assetsPath;
		float m_aspectRatio;

		// App resources.
		ComPtr<ID3D12Resource> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	};
}
#endif