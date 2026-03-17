// Defining GLFW_EXPOSE_NATIVE_WIN32 through pre-processor statements
#define GLFW_EXPOSE_NATIVE_WIN32

//GLFW headers
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//DirectX11 headers and library
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <dxgi.h>


//header
#include "DXRender.h"
#include <cstdio>

#include "imgui/imgui.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_dx11.h>

bool DXRender::Initialize(GLFWwindow* GLFWhwnd)
{
    HWND hwnd = glfwGetWin32Window(GLFWhwnd);

    // Swap chain description
    {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.Windowed = TRUE;
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        D3D_FEATURE_LEVEL featureLevel;
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        IDXGISwapChain* swapChain = nullptr;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &scd,
            &swapChain,
            &device,
            &featureLevel,
            &context
        );
        if (FAILED(hr))
            return false;

        m_swapChain = swapChain;
        m_device = device;
        m_context = context;
    }

    // Backbuffer RTV
    {
        ID3D11Texture2D* backBuffer = nullptr;
        hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (FAILED(hr) || !backBuffer)
            return false;

        D3D11_TEXTURE2D_DESC backBufferDesc = {};
        backBuffer->GetDesc(&backBufferDesc);
        UINT sceneWidth = backBufferDesc.Width;
        UINT sceneHeight = backBufferDesc.Height;
        if (sceneWidth == 0) sceneWidth = 1280;
        if (sceneHeight == 0) sceneHeight = 720;

        hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
        backBuffer->Release();
        if (FAILED(hr) || !m_rtv)
            return false;
    }

    // Viewport
    {
        D3D11_VIEWPORT vp = {};
        vp.Width = 1280;
        vp.Height = 720;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        m_context->RSSetViewports(1, &vp);
    }

    // Scene render texture (RTV+SRV)
    D3D11_TEXTURE2D_DESC td = {};
    {
        td.Width = 1280;
        td.Height = 720;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags = 0;
    }

    hr = m_device->CreateTexture2D(&td, nullptr, &m_sceneTexture);
    if (FAILED(hr) || !m_sceneTexture)
        return false;

    hr = m_device->CreateRenderTargetView(m_sceneTexture, nullptr, &m_sceneRTV);
    if (FAILED(hr) || !m_sceneRTV)
        return false;

    hr = m_device->CreateShaderResourceView(m_sceneTexture, nullptr, &m_sceneSRV);
    if (FAILED(hr) || !m_sceneSRV)
        return false;

    return true;
}

bool DXRender::InitializeImgui(GLFWwindow* glfwWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOther(glfwWindow, true))
        return false;
    if (!ImGui_ImplDX11_Init(m_device, m_context))
        return false;

    return true;
}

void DXRender::Resize(UINT width, UINT height)
{
    if (!m_swapChain) return;

    // Release old views/textures
    if (m_rtv) { m_rtv->Release();        m_rtv = nullptr; }
    if (m_sceneRTV) { m_sceneRTV->Release();   m_sceneRTV = nullptr; }
    if (m_sceneSRV) { m_sceneSRV->Release();   m_sceneSRV = nullptr; }
    if (m_sceneTexture) { m_sceneTexture->Release(); m_sceneTexture = nullptr; }

    // Resize swap chain buffers
    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    // Recreate backbuffer RTV (and update viewport)
    ID3D11Texture2D* backBuffer = nullptr;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    D3D11_TEXTURE2D_DESC backDesc{};
    backBuffer->GetDesc(&backDesc);
    m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
    backBuffer->Release();

    D3D11_VIEWPORT vp{};
    vp.Width = (FLOAT)backDesc.Width;
    vp.Height = (FLOAT)backDesc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);

    // Recreate scene texture/RTV/SRV with new size
    D3D11_TEXTURE2D_DESC td{};
    td.Width = backDesc.Width;
    td.Height = backDesc.Height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    m_device->CreateTexture2D(&td, nullptr, &m_sceneTexture);
    m_device->CreateRenderTargetView(m_sceneTexture, nullptr, &m_sceneRTV);
    m_device->CreateShaderResourceView(m_sceneTexture, nullptr, &m_sceneSRV);
}