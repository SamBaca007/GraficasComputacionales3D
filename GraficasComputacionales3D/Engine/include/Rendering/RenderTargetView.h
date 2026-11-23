#pragma once
#include <Engine/Prerequisites.h>

class Device;
class DeviceContext;
class Texture2D;
class DepthStencilView;

class RenderTargetView {
public:
  RenderTargetView() = default;
  ~RenderTargetView() = default;

  HRESULT
  init(Device& device, Texture2D& backBuffer,
    DXGI_FORMAT Format);

  HRESULT
  init(Device& device, Texture2D& inTex, D3D11_RTV_DIMENSION ViewDimension,
    DXGI_FORMAT Format);

  HRESULT
  init(Device& device, ID3D11Texture2D* backBuffer, DXGI_FORMAT Format);

  void
  update();

  void
  render(DeviceContext& DeviceContext, DepthStencilView& DepthStencilView,
    unsigned int numViews, const float ClearColor[4]);

  void
  render(DeviceContext& deviceContext, unsigned int numViews);

  void
  destroy();

  ID3D11RenderTargetView*
  getRenderTargetView() const {
    return m_RenderTargetView;
  }
private:
  ID3D11RenderTargetView* m_RenderTargetView = nullptr;
};