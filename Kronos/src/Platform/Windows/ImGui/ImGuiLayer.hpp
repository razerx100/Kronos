#ifndef __IMGUI_LAYER_HPP__
#define __IMGUI_LAYER_HPP__
#include<Kronos/Layer.hpp>
#include<d3d12.h>
#include<dxgi1_4.h>
#include"imgui.h"
#include"Kronos/Events/ApplicationEvent.hpp"
#include"Kronos/Events/MouseEvent.hpp"
#include"Kronos/Events/KeyEvent.hpp"

namespace Kronos {
	struct FrameContext {
	    ID3D12CommandAllocator* CommandAllocator;
		UINT64                  FenceValue;
	};
	class KRONOS_API ImGuiLayer : public Layer {
	private:
		static int const             NUM_FRAMES_IN_FLIGHT = 3;
		FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT];
		UINT                         g_frameIndex;

		static int const             NUM_BACK_BUFFERS = 3;
		ID3D12Device*                g_pd3dDevice;
		ID3D12DescriptorHeap*        g_pd3dRtvDescHeap;
		ID3D12DescriptorHeap*        g_pd3dSrvDescHeap;
		ID3D12CommandQueue*          g_pd3dCommandQueue;
		ID3D12GraphicsCommandList*   g_pd3dCommandList;
		ID3D12Fence*                 g_fence;
		HANDLE                       g_fenceEvent;
		UINT64                       g_fenceLastSignaledValue;
		IDXGISwapChain3*             g_pSwapChain;
		HANDLE                       g_hSwapChainWaitableObject;
		ID3D12Resource*              g_mainRenderTargetResource[NUM_BACK_BUFFERS];
		D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS];

		HWND hwnd;
		bool showDemowindow = true, showAnotherWindow = false;
		ImVec4 clear_color;

		bool CreateDeviceD3D(HWND hWnd);
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		void WaitForLastSubmittedFrame();
		FrameContext* WaitForNextFrameResources();
		void ResizeSwapChain(HWND hWnd, int width, int height);

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnMousePress(MouseButtonPressedEvent& event);
		bool OnMouseRelease(MouseButtonReleasedEvent& event);
		bool OnMouseScroll(MouseScrolledEvent& event);
		bool OnKeyPress(KeyPressedEvent& event);
		bool OnKeyRelease(KeyReleasedEvent& event);
		bool OnKeyType(KeyTypedEvent& event);

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	};
}
#endif