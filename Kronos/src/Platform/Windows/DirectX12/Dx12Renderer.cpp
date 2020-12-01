#include "Dx12Renderer.hpp"
#include "DxHelper.hpp"

Kronos::DxRenderer::DxRenderer(unsigned int width, unsigned int height, std::string title)
	: Renderer(width, height, title) {
	GetAssetsPath();
}

void Kronos::DxRenderer::GetAssetsPath() {
	WCHAR assetsPath[512];
	Kronos::GetAssetsPath(assetsPath, _countof(assetsPath));
	m_assetsPath = assetsPath;
}

Kronos::DxRenderer::~DxRenderer() {
}

void Kronos::DxRenderer::OnInit() {
}

void Kronos::DxRenderer::OnUpdate() {
}

void Kronos::DxRenderer::OnRender() {
}

void Kronos::DxRenderer::OnDestroy() {
}
