#include "Rendering/RenderTargetView.h"

RenderTargetView::~RenderTargetView() {
  destroy();
}

HRESULT
RenderTargetView::init(Device& device, ID3D11Resource* pResource,
  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc) {
  if (pResource == nullptr) {
    ERROR("RenderTargetView", "init", "Resource pointer is null.");
    return E_INVALIDARG;
  }

  // Usamos el Device que ya encapsulaste para crear la vista
  HRESULT result = device.CreateRenderTargetView(pResource, pDesc, &m_rtv);

  if (FAILED(result)) {
    ERROR("RenderTargetView", "init",
      ("Failed to initialize RenderTargetView. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("RenderTargetView", "init", "SUCCESS");
  return result;
}

void
RenderTargetView::destroy() {
  SafeRelease(m_rtv);
}