#include"ImGuiLayer.hpp"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"
#include<d3d12.h>
#include<dxgi1_4.h>
#include<tchar.h>

namespace Kronos {
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
	}
	ImGuiLayer::~ImGuiLayer() {
	}
	void ImGuiLayer::OnAttach() {
	}
	void ImGuiLayer::OnDetach() {
	}
	void ImGuiLayer::OnUpdate() {

	}
	void ImGuiLayer::OnEvent(Event& event) {
	}
}