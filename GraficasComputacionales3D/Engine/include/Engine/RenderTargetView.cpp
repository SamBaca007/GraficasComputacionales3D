#include "Rendering/RenderTargetView.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/DepthStencilView.h"
#include "Rendering/Texture2D.h"

HRESULT
RenderTargetView::init(Device& device, Texture2D& backBuffer, DXGI_FORMAT Format) {
  if (!device.getDevice()) {
    ERROR("RenderTargetView", "init", "Device is nullptr.");
    return E_POINTER;
  }

  // CORRECCIÓN: Usamos getTexture() para verificar si es nulo
  if (!backBuffer.getTexture()) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }
  if (Format == DXGI_FORMAT_UNKNOWN) {
    ERROR("RenderTargetView", "init", "Format is DXGI_FORMAT_UNKNOWN.");
    return E_INVALIDARG;
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = Format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

  HRESULT hr = device.CreateRenderTargetView(backBuffer.getTexture(), &desc,
    &m_RenderTargetView);
  if (FAILED(hr)) {
    ERROR("RenderTargetView", "init", ("Failed to create render target view. HRESULT: "
      + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

HRESULT
RenderTargetView::init(Device& device, Texture2D& inTex, D3D11_RTV_DIMENSION ViewDimension,
  DXGI_FORMAT Format) {
  if (!device.getDevice()) {
    ERROR("RenderTargetView", "init", "Device is nullptr.");
    return E_POINTER;
  }

  // CORRECCIÓN: Usamos getTexture()
  if (!inTex.getTexture()) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }
  if (Format == DXGI_FORMAT_UNKNOWN) {
    ERROR("RenderTargetView", "init", "Format is DXGI_FORMAT_UNKNOWN.");
    return E_INVALIDARG;
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = Format;
  desc.ViewDimension = ViewDimension;

  HRESULT hr = device.CreateRenderTargetView(inTex.getTexture(),
    &desc, &m_RenderTargetView);

  if (FAILED(hr)) {
    ERROR("RenderTargetView", "init", ("Failed to create render target view. HRESULT: "
      + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

HRESULT
RenderTargetView::init(Device& device, ID3D11Texture2D* backBuffer, DXGI_FORMAT Format) {
  if (!device.getDevice()) return E_POINTER;
  if (!backBuffer) return E_POINTER;

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = Format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

  return device.CreateRenderTargetView(backBuffer, &desc, &m_RenderTargetView);
}

void
RenderTargetView::update() {
  // Vacío por ahora
}

void
RenderTargetView::render(DeviceContext& deviceContext, DepthStencilView& depthStencilView,
  unsigned int numViews, const float ClearColor[4]) {
  if (!deviceContext.getDeviceContext()) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_RenderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }

  deviceContext.ClearRenderTargetView(m_RenderTargetView, ClearColor);

  deviceContext.OMSetRenderTargets(numViews, &m_RenderTargetView,
    depthStencilView.getDepthStencilView());
}

void
RenderTargetView::render(DeviceContext& deviceContext, unsigned int numViews) {
  if (!deviceContext.getDeviceContext()) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_RenderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }

  deviceContext.OMSetRenderTargets(numViews, &m_RenderTargetView, nullptr);
}

void
RenderTargetView::destroy() {
  SafeRelease(m_RenderTargetView);
}