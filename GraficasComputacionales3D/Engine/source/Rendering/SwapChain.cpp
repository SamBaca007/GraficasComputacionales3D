#include "Rendering/SwapChain.h"
#include "Rendering/Device.h"

void
SwapChain::destroy() {
  SafeRelease(m_SwapChain);
}

HRESULT
SwapChain::init(Device device, HWND windowHandle, unsigned int width, unsigned int height) {
  ID3D11Device* pDevice = device.getDevice();
  if (!pDevice) {
    ERROR("SwapChain", "init", "Provided Device is null.");
    return E_INVALIDARG;
  }

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  // Asignamos directamente el HWND que recibimos por parámetro
  sd.OutputWindow = windowHandle;

  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  IDXGIDevice* pDXGIDevice = nullptr;
  HRESULT hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
  if (FAILED(hr)) return hr;

  IDXGIAdapter* pDXGIAdapter = nullptr;
  hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
  if (FAILED(hr)) { SafeRelease(pDXGIDevice); return hr; }

  IDXGIFactory* pDXGIFactory = nullptr;
  hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
  if (FAILED(hr)) { SafeRelease(pDXGIAdapter); SafeRelease(pDXGIDevice); return hr; }

  hr = pDXGIFactory->CreateSwapChain(pDevice, &sd, &m_SwapChain);

  SafeRelease(pDXGIFactory);
  SafeRelease(pDXGIAdapter);
  SafeRelease(pDXGIDevice);

  if (FAILED(hr)) {
    ERROR("SwapChain", "init", ("Failed to create SwapChain. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  MESSAGE("SwapChain", "init", "SUCCESS");
  return S_OK;
}

SwapChain::~SwapChain() {
  destroy();
}

HRESULT
SwapChain::GetBuffer(unsigned int Buffer,
  REFIID riid,
  void** ppSurface) {
  if (ppSurface == nullptr) {
    ERROR("SwapChain", "GetBuffer", "Output pointer ppSurface is null.");
    return E_POINTER;
  }

  *ppSurface = nullptr;

  if (!m_SwapChain) {
    ERROR("SwapChain", "GetBuffer", "SwapChain is not initialized.");
    return E_FAIL;
  }

  HRESULT result = m_SwapChain->GetBuffer(Buffer, riid, ppSurface);

  if (FAILED(result)) {
    ERROR("SwapChain", "GetBuffer",
      ("Failed to get buffer from swap chain. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("SwapChain", "GetBuffer", "SUCCESS");
  return result;
}

HRESULT
SwapChain::Present(unsigned int SyncInterval,
  unsigned int Flags) {
  // Verificamos que el puntero al SwapChain exista antes de llamar a Present
  if (!m_SwapChain) {
    ERROR("SwapChain", "Present", "SwapChain is not initialized.");
    return E_FAIL; // Retornamos un HRESULT de falla estándar
  }

  // Llamamos al método Present del Swap Chain nativo de DXGI
  return m_SwapChain->Present(SyncInterval, Flags);
}

HRESULT
SwapChain::ResizeBuffers(unsigned int width,
  unsigned int height) {
  if (!m_SwapChain) {
    ERROR("SwapChain", "ResizeBuffers", "SwapChain is not initialized.");
    return E_FAIL;
  }

  // Los parámetros son: BufferCount, Width, Height, NewFormat, SwapChainFlags
  // Pasando 0 en BufferCount y DXGI_FORMAT_UNKNOWN (0) preservamos los ajustes originales.
  HRESULT result = m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  if (FAILED(result)) {
    ERROR("SwapChain", "ResizeBuffers",
      ("Failed to resize buffers. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("SwapChain", "ResizeBuffers", "SUCCESS");
  return result;
}