#pragma once
//GLFW header
#include <GLFW/glfw3.h> //no fucking way ts fixed it

//d3d11 headers
#include <d3d11.h>
#include <dxgi.h>

class DXRender {
    public:
        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;
        ID3D11RenderTargetView* m_rtv = nullptr;

        ID3D11Texture2D* m_sceneTexture = nullptr;
        ID3D11RenderTargetView* m_sceneRTV = nullptr;
        ID3D11ShaderResourceView* m_sceneSRV = nullptr;

        bool Initialize(GLFWwindow* hwnd);
        bool InitializeImgui(GLFWwindow* hwnd);
		void Resize(UINT width, UINT height);
};