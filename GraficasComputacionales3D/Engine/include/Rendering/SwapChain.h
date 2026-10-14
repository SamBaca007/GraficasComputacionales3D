#pragma once
#include "Engine/Prerequisites.h"

class Device;
class Texture2D;

class
SwapChain {
public:
  SwapChain() = default;
  ~SwapChain();

  IDXGISwapChain*
  getSwapChain() const noexcept
  { return m_SwapChain; }

  IDXGISwapChain**
    getSwapChainAdress() noexcept
  { return &m_SwapChain; }

  SwapChain(const SwapChain&) = delete;
  SwapChain& operator = (const SwapChain&) = delete;

  SwapChain(SwapChain&&) = delete;
  SwapChain& operator = (SwapChain&&) = delete;

  HRESULT
    init(Device device, HWND windowHandle, unsigned int width, unsigned int height);

  void
  destroy();

  HRESULT GetBuffer(unsigned int Buffer,
                    REFIID riid,
                    void** ppSurface);

  HRESULT Present(unsigned int SyncInterval,
                  unsigned int Flags);

  HRESULT ResizeBuffers(unsigned int width,
                        unsigned int height);

private:
  IDXGISwapChain* m_SwapChain = nullptr;
};