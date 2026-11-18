#include "Rendering/RasterizerState.h"

RasterizerState::~RasterizerState() {
  destroy();
}

HRESULT
RasterizerState::init(Device& device, const D3D11_RASTERIZER_DESC* pDesc) {
  if (pDesc == nullptr) {
    ERROR("RasterizerState", "init", "Description pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = device.CreateRasterizerState(pDesc, &m_rasterizerState);

  if (FAILED(result)) {
    ERROR("RasterizerState", "init",
      ("Failed to initialize RasterizerState. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("RasterizerState", "init", "SUCCESS");
  return result;
}

void
RasterizerState::destroy() {
  SafeRelease(m_rasterizerState);
}