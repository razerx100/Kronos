#include"ImGuiLayer.hpp"
#include"imgui.h"
#include"Kronos/Application.hpp"
#include"Platform/Windows/WindowsWindow.hpp"
#include"Platform/Windows/DirectX12/imgui_impl_win32.hpp"
#include"Platform/Windows/DirectX12/imgui_impl_dx12.hpp"

namespace Kronos {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer"), g_frameContext{}, g_frameIndex(0), g_pd3dDevice(NULL),
		g_pd3dRtvDescHeap(NULL), g_pd3dSrvDescHeap(NULL), g_pd3dCommandQueue(NULL),
		g_pd3dCommandList(NULL), g_fence(NULL), g_fenceEvent(NULL),
		g_fenceLastSignaledValue(0), g_pSwapChain(NULL), g_hSwapChainWaitableObject(NULL),
		g_mainRenderTargetResource{}, g_mainRenderTargetDescriptor{},
		showDemowindow(true), clear_color(0.45f, 0.55f, 0.60f, 1.00f) {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		HWND hwnd = reinterpret_cast<WindowsWindow*>(&Application::GetApp())->GetHWND();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
			DXGI_FORMAT_B8G8R8A8_UNORM, g_pd3dSrvDescHeap,
			g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
			g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
	}
	void ImGuiLayer::OnDetach() {
		WaitForLastSubmittedFrame();
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
	}
	void ImGuiLayer::OnUpdate() {
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&showDemowindow);

		FrameContext* frameCtxt = WaitForNextFrameResources();
        UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
        frameCtxt->CommandAllocator->Reset();

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource   = g_mainRenderTargetResource[backBufferIdx];
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;

        g_pd3dCommandList->Reset(frameCtxt->CommandAllocator, NULL);
        g_pd3dCommandList->ResourceBarrier(1, &barrier);
        g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], (float*)&clear_color, 0, NULL);
        g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);
        g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
        ImGui::Render();
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
        g_pd3dCommandList->ResourceBarrier(1, &barrier);
        g_pd3dCommandList->Close();

        g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync

        UINT64 fenceValue = g_fenceLastSignaledValue + 1;
        g_pd3dCommandQueue->Signal(g_fence, fenceValue);
        g_fenceLastSignaledValue = fenceValue;
        frameCtxt->FenceValue = fenceValue;
	}
	void ImGuiLayer::OnEvent(Event& event) {
	}
	bool ImGuiLayer::CreateDeviceD3D(HWND hWnd) {
	}
	void ImGuiLayer::CleanupDeviceD3D() {

	}
	void ImGuiLayer::CreateRenderTarget() {

	}
	void ImGuiLayer::CleanupRenderTarget() {

	}
	void ImGuiLayer::WaitForLastSubmittedFrame() {

	}
	FrameContext* ImGuiLayer::WaitForNextFrameResources() {

	}
	void ImGuiLayer::ResizeSwapChain(HWND hWnd, int width, int height) {

	}
}