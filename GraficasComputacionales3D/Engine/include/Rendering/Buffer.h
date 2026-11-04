#pragma once
#include "Engine/Prerequisites.h"
#include "Engine/MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @brief Clase que representa y administra un Buffer de Direct3D 11.
 *
 * Facilita la creación, actualización y enlace (binding) de diferentes tipos
 * de buffers (Vertex, Index, Constant) en el pipeline gráfico.
 */
class Buffer {
public:

  /**
   * @brief Constructor por defecto.
   */
  Buffer() = default;

  /**
   * @brief Destructor por defecto.
   */
  ~Buffer() = default;

  /**
   * @brief Inicializa un buffer a partir de la información de un MeshComponent.
   *
   * Usualmente empleado para inicializar Vertex Buffers o Index Buffers.
   *
   * @param device Referencia al dispositivo (Device) que creará el buffer.
   * @param mesh Componente de malla (MeshComponent) que contiene los datos (vértices/índices).
   * @param bindFlag Bandera de Direct3D 11 (D3D11_BIND_FLAG) que indica el propósito del buffer.
   * @return HRESULT Código de resultado de la operación (S_OK si fue exitoso).
   */
  HRESULT
  init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

  /**
   * @brief Inicializa un buffer vacío especificando únicamente su tamaño.
   *
   * Usualmente empleado para inicializar Constant Buffers.
   *
   * @param device Referencia al dispositivo (Device) que creará el buffer.
   * @param ByteWidth Tamaño del buffer en bytes.
   * @return HRESULT Código de resultado de la operación (S_OK si fue exitoso).
   */
  HRESULT
  init(Device& device, unsigned int ByteWidth);

  /**
   * @brief Actualiza los datos del buffer enviando nueva información a la VRAM.
   *
   * Actúa como un wrapper de UpdateSubresource de Direct3D 11.
   *
   * @param deviceContext Contexto del dispositivo usado para la actualización.
   * @param pDstResource Recurso de destino (generalmente m_buffer).
   * @param DstSubresource Índice del subrecurso de destino.
   * @param pDstBox Caja (Box) que define la porción a actualizar (puede ser nullptr).
   * @param pSrcData Puntero a los datos fuente en RAM.
   * @param SrcRowPitch Tamaño en bytes de una fila de datos.
   * @param SrcDepthPitch Tamaño en bytes de un slice de profundidad.
   */
  void
  update(DeviceContext& deviceContext,
      ID3D11Resource* pDstResource,
      unsigned int DstSubresource,
      const D3D11_BOX* pDstBox,
      const void* pSrcData,
      unsigned int SrcRowPitch,
      unsigned int SrcDepthPitch);

  /**
   * @brief Enlaza el buffer al pipeline gráfico para el renderizado.
   *
   * Dependiendo del bindFlag interno, establecerá el buffer en la etapa correspondiente
   * (IA para Vertex/Index Buffers, o VS/PS para Constant Buffers).
   *
   * @param deviceContext Contexto del dispositivo (DeviceContext).
   * @param startSlot Ranura (slot) inicial donde se enlazará el buffer.
   * @param numBuffers Número de buffers a enlazar.
   * @param setPixelShader Si es true, y es un Constant Buffer, lo enlazará al Pixel Shader.
   * @param format Formato DXGI usado (necesario principalmente para Index Buffers).
   */
  void
  render(DeviceContext& deviceContext,
      unsigned int startSlot,
      unsigned int numBuffers,
      bool setPixelShader = false,
      DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

  /**
   * @brief Libera la memoria del buffer y destruye el recurso de Direct3D.
   */
  void
  destroy();

  /**
   * @brief Crea directamente el buffer de D3D11 usando su descripción y datos iniciales.
   *
   * @param device Referencia al dispositivo (Device).
   * @param desc Descripción completa del buffer (D3D11_BUFFER_DESC).
   * @param initData Datos iniciales a cargar en el buffer (puede ser nullptr).
   * @return HRESULT Código de resultado de la operación.
   */
  HRESULT
  CreateBuffer(Device& device,
      D3D11_BUFFER_DESC& desc,
      D3D11_SUBRESOURCE_DATA* initData);

  /**
   * @brief Obtiene el puntero directo al recurso ID3D11Buffer.
   * @return Puntero a ID3D11Buffer.
   */
  ID3D11Buffer*
  getBuffer() { return m_buffer; };

  /**
   * @brief Obtiene la dirección de memoria del puntero del buffer (ID3D11Buffer**).
   * @return Puntero al puntero de ID3D11Buffer.
   */
  ID3D11Buffer**
  getBufferAddress() { return &m_buffer; }

public:
ID3D11Buffer* m_buffer = nullptr; ///< Puntero nativo al recurso de buffer de Direct3D 11.

private:
  unsigned int m_stride = 0;   ///< Tamaño en bytes de un elemento individual en el buffer.
  unsigned int m_offset = 0;   ///< Desplazamiento inicial (offset) en bytes para su lectura.
  unsigned int m_bindFlag = 0; ///< Bandera que define el tipo de buffer (D3D11_BIND_FLAG).
};