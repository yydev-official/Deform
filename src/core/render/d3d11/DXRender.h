#pragma
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

class Render {
    public:
        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;
        ID3D11RenderTargetView* m_rtv = nullptr;
        bool Initialize(HWND hwnd);
};