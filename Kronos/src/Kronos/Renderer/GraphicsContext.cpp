#include"GraphicsContext.hpp"
#include"Platform/Windows/DirectX12/Dx12Context.hpp"
namespace Kronos {
	GraphicsContext* GraphicsContext::s_Context = nullptr;
	Shader* GraphicsContext::s_shaderAsset = nullptr;

	void GraphicsContext::SwapBuffers() {
		s_Context->swapBuffers();
	}
	Dx12Context* GraphicsContext::GetDirectXContext() {
		return reinterpret_cast<Dx12Context*>(s_Context);
	}
}