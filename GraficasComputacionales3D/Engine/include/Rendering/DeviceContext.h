#pragma once
#include "Engine/Prerequisites.h"

class
DeviceContext {
public:
  DeviceContext() = default;
  ~DeviceContext();

  ID3D11DeviceContext*
  getDeviceContext() const noexcept
  { return m_DeviceContext; }

  ID3D11DeviceContext**
  getDeviceContextAdress() noexcept
  { return &m_DeviceContext; }

  void
  destroy();

  ID3D11DeviceContext& requireContext() const noexcept;

  void RSSetViewports(unsigned int NumViewports,
                      const D3D11_VIEWPORT* pViewports);

  void PSSetShaderResources(unsigned int StartSlot,
                            unsigned int NumViews,
                            ID3D11ShaderResourceView* const* ppShaderResourceViews);

  void OMSetRenderTargets(unsigned int NumViews,
                          ID3D11RenderTargetView* const* ppRenderTargetViews,
                          ID3D11DepthStencilView* pDepthStencilView);

  void ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
                             const float ColorRGBA[4]);

  void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                             unsigned int ClearFlags,
                             float Depth,
                             unsigned int Stencil);

  void UpdateSubresource(ID3D11Resource* pDstResource,
                         unsigned int DstSubresource,
                         const D3D11_BOX* pDstBox,
                         const void* pSrcData,
                         unsigned int SrcRowPitch,
                         unsigned int SrcDepthPitch);

  void IASetVertexBuffers(unsigned int StartSlot,
                          unsigned int NumBuffers,
                          ID3D11Buffer* const* ppVertexBuffers,
                          const unsigned int* pStrides,
                          const unsigned int* pOffsets);

  void IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
                        DXGI_FORMAT Format,
                        unsigned int Offset);

  void IASetInputLayout(ID3D11InputLayout* pInputLayout);

  void IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  void RSSetState(ID3D11RasterizerState* pRasterizerState);

  void VSSetShader(ID3D11VertexShader* pVertexShader,
                   ID3D11ClassInstance* const* ppClassInstances,
                   unsigned int NumClassInstances);

  void VSSetConstantBuffers(unsigned int StartSlot,
                            unsigned int NumBuffers,
                            ID3D11Buffer* const* ppConstantBuffers);

  void PSSetShader(ID3D11PixelShader* pPixelShader,
                   ID3D11ClassInstance* const* ppClassInstances,
                   unsigned int NumClassInstances);

  void DrawIndexed(unsigned int IndexCount,
                   unsigned int StartIndexLocation,
                   int BaseVertexLocation);

private:
  ID3D11DeviceContext* m_DeviceContext = nullptr;
  IDXGISwapChain* m_SwapChain = nullptr; // <- Para Present
};