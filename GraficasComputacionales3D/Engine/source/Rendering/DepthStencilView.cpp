#include "Rendering/DepthStencilView.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/Texture2D.h"

HRESULT
DepthStencilView::init(Device& device, Texture2D& depthStencil, DXGI_FORMAT format) {
  if (!device.getDevice()) {
    ERROR("DepthStencilView", "init", "Device is null.");
    return E_POINTER; // CORRECCIÓN: Faltaba el return para evitar el crash
  }
  if (!depthStencil.getTexture()) {
    ERROR("DepthStencilView", "init", "Texture is null.");
    return E_POINTER;
  }

  // Config the depth stencil view description
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  memset(&descDSV, 0, sizeof(descDSV));
  descDSV.Format = format;

  // CORRECCIÓN CRÍTICA: TEXTURE2D en lugar de TEXTURE2DMS
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;

  // Create depth stencil view (CORRECCIÓN: getTexture en minúscula)
  HRESULT hr = device.CreateDepthStencilView(depthStencil.getTexture(),
    &descDSV, &m_DepthStencilView);

  if (FAILED(hr)) {
    ERROR("DepthStencilView", "init", ("Failed to create depth stencil view. HRESULT: "
      + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

HRESULT
DepthStencilView::init(Device& device, Texture2D& depthStencil, DXGI_FORMAT format,
  D3D11_DSV_DIMENSION ViewDimension) {
  if (!device.getDevice()) {
    ERROR("DepthStencilView", "init", "Device is null.");
    return E_POINTER; // CORRECCIÓN: Faltaba el return
  }
  if (!depthStencil.getTexture()) {
    ERROR("DepthStencilView", "init", "Texture is null.");
    return E_POINTER;
  }

  // Config the depth stencil view description
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
  descDSV.Format = format;
  descDSV.ViewDimension = ViewDimension; // CORRECCIÓN: Coincidir con la mayúscula del parámetro

  if (ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2D) { // CORRECCIÓN: Coincidir con la mayúscula del parámetro
    descDSV.Texture2D.MipSlice = 0;
  }

  HRESULT hr = device.CreateDepthStencilView(depthStencil.getTexture(),
    &descDSV, &m_DepthStencilView);

  if (FAILED(hr)) {
    ERROR("DepthStencilView", "init", ("Failed to create depth stencil view. HRESULT: "
      + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

void
DepthStencilView::render(DeviceContext& deviceContext) {
  if (!deviceContext.getDeviceContext()) {
    ERROR("DepthStencilView", "render", "Device context is null.");
    return;
  }

  // Clear depth stencil view
  deviceContext.ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH |
    D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void
DepthStencilView::destroy() {
  SafeRelease(m_DepthStencilView); // CORRECCIÓN: Uso de tu macro SafeRelease
}