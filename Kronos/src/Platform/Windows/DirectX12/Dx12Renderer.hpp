#ifndef __DX12_RENDERER_HPP__
#define __DX12_RENDERER_HPP__
#include "Kronos/Renderer.hpp"
#include "Windows/stdafx.hpp"
namespace Kronos {
	class DxRenderer : public Renderer {
	public:
		DxRenderer(unsigned int width, unsigned int height, std::string title);
		~DxRenderer();

		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnDestroy() override;

		void GetAssetsPath() override;
	};
}
#endif