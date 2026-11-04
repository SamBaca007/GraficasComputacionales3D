#pragma once
#include "Engine/Prerequisites.h"

/**
 * @brief Clase que encapsula el contexto del dispositivo (ID3D11DeviceContext) de Direct3D 11.
 *
 * El DeviceContext es el responsable de emitir comandos de renderizado a la GPU,
 * configurar el pipeline gráfico, enlazar recursos a las distintas etapas (shaders)
 * y realizar las llamadas de dibujo (draw calls).
 */
class DeviceContext {
public:
  /**
   * @brief Constructor por defecto.
   */
  DeviceContext() = default;

  /**
   * @brief Destructor de la clase.
   */
  ~DeviceContext();

  /**
   * @brief Obtiene el puntero directo al recurso nativo ID3D11DeviceContext.
   * @return ID3D11DeviceContext* Puntero al contexto.
   */
  ID3D11DeviceContext*
  getDeviceContext() const noexcept {
    return m_DeviceContext;
  }

  /**
   * @brief Obtiene la dirección de memoria del puntero del contexto.
   * @return ID3D11DeviceContext** Dirección de memoria de m_DeviceContext.
   */
  ID3D11DeviceContext**
  getDeviceContextAdress() noexcept {
    return &m_DeviceContext;
  }

  /**
   * @brief Libera los recursos de memoria del contexto (COM release).
   */
  void
  destroy();

  /**
   * @brief Obtiene una referencia segura al contexto.
   * @return ID3D11DeviceContext& Referencia al contexto, garantizando que no sea nula.
   */
  ID3D11DeviceContext&
  requireContext() const noexcept;

  /**
   * @brief Establece uno o varios Viewports en la etapa del Rasterizador (RS).
   *
   * @param NumViewports Cantidad de viewports a enlazar.
   * @param pViewports Arreglo de estructuras D3D11_VIEWPORT con la configuración de cada uno.
   */
  void
  RSSetViewports(unsigned int NumViewports,
    const D3D11_VIEWPORT* pViewports);

  /**
   * @brief Enlaza recursos (como texturas) a la etapa del Pixel Shader (PS).
   *
   * @param StartSlot Ranura (slot) inicial para el enlace.
   * @param NumViews Número de vistas (recursos) a enlazar.
   * @param ppShaderResourceViews Arreglo de punteros a ID3D11ShaderResourceView.
   */
  void
  PSSetShaderResources(unsigned int StartSlot,
    unsigned int NumViews,
    ID3D11ShaderResourceView* const* ppShaderResourceViews);

  /**
   * @brief Enlaza los Render Targets y el Depth/Stencil al Output Merger (OM).
   *
   * @param NumViews Número de Render Targets a enlazar.
   * @param ppRenderTargetViews Arreglo de punteros a ID3D11RenderTargetView.
   * @param pDepthStencilView Puntero a la vista del Depth/Stencil.
   */
  void
  OMSetRenderTargets(unsigned int NumViews,
    ID3D11RenderTargetView* const* ppRenderTargetViews,
    ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Limpia la vista del Render Target con un color específico.
   *
   * @param pRenderTargetView Vista del Render Target a limpiar.
   * @param ColorRGBA Arreglo de 4 flotantes que define el color (R, G, B, A).
   */
  void
  ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
    const float ColorRGBA[4]);

  /**
   * @brief Limpia el buffer de profundidad (Depth) y/o el de plantilla (Stencil).
   *
   * @param pDepthStencilView Vista del Depth Stencil a limpiar.
   * @param ClearFlags Banderas que indican qué limpiar (profundidad, stencil o ambos).
   * @param Depth Valor con el que se limpiará la profundidad (típicamente 1.0f).
   * @param Stencil Valor con el que se limpiará el stencil (típicamente 0).
   */
  void
  ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
    unsigned int ClearFlags,
    float Depth,
    unsigned int Stencil);

  /**
   * @brief Actualiza dinámicamente un recurso de la GPU usando datos de la CPU.
   *
   * @param pDstResource Recurso de destino en la VRAM.
   * @param DstSubresource Índice del subrecurso a actualizar.
   * @param pDstBox Caja (Box) opcional que define la región a actualizar.
   * @param pSrcData Puntero a los datos fuente en la RAM.
   * @param SrcRowPitch Tamaño en bytes de una fila.
   * @param SrcDepthPitch Tamaño en bytes de una rebanada de profundidad.
   */
  void
  UpdateSubresource(ID3D11Resource* pDstResource,
    unsigned int DstSubresource,
    const D3D11_BOX* pDstBox,
    const void* pSrcData,
    unsigned int SrcRowPitch,
    unsigned int SrcDepthPitch);

  /**
   * @brief Enlaza buffers de vértices a la etapa del Input Assembler (IA).
   *
   * @param StartSlot Ranura inicial de enlace.
   * @param NumBuffers Cantidad de buffers de vértices a enlazar.
   * @param ppVertexBuffers Arreglo de punteros a los buffers.
   * @param pStrides Arreglo con los tamaños (en bytes) de cada elemento de los buffers.
   * @param pOffsets Arreglo con los desplazamientos (offsets) en bytes.
   */
  void
  IASetVertexBuffers(unsigned int StartSlot,
    unsigned int NumBuffers,
    ID3D11Buffer* const* ppVertexBuffers,
    const unsigned int* pStrides,
    const unsigned int* pOffsets);

  /**
   * @brief Enlaza un buffer de índices a la etapa del Input Assembler (IA).
   *
   * @param pIndexBuffer Puntero al buffer de índices.
   * @param Format Formato de los índices (ej. DXGI_FORMAT_R32_UINT).
   * @param Offset Desplazamiento inicial en bytes.
   */
  void
  IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
    DXGI_FORMAT Format,
    unsigned int Offset);

  /**
   * @brief Define la estructura de datos de entrada (Input Layout) para el Input Assembler.
   *
   * @param pInputLayout Puntero al layout configurado previamente.
   */
  void
  IASetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Define cómo el Input Assembler debe interpretar la geometría (ej. lista de triángulos).
   *
   * @param Topology Topología primitiva (ej. D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST).
   */
  void
  IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  /**
   * @brief Establece el estado del Rasterizador (modo de dibujado, culling, etc.).
   *
   * @param pRasterizerState Puntero al estado del rasterizador a aplicar.
   */
  void
  RSSetState(ID3D11RasterizerState* pRasterizerState);

  /**
   * @brief Enlaza un programa Vertex Shader a la etapa homónima del pipeline.
   *
   * @param pVertexShader Puntero al Vertex Shader compilado.
   * @param ppClassInstances Instancias de clase para dynamic linkage (generalmente nullptr).
   * @param NumClassInstances Número de instancias de clase (generalmente 0).
   */
  void
  VSSetShader(ID3D11VertexShader* pVertexShader,
    ID3D11ClassInstance* const* ppClassInstances,
    unsigned int NumClassInstances);

  /**
   * @brief Enlaza Constant Buffers a la etapa del Vertex Shader (VS).
   *
   * @param StartSlot Ranura inicial de enlace.
   * @param NumBuffers Cantidad de Constant Buffers a enlazar.
   * @param ppConstantBuffers Arreglo de punteros a los buffers.
   */
  void
  VSSetConstantBuffers(unsigned int StartSlot,
    unsigned int NumBuffers,
    ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Enlaza un programa Pixel Shader a la etapa homónima del pipeline.
   *
   * @param pPixelShader Puntero al Pixel Shader compilado.
   * @param ppClassInstances Instancias de clase para dynamic linkage (generalmente nullptr).
   * @param NumClassInstances Número de instancias de clase (generalmente 0).
   */
  void
  PSSetShader(ID3D11PixelShader* pPixelShader,
    ID3D11ClassInstance* const* ppClassInstances,
    unsigned int NumClassInstances);

  /**
   * @brief Ejecuta el comando de dibujo utilizando índices (Draw Call).
   *
   * @param IndexCount Número de índices a dibujar.
   * @param StartIndexLocation Ubicación del primer índice leído por el GPU.
   * @param BaseVertexLocation Valor sumado a cada índice antes de leer los vértices.
   */
  void
  DrawIndexed(unsigned int IndexCount,
    unsigned int StartIndexLocation,
    int BaseVertexLocation);

private:
  ID3D11DeviceContext* m_DeviceContext = nullptr; ///< Puntero nativo al DeviceContext de Direct3D.
  IDXGISwapChain* m_SwapChain = nullptr; // <- Para Present (Referencia al SwapChain para operaciones relacionadas)
};