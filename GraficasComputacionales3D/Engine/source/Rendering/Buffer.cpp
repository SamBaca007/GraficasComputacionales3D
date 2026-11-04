/**
 * @file Buffer.cpp
 * @brief Implementación de la clase Buffer para la creación y gestión de recursos en Direct3D 11.
 */

#include "Rendering/Buffer.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

 /**
  * @brief Inicializa el buffer a partir de un MeshComponent.
  *
  * Valida que el dispositivo sea válido y que el MeshComponent contenga datos
  * según el flag especificado (vértices o índices). Configura la descripción
  * del buffer y llama a CreateBuffer.
  *
  * @param device Referencia al dispositivo de Direct3D.
  * @param mesh Componente de malla que contiene la geometría.
  * @param bindFlag Propósito del buffer (D3D11_BIND_VERTEX_BUFFER, D3D11_BIND_INDEX_BUFFER, etc.).
  * @return HRESULT S_OK si se creó correctamente, o un código de error si fallaron las validaciones.
  */
HRESULT
Buffer::init(Device& device, const MeshComponent& mesh, unsigned int bindFlag) {
  if (!device.getDevice()) {
    ERROR("ShaderProgram", "init", "Device is null.");
    return E_POINTER;
  }
  if ((bindFlag & D3D11_BIND_VERTEX_BUFFER) && mesh.m_vertex.empty() /*&& mesh.m_skyVertex.empty()*/) {
    ERROR("Buffer", "init", "Vertex buffer is empty.");
    return E_INVALIDARG;
  }
  if ((bindFlag & D3D11_BIND_INDEX_BUFFER) && mesh.m_index.empty()) {
    ERROR("Buffer", "init", "Index buffer is empty.");
    return E_INVALIDARG;
  }

  D3D11_BUFFER_DESC desc = {};
  D3D11_SUBRESOURCE_DATA data = {};

  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.CPUAccessFlags = 0;
  m_bindFlag = bindFlag;
  desc.BindFlags = (D3D11_BIND_FLAG)bindFlag;

  // NOTA: D3D11_USAGE_DEFAULT no es un bindFlag, probablemente aquí se pretendía 
  // verificar D3D11_BIND_VERTEX_BUFFER. Se mantiene tal cual tu código base.
  if (bindFlag & D3D11_USAGE_DEFAULT) {
    if (/*mesh.m_skyVertex.size() > 0 && */mesh.m_vertex.size() == 0) {
      /*m_stride = sizeof(SkyboxVertex);
      desc.ByteWidth = m_stride * static_cast<unsigned int>(mesh.m_skyVertex.size());
      data.pSysMem = mesh.m_skyVertex.data();*/
    }
    else {
      m_stride = sizeof(Vertex);
      desc.ByteWidth = m_stride * static_cast<unsigned int>(mesh.m_vertex.size());
      data.pSysMem = mesh.m_vertex.data();
    }
  }
  else if (bindFlag & D3D11_BIND_INDEX_BUFFER) {
    m_stride = sizeof(unsigned int);
    desc.ByteWidth = m_stride * static_cast<unsigned int>(mesh.m_index.size());
    data.pSysMem = mesh.m_index.data();
  }

  return CreateBuffer(device, desc, &data);
}

/**
 * @brief Inicializa un buffer vacío reservando su tamaño en memoria (usado para Constant Buffers).
 *
 * @param device Referencia al dispositivo de Direct3D.
 * @param ByteWidth Tamaño total en bytes que se reservará para el buffer.
 * @return HRESULT S_OK en caso de éxito, o un código de error de D3D11.
 */
HRESULT
Buffer::init(Device& device, unsigned int ByteWidth) {
  if (!device.getDevice()) {
    ERROR("ShaderProgram", "init", "Device is null.");
    return E_POINTER;
  }
  if (ByteWidth == 0) {
    ERROR("Buffer", "init", "ByteWidth is zero.");
    return E_INVALIDARG;
  }
  m_stride = ByteWidth;

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = ByteWidth;
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  m_bindFlag = desc.BindFlags;

  return CreateBuffer(device, desc, nullptr);
}

/**
 * @brief Actualiza dinámicamente el contenido del buffer en la VRAM.
 *
 * Emplea UpdateSubresource del DeviceContext para transferir memoria de la RAM
 * hacia la memoria gráfica de la GPU.
 *
 * @param deviceContext Contexto del dispositivo responsable de la actualización.
 * @param pDstResource Recurso destino (generalmente m_buffer, no se usa explícitamente en el interior porque usa m_buffer por defecto).
 * @param DstSubresource Índice del subrecurso a actualizar (usualmente 0).
 * @param pDstBox Caja 3D opcional que define la porción a actualizar.
 * @param pSrcData Puntero a la memoria fuente en la CPU.
 * @param SrcRowPitch Tamaño en bytes de una fila (para texturas/2D).
 * @param SrcDepthPitch Tamaño en bytes de un corte de profundidad (para 3D).
 */
void
Buffer::update(DeviceContext& deviceContext,
               ID3D11Resource* pDstResource,
               unsigned int DstSubresource,
               const D3D11_BOX* pDstBox,
               const void* pSrcData,
               unsigned int SrcRowPitch,
               unsigned int SrcDepthPitch) {
  if (!m_buffer) {
    ERROR("ShaderProgram", "update", "m_buffer is null.");
    return;
  }
  if (!pSrcData) {
    ERROR("ShaderProgram", "update", "pSrcData is null.");
    return;
  }

  deviceContext.getDeviceContext()->UpdateSubresource(m_buffer,
    DstSubresource,
    pDstBox,
    pSrcData,
    SrcRowPitch,
    SrcDepthPitch);
}

/**
 * @brief Enlaza el buffer al pipeline de Direct3D 11 basándose en su tipo (m_bindFlag).
 *
 * Llama a la función adecuada de la API (IASetVertexBuffers, VSSetConstantBuffers,
 * PSSetConstantBuffers o IASetIndexBuffer) dependiendo del propósito de este buffer.
 *
 * @param deviceContext Contexto del dispositivo utilizado para enlazar.
 * @param startSlot Slot inicial para el enlazado (útil para múltiples buffers).
 * @param numBuffers Cantidad de buffers a enlazar (generalmente 1).
 * @param setPixelShader Determina si también se enlaza al Pixel Shader (sólo para Constant Buffers).
 * @param format Formato de los datos, necesario al enlazar Index Buffers (ej. DXGI_FORMAT_R32_UINT).
 */
void
Buffer::render(DeviceContext& deviceContext,
               unsigned int startSlot,
               unsigned int numBuffers,
               bool setPixelShader,
               DXGI_FORMAT format) {
  if (!deviceContext.getDeviceContext()) {
    ERROR("RenderTargetView", "render", "Device Context is nullptr.");
    return;
  }
  if (!m_buffer) {
    ERROR("Buffer", "render", "m_buffer is null.");
    return;
  }

  switch (m_bindFlag) {
  case D3D11_BIND_VERTEX_BUFFER:
    deviceContext.getDeviceContext()->IASetVertexBuffers(startSlot, numBuffers, &m_buffer,
      &m_stride, &m_offset);
    break;
  case D3D11_BIND_CONSTANT_BUFFER:
    deviceContext.getDeviceContext()->VSSetConstantBuffers(startSlot, numBuffers, &m_buffer);
    if (setPixelShader) {
      deviceContext.getDeviceContext()->PSSetConstantBuffers(startSlot, numBuffers, &m_buffer);
    }
    break;
  case D3D11_BIND_INDEX_BUFFER:
    deviceContext.getDeviceContext()->IASetIndexBuffer(m_buffer, format, m_offset);
    break;
  default:
    ERROR("Buffer", "render", "Unsupportes BindFlag"); // Nota: Tienes un pequeño typo en el error original ("Unsupportes"). Lo dejé igual para respetar tu código base.
    break;
  }
}

/**
 * @brief Libera de forma segura el recurso del buffer mediante la macro SAFE_RELEASE.
 */
void
Buffer::destroy() {
  SAFE_RELEASE(m_buffer);
}

/**
 * @brief Método interno que interactúa con la API de D3D11 para construir el buffer.
 *
 * @param device Dispositivo con el que se realizará la llamada a CreateBuffer.
 * @param desc Estructura descriptiva que dicta los parámetros del buffer.
 * @param initData Estructura con la información inicial de llenado, puede ser nullptr.
 * @return HRESULT S_OK si la API logra instanciar el recurso.
 */
HRESULT
Buffer::CreateBuffer(Device& device,
                     D3D11_BUFFER_DESC& desc,
                     D3D11_SUBRESOURCE_DATA* initData) {
  if (!device.getDevice()) {
    ERROR("Buffer", "createBuffer", "Device is nullptr.");
    return E_POINTER;
  }

  HRESULT hr = device.getDevice()->CreateBuffer(&desc, initData, &m_buffer);
  if (FAILED(hr)) {
    ERROR("Buffer", "createBuffer", "Failed to create buffer");
    return hr;
  }
  return S_OK;
}