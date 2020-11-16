#include"ImGuiLayer.hpp"
#include"imgui.h"
#include"Kronos/Application.hpp"
#include"Platform/Windows/WindowsWindow.hpp"
#include"Platform/DirectX12/imgui_impl_win32.hpp"
#include"Platform/DirectX12/imgui_impl_dx12.hpp"

namespace Kronos {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer"), g_frameContext{}, g_frameIndex(0), g_pd3dDevice(NULL)
		, g_pd3dRtvDescHeap(NULL), g_pd3dSrvDescHeap(NULL), g_pd3dCommandQueue(NULL)
		, g_pd3dCommandList(NULL), g_fence(NULL), g_fenceEvent(NULL)
		, g_fenceLastSignaledValue(0), g_pSwapChain(NULL), g_hSwapChainWaitableObject(NULL)
		, g_mainRenderTargetResource{}, g_mainRenderTargetDescriptor{} {}

	ImGuiLayer::~ImGuiLayer() {
	}
	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		HWND hwnd = reinterpret_cast<WindowsWindow*>(&Application::GetApp())->GetHWND();
		//ImGui_ImplWin32_Init(hwnd);
	}
	void ImGuiLayer::OnDetach() {
	}
	void ImGuiLayer::OnUpdate() {

	}
	void ImGuiLayer::OnEvent(Event& event) {
	}
}