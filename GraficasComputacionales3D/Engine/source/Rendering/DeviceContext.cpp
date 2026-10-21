#include "Rendering/DeviceContext.h"

void
DeviceContext::destroy() {
  SafeRelease(m_DeviceContext);
}

DeviceContext::~DeviceContext() {
  if (m_DeviceContext) {
    m_DeviceContext->Release();
    m_DeviceContext = nullptr;
  }
}

ID3D11DeviceContext&
DeviceContext::requireContext() const noexcept {
  if (!m_DeviceContext) {
    ERROR("DeviceContext", "RequireContext", "Device content is not initialized.");
  }
  return *m_DeviceContext;
}

void
DeviceContext::RSSetViewports(unsigned int NumViewports,
  const D3D11_VIEWPORT* pViewports) {
  if (!pViewports || NumViewports == 0) {
    ERROR("DeviceContext", "RSSetViewports", "Invalid viewport parameters.");
    return;
  }
  requireContext().RSSetViewports(NumViewports, pViewports);
}

void
DeviceContext::PSSetShaderResources(unsigned int StartSlot,
                                    unsigned int NumViews,
                                    ID3D11ShaderResourceView* const* ppShaderResourceViews) {
  if (!ppShaderResourceViews || NumViews == 0) {
    ERROR("DeviceContext", "PSSetShaderResources", "Invalid shader resource view parameters.");
    return;
  }
}

void
DeviceContext::OMSetRenderTargets(unsigned int NumViews,
  ID3D11RenderTargetView* const* ppRenderTargetViews,
  ID3D11DepthStencilView* pDepthStencilView) {
  // En DX11 es válido pasar nullptr para desvincular el Render Target o el Depth Stencil
  requireContext().OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void
DeviceContext::ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
  const float ColorRGBA[4]) {
  if (!pRenderTargetView) {
    ERROR("DeviceContext", "ClearRenderTargetView", "RenderTargetView is null.");
    return;
  }
  requireContext().ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void
DeviceContext::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
  unsigned int ClearFlags,
  float Depth,
  unsigned int Stencil) {
  if (!pDepthStencilView) {
    ERROR("DeviceContext", "ClearDepthStencilView", "DepthStencilView is null.");
    return;
  }
  requireContext().ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

void
DeviceContext::UpdateSubresource(ID3D11Resource* pDstResource,
  unsigned int DstSubresource,
  const D3D11_BOX* pDstBox,
  const void* pSrcData,
  unsigned int SrcRowPitch,
  unsigned int SrcDepthPitch) {
  if (!pDstResource || !pSrcData) {
    ERROR("DeviceContext", "UpdateSubresource", "Invalid destination resource or source data.");
    return;
  }
  requireContext().UpdateSubresource(pDstResource, DstSubresource, pDstBox,
    pSrcData, SrcRowPitch, SrcDepthPitch);
}

void
DeviceContext::IASetVertexBuffers(unsigned int StartSlot,
  unsigned int NumBuffers,
  ID3D11Buffer* const* ppVertexBuffers,
  const unsigned int* pStrides,
  const unsigned int* pOffsets) {
  if (NumBuffers > 0 && (!ppVertexBuffers || !pStrides || !pOffsets)) {
    ERROR("DeviceContext", "IASetVertexBuffers", "Invalid vertex buffer arrays.");
    return;
  }
  requireContext().IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void
DeviceContext::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
  DXGI_FORMAT Format,
  unsigned int Offset) {
  // Es válido pasar pIndexBuffer como nullptr para desvincularlo
  requireContext().IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void
DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout) {
  // Es válido pasar nullptr para desvincular el layout
  requireContext().IASetInputLayout(pInputLayout);
}

void
DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) {
  requireContext().IASetPrimitiveTopology(Topology);
}

void
DeviceContext::RSSetState(ID3D11RasterizerState* pRasterizerState) {
  // Pasar nullptr restaura el estado de rasterización por defecto de DX11
  requireContext().RSSetState(pRasterizerState);
}

void
DeviceContext::VSSetShader(ID3D11VertexShader* pVertexShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  // Es válido pasar nullptr en pVertexShader para desvincular el shader
  requireContext().VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void
DeviceContext::VSSetConstantBuffers(unsigned int StartSlot,
  unsigned int NumBuffers,
  ID3D11Buffer* const* ppConstantBuffers) {
  if (NumBuffers > 0 && !ppConstantBuffers) {
    ERROR("DeviceContext", "VSSetConstantBuffers", "Constant buffer array is null.");
    return;
  }
  requireContext().VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void
DeviceContext::PSSetShader(ID3D11PixelShader* pPixelShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  // Es válido pasar nullptr en pPixelShader para desvincular el shader
  requireContext().PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void
DeviceContext::DrawIndexed(unsigned int IndexCount,
  unsigned int StartIndexLocation,
  int BaseVertexLocation) {
  if (IndexCount == 0) {
    ERROR("DeviceContext", "DrawIndexed", "Index count is zero.");
    return;
  }
  requireContext().DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}