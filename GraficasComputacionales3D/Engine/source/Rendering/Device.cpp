#include "Rendering/Device.h"

void
Device::destroy() {
  SafeRelease(m_device);
}

HRESULT
Device::CreateRenderTargetView(ID3D11Resource* pResource,
                               const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                               ID3D11RenderTargetView** ppRTView) {
  if (ppRTView == nullptr) {
    ERROR("Device", "CreateRenderTargetView", "Output pointer is null.");
    return E_POINTER;
  }

  *ppRTView = nullptr;

  if (m_device == nullptr || pResource == nullptr) {
    ERROR("Device", "CreateRenderTargetView", "Device or resource ponter is null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);

  if (FAILED(result)) {
    ERROR("Device", "CreateRenderTargetView",
      ("Failed to create render target view. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateRenderTargetView", "SUCCESS");
  return result;
}

HRESULT
Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                        const D3D11_SUBRESOURCE_DATA* pInitialData,
                        ID3D11Texture2D** ppTexture2D) {
  if (ppTexture2D == nullptr) {
    ERROR("Device", "CreateTexture2D", "Output pointer is null.");
    return E_POINTER;
  }

  *ppTexture2D = nullptr;

  // pInitialData puede ser null si creas una textura vacía, pero pDesc es obligatorio.
  if (m_device == nullptr || pDesc == nullptr) {
    ERROR("Device", "CreateTexture2D", "Device or description pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);

  if (FAILED(result)) {
    ERROR("Device", "CreateTexture2D",
      ("Failed to create texture 2D. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateTexture2D", "SUCCESS");
  return result;
}

HRESULT
Device::CreateDepthStencilView(ID3D11Resource* pResource,
                               const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                               ID3D11DepthStencilView** ppDepthStencilView) {
  if (ppDepthStencilView == nullptr) {
    ERROR("Device", "CreateDepthStencilView", "Output pointer is null.");
    return E_POINTER;
  }

  *ppDepthStencilView = nullptr;

  // pDesc puede ser null en DirectX 11 (usa el formato por defecto del recurso), pero pResource es obligatorio.
  if (m_device == nullptr || pResource == nullptr) {
    ERROR("Device", "CreateDepthStencilView", "Device or resource pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);

  if (FAILED(result)) {
    ERROR("Device", "CreateDepthStencilView",
      ("Failed to create depth stencil view. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateDepthStencilView", "SUCCESS");
  return result;
}

HRESULT
Device::CreateVertexShader(const void* pShaderBytecode,
                            SIZE_T BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11VertexShader** ppVertexShader) {
  if (ppVertexShader == nullptr) {
    ERROR("Device", "CreateVertexShader", "Output pointer is null.");
    return E_POINTER;
  }

  *ppVertexShader = nullptr;

  // pClassLinkage normalmente es null a menos que uses shader interfaces.
  if (m_device == nullptr || pShaderBytecode == nullptr || BytecodeLength == 0) {
    ERROR("Device", "CreateVertexShader", "Device, shader bytecode is null, or length is 0.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);

  if (FAILED(result)) {
    ERROR("Device", "CreateVertexShader",
      ("Failed to create vertex shader. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateVertexShader", "SUCCESS");
  return result;
}

HRESULT
Device::CreatePixelShader(const void* pShaderBytecode,
                          SIZE_T BytecodeLength,
                          ID3D11ClassLinkage* pClassLinkage,
                          ID3D11PixelShader** ppPixelShader) {
  if (ppPixelShader == nullptr) {
    ERROR("Device", "CreatePixelShader", "Output pointer is null.");
    return E_POINTER;
  }

  *ppPixelShader = nullptr;

  if (m_device == nullptr || pShaderBytecode == nullptr || BytecodeLength == 0) {
    ERROR("Device", "CreatePixelShader", "Device, shader bytecode is null, or length is 0.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);

  if (FAILED(result)) {
    ERROR("Device", "CreatePixelShader",
      ("Failed to create pixel shader. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreatePixelShader", "SUCCESS");
  return result;
}

HRESULT
Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                          UINT NumElements,
                          const void* pShaderBytecodeWithInputSignature,
                          SIZE_T BytecodeLength,
                          ID3D11InputLayout** ppInputLayout) {
  if (ppInputLayout == nullptr) {
    ERROR("Device", "CreateInputLayout", "Output pointer is null.");
    return E_POINTER;
  }

  *ppInputLayout = nullptr;

  // Ambos punteros de entrada son estrictamente necesarios para el Input Layout.
  if (m_device == nullptr || pInputElementDescs == nullptr || pShaderBytecodeWithInputSignature == nullptr) {
    ERROR("Device", "CreateInputLayout", "Device or input signature pointers are null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);

  if (FAILED(result)) {
    ERROR("Device", "CreateInputLayout",
      ("Failed to create input layout. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateInputLayout", "SUCCESS");
  return result;
}

HRESULT
Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
                     const D3D11_SUBRESOURCE_DATA* pInitialData,
                     ID3D11Buffer** ppBuffer) {
  if (ppBuffer == nullptr) {
    ERROR("Device", "CreateBuffer", "Output pointer is null.");
    return E_POINTER;
  }

  *ppBuffer = nullptr;

  // pInitialData puede ser null si creas un buffer que llenarás después.
  if (m_device == nullptr || pDesc == nullptr) {
    ERROR("Device", "CreateBuffer", "Device or description pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);

  if (FAILED(result)) {
    ERROR("Device", "CreateBuffer",
      ("Failed to create buffer. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateBuffer", "SUCCESS");
  return result;
}

HRESULT
Device::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
                              ID3D11RasterizerState** ppRasterizerState) {
  if (ppRasterizerState == nullptr) {
    ERROR("Device", "CreateRasterizerState", "Output pointer is null.");
    return E_POINTER;
  }

  *ppRasterizerState = nullptr;

  if (m_device == nullptr || pRasterizerDesc == nullptr) {
    ERROR("Device", "CreateRasterizerState", "Device or description pointer is null.");
    return E_INVALIDARG;
  }

  HRESULT result = m_device->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);

  if (FAILED(result)) {
    ERROR("Device", "CreateRasterizerState",
      ("Failed to create rasterizer state. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateRasterizerState", "SUCCESS");
  return result;
}