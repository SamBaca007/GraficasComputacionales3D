#include "Rendering/DepthStencilView.h"

DepthStencilView::~DepthStencilView() {
  destroy();
}

HRESULT
DepthStencilView::init(Device& device, ID3D11Resource* pResource,
  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc) {
  if (pResource == nullptr) {
    ERROR("DepthStencilView", "init", "Resource pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = device.CreateDepthStencilView(pResource, pDesc, &m_dsv);

  if (FAILED(result)) {
    ERROR("DepthStencilView", "init",
      ("Failed to initialize DepthStencilView. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("DepthStencilView", "init", "SUCCESS");
  return result;
}

void
DepthStencilView::destroy() {
  SafeRelease(m_dsv);
}