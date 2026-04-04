#pragma once

#include <GLFW/glfw3.h>
#include <EngineAPI.h>
#include <d3d11.h>
#include <dxgi.h>

class DEFORM_API DXRender
{
public:
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    ID3D11RenderTargetView* m_rtv = nullptr;

    ID3D11Texture2D* m_sceneTexture = nullptr;
    ID3D11RenderTargetView* m_sceneRTV = nullptr;
    ID3D11ShaderResourceView* m_sceneSRV = nullptr;

    ID3D11VertexShader* m_skyVSBlob = nullptr;

    bool Initialize(GLFWwindow* hwnd);
    void Shutdown();
    void Resize(UINT width, UINT height);
    bool IsInitialized() const;

    void BeginBackbufferPass(const float clearColor[4]) const;
    void Present(UINT syncInterval = 1) const;

    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_context; }
    ID3D11RenderTargetView* GetBackbufferRenderTargetView() const { return m_rtv; }
    ID3D11RenderTargetView* GetSceneRenderTargetView() const { return m_sceneRTV; }
    ID3D11ShaderResourceView* GetSceneTextureView() const { return m_sceneSRV; }
    IDXGISwapChain* GetSwapChain() const { return m_swapChain; }
private:
	GLFWwindow* m_window = nullptr;
};
