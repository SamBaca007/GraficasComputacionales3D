#pragma once
#include <Engine/Prerequisites.h>

class Device;
class DeviceContext;
class Texture2D;

class
  DepthStencilView {
public:

  DepthStencilView() = default;

  ~DepthStencilView() = default;

  HRESULT
    init(Device& device, Texture2D& depthStencil, DXGI_FORMAT format);

  HRESULT
    init(Device& device, Texture2D& depthStencil, DXGI_FORMAT format,
      D3D11_DSV_DIMENSION ViewDimension);

  void
    update() {};

  void
    render(DeviceContext& deviceContext);

  void
    destroy();

  ID3D11DepthStencilView*
    getDepthStencilView() const
  {
    return m_DepthStencilView;
  }

public:
  ID3D11DepthStencilView* m_DepthStencilView = nullptr;
};
