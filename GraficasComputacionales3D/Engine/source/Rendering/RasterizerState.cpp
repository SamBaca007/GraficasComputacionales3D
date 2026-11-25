#include "Rendering/RasterizerState.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

HRESULT
RasterizerState::init(Device& device) {
  if (!device.getDevice()) {
    ERROR("RasterizerState", "init", "Device is nullptr.");
    return E_POINTER;
  }

  D3D11_RASTERIZER_DESC rasterizerDesc = {};
  rasterizerDesc.FillMode = D3D11_FILL_SOLID;
  rasterizerDesc.CullMode = D3D11_CULL_BACK;
  rasterizerDesc.FrontCounterClockwise = false;
  rasterizerDesc.DepthBias = 0;
  rasterizerDesc.SlopeScaledDepthBias = 0.0f;
  rasterizerDesc.DepthBiasClamp = 0.0f;
  rasterizerDesc.DepthClipEnable = true;
  rasterizerDesc.ScissorEnable = false;
  rasterizerDesc.MultisampleEnable = false;
  rasterizerDesc.AntialiasedLineEnable = false;

  HRESULT hr = device.CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);

  if (FAILED(hr)) {
    ERROR("RasterizerState", "init", "CHECK FOR CreateRasterizerState()");
  }
  return hr;
}

HRESULT
RasterizerState::init(Device& device, D3D11_FILL_MODE fill, D3D11_CULL_MODE cull,
  bool frontCCW, bool depthClip) {
  if (!device.getDevice()) {
    ERROR("RasterizerState", "init", "Device is nullptr.");
    return E_POINTER;
  }

  D3D11_RASTERIZER_DESC desc{};
  desc.FillMode = fill;
  desc.CullMode = cull;
  desc.FrontCounterClockwise = frontCCW ? TRUE : FALSE;
  desc.DepthClipEnable = depthClip ? TRUE : FALSE;

  HRESULT hr = device.CreateRasterizerState(&desc, &m_RasterizerState);

  if (FAILED(hr)) {
    ERROR("RasterizerState", "init", "CHECK FOR CreateRasterizerState()");
  }
  return hr;
}

void
RasterizerState::update() {
  // Vacío por ahora
}

void
RasterizerState::render(DeviceContext& deviceContext) {
  if (!m_RasterizerState) {
    ERROR("RasterizerState", "render", "RasterizerState is nullptr (init failed or not called)");
    return;
  }
  deviceContext.RSSetState(m_RasterizerState);
}

void
RasterizerState::destroy() {
  SafeRelease(m_RasterizerState);
}