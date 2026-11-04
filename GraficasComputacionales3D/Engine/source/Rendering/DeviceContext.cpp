/**
 * @file DeviceContext.cpp
 * @brief Implementación de la clase DeviceContext, actuando como intermediario seguro para ID3D11DeviceContext.
 */

#include "Rendering/DeviceContext.h"

 /**
  * @brief Libera los recursos de memoria del contexto del dispositivo.
  *
  * Utiliza la macro SafeRelease para liberar el puntero COM de forma segura.
  */
void
DeviceContext::destroy() {
  SafeRelease(m_DeviceContext);
}

/**
 * @brief Destructor de la clase DeviceContext.
 *
 * Asegura la liberación del ID3D11DeviceContext en caso de que no se haya
 * llamado a destroy() previamente.
 */
DeviceContext::~DeviceContext() {
  if (m_DeviceContext) {
    m_DeviceContext->Release();
    m_DeviceContext = nullptr;
  }
}

/**
 * @brief Garantiza que el contexto esté inicializado antes de su uso.
 *
 * @return ID3D11DeviceContext& Referencia al contexto de D3D11.
 * @throws Puede generar un log de ERROR si el contexto no está inicializado (dependiendo de la implementación de la macro ERROR).
 */
ID3D11DeviceContext&
DeviceContext::requireContext() const noexcept {
  if (!m_DeviceContext) {
    ERROR("DeviceContext", "RequireContext", "Device content is not initialized.");
  }
  return *m_DeviceContext;
}

/**
 * @brief Establece los viewports en la etapa de rasterización.
 *
 * @param NumViewports Cantidad de viewports a vincular.
 * @param pViewports Arreglo con las estructuras de los viewports.
 */
void
DeviceContext::RSSetViewports(unsigned int NumViewports,
  const D3D11_VIEWPORT* pViewports) {
  if (!pViewports || NumViewports == 0) {
    ERROR("DeviceContext", "RSSetViewports", "Invalid viewport parameters.");
    return;
  }
  requireContext().RSSetViewports(NumViewports, pViewports);
}

/**
 * @brief Establece los recursos de shader (texturas) en el Pixel Shader.
 *
 * @param StartSlot Ranura inicial donde se empezarán a vincular los recursos.
 * @param NumViews Cantidad de vistas a vincular.
 * @param ppShaderResourceViews Arreglo de punteros a las vistas de recursos (SRV).
 */
void
DeviceContext::PSSetShaderResources(unsigned int StartSlot,
  unsigned int NumViews,
  ID3D11ShaderResourceView* const* ppShaderResourceViews) {
  if (!ppShaderResourceViews || NumViews == 0) {
    ERROR("DeviceContext", "PSSetShaderResources", "Invalid shader resource view parameters.");
    return;
  }
  // TODO: Falta la llamada real al API (requireContext().PSSetShaderResources(...))
}

/**
 * @brief Establece los Render Targets y el Depth/Stencil buffer en la etapa de Output Merger.
 *
 * @note En DX11 es válido pasar nullptr en los arreglos para desvincular el Render Target o el Depth Stencil.
 *
 * @param NumViews Número de Render Targets a vincular.
 * @param ppRenderTargetViews Arreglo de punteros a Render Target Views.
 * @param pDepthStencilView Puntero a la vista del Depth Stencil.
 */
void
DeviceContext::OMSetRenderTargets(unsigned int NumViews,
  ID3D11RenderTargetView* const* ppRenderTargetViews,
  ID3D11DepthStencilView* pDepthStencilView) {
  requireContext().OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

/**
 * @brief Limpia el Render Target View con un color específico.
 *
 * @param pRenderTargetView Puntero al Render Target que se va a limpiar.
 * @param ColorRGBA Arreglo de 4 flotantes representando el color (R, G, B, A).
 */
void
DeviceContext::ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
  const float ColorRGBA[4]) {
  if (!pRenderTargetView) {
    ERROR("DeviceContext", "ClearRenderTargetView", "RenderTargetView is null.");
    return;
  }
  requireContext().ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

/**
 * @brief Limpia la vista del Depth/Stencil.
 *
 * @param pDepthStencilView Puntero al Depth Stencil View a limpiar.
 * @param ClearFlags Banderas indicando qué componentes limpiar (Profundidad, Stencil o ambos).
 * @param Depth Valor de profundidad inicial (típicamente 1.0f).
 * @param Stencil Valor de stencil inicial (típicamente 0).
 */
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

/**
 * @brief Actualiza la memoria de un subrecurso de la GPU copiando datos desde la CPU.
 *
 * @param pDstResource Recurso de destino en la memoria gráfica.
 * @param DstSubresource Índice del subrecurso objetivo.
 * @param pDstBox Caja (box) delimitadora que define la región a actualizar.
 * @param pSrcData Puntero a los datos de origen en la memoria del sistema.
 * @param SrcRowPitch Tamaño en bytes de una fila de datos.
 * @param SrcDepthPitch Tamaño en bytes de un bloque de profundidad de datos.
 */
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

/**
 * @brief Vincula arreglos de buffers de vértices a la etapa del Input Assembler.
 *
 * @param StartSlot Ranura inicial de vinculación.
 * @param NumBuffers Número de buffers a vincular.
 * @param ppVertexBuffers Arreglo de punteros a los buffers de vértices.
 * @param pStrides Arreglo con los tamaños (strides) de cada vértice por buffer.
 * @param pOffsets Arreglo con los offsets iniciales por buffer.
 */
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

/**
 * @brief Vincula un buffer de índices a la etapa del Input Assembler.
 *
 * @note Es válido pasar pIndexBuffer como nullptr para desvincular el buffer actual.
 *
 * @param pIndexBuffer Puntero al buffer de índices.
 * @param Format Formato de los datos del índice.
 * @param Offset Desplazamiento inicial en bytes.
 */
void
DeviceContext::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
  DXGI_FORMAT Format,
  unsigned int Offset) {
  requireContext().IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

/**
 * @brief Establece el diseño de entrada (Input Layout) de los vértices para el Input Assembler.
 *
 * @note Es válido pasar nullptr para desvincular el layout actual.
 *
 * @param pInputLayout Puntero al objeto Input Layout.
 */
void
DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout) {
  requireContext().IASetInputLayout(pInputLayout);
}

/**
 * @brief Define cómo el Input Assembler procesará e interpretará los vértices (Topología).
 *
 * @param Topology Tipo de topología (ej. Lista de triángulos, tiras de líneas, etc.).
 */
void
DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) {
  requireContext().IASetPrimitiveTopology(Topology);
}

/**
 * @brief Establece el estado de rasterización en la etapa del Rasterizador.
 *
 * @note Pasar nullptr restaura el estado de rasterización por defecto de DX11.
 *
 * @param pRasterizerState Puntero al estado del rasterizador a aplicar.
 */
void
DeviceContext::RSSetState(ID3D11RasterizerState* pRasterizerState) {
  requireContext().RSSetState(pRasterizerState);
}

/**
 * @brief Vincula un Vertex Shader al pipeline gráfico.
 *
 * @note Es válido pasar nullptr en pVertexShader para desvincular el shader activo.
 *
 * @param pVertexShader Puntero al Vertex Shader.
 * @param ppClassInstances Arreglo de instancias de clase (para interfaces de shader).
 * @param NumClassInstances Número de instancias de clase provistas.
 */
void
DeviceContext::VSSetShader(ID3D11VertexShader* pVertexShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  requireContext().VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

/**
 * @brief Vincula arreglos de Constant Buffers a la etapa del Vertex Shader.
 *
 * @param StartSlot Ranura inicial de vinculación.
 * @param NumBuffers Número de buffers a vincular.
 * @param ppConstantBuffers Arreglo de punteros a los buffers constantes.
 */
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

/**
 * @brief Vincula un Pixel Shader al pipeline gráfico.
 *
 * @note Es válido pasar nullptr en pPixelShader para desvincular el shader activo.
 *
 * @param pPixelShader Puntero al Pixel Shader.
 * @param ppClassInstances Arreglo de instancias de clase.
 * @param NumClassInstances Número de instancias de clase provistas.
 */
void
DeviceContext::PSSetShader(ID3D11PixelShader* pPixelShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  requireContext().PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

/**
 * @brief Ejecuta el comando de dibujo utilizando los vértices y los índices actualmente vinculados.
 *
 * @param IndexCount Número total de índices a dibujar.
 * @param StartIndexLocation Posición del primer índice a leer en el buffer.
 * @param BaseVertexLocation Valor que se suma a cada índice antes de leer un vértice.
 */
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