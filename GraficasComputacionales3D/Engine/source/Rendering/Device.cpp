/**
 * @file Device.cpp
 * @brief Implementación de la clase Device, actuando como un intermediario seguro para la creación de recursos con ID3D11Device.
 */

#include "Rendering/Device.h"

 /**
  * @brief Destructor de la clase Device.
  *
  * Llama internamente al método destroy() para asegurar la liberación del
  * puntero COM m_device sin fugas de memoria.
  */
Device::~Device() {
  destroy(); // Como ya tienes un método destroy que libera m_device, lo reutilizamos.
}

/**
 * @brief Inicializa el dispositivo de Direct3D 11.
 *
 * @note Actualmente la lógica de creación nativa (D3D11CreateDevice) puede
 *       estar siendo ejecutada externamente.
 */
void
Device::init() {
  // Aquí irá la lógica para crear el dispositivo (D3D11CreateDevice o D3D11CreateDeviceAndSwapChain)
  // Si por ahora la inicialización la haces desde otro lado, déjalo vacío o pon un mensaje:
  MESSAGE("Device", "init", "Device initialized.");
}

/**
 * @brief Libera los recursos de memoria del dispositivo.
 *
 * Utiliza la macro SafeRelease para liberar de forma segura el puntero COM.
 */
void
Device::destroy() {
  SafeRelease(m_device);
}

/**
 * @brief Crea una vista de destino de renderizado (Render Target View).
 *
 * @param pResource Puntero al recurso del cual se creará la vista (usualmente el back buffer).
 * @param pDesc Puntero a la descripción de la vista (puede ser nullptr para valores por defecto).
 * @param ppRTView Puntero doble donde se almacenará el objeto de vista creado.
 * @return HRESULT S_OK si se creó la vista exitosamente, o un código de error detallado en caso contrario.
 */
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

/**
 * @brief Crea una textura 2D en la memoria gráfica.
 *
 * @param pDesc Puntero a la descripción completa de las propiedades de la textura.
 * @param pInitialData Puntero opcional a los datos iniciales con los que se llenará la textura.
 * @param ppTexture2D Puntero doble donde se almacenará el recurso de la textura creada.
 * @return HRESULT S_OK si se creó con éxito.
 */
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

/**
 * @brief Crea una vista para el buffer de profundidad y estarcido (Depth Stencil View).
 *
 * @param pResource Puntero al recurso de textura base.
 * @param pDesc Puntero a la descripción de la vista. Puede ser nullptr para utilizar el formato base.
 * @param ppDepthStencilView Puntero doble donde se almacenará la vista generada.
 * @return HRESULT S_OK si la operación es exitosa.
 */
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

/**
 * @brief Crea un objeto Vertex Shader a partir del código bytecode proporcionado.
 *
 * @param pShaderBytecode Puntero al arreglo de bytes del shader ya compilado.
 * @param BytecodeLength Longitud del arreglo en bytes.
 * @param pClassLinkage Puntero de enlace de clases dinámico (generalmente nullptr).
 * @param ppVertexShader Puntero doble donde se guardará el Vertex Shader creado.
 * @return HRESULT S_OK si se completó exitosamente.
 */
HRESULT
Device::CreateVertexShader(const void* pShaderBytecode,
                           unsigned int BytecodeLength,
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

/**
 * @brief Crea un objeto Pixel Shader a partir del código bytecode proporcionado.
 *
 * @param pShaderBytecode Puntero al arreglo de bytes del shader ya compilado.
 * @param BytecodeLength Longitud del arreglo en bytes.
 * @param pClassLinkage Puntero de enlace de clases dinámico (generalmente nullptr).
 * @param ppPixelShader Puntero doble donde se guardará el Pixel Shader creado.
 * @return HRESULT S_OK si se completó exitosamente.
 */
HRESULT
Device::CreatePixelShader(const void* pShaderBytecode,
                          unsigned int BytecodeLength,
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

/**
 * @brief Construye la estructura de entrada de los vértices (Input Layout) basándose en la firma de un Vertex Shader.
 *
 * @param pInputElementDescs Arreglo que describe los componentes de entrada (Posición, Color, UV, etc.).
 * @param NumElements Cantidad de elementos descritos en el arreglo.
 * @param pShaderBytecodeWithInputSignature Puntero al bytecode del Vertex Shader que validará este layout.
 * @param BytecodeLength Longitud del bytecode de validación.
 * @param ppInputLayout Puntero doble donde se guardará el Layout creado.
 * @return HRESULT S_OK si el layout es válido y se ha creado.
 */
HRESULT
Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                          unsigned int NumElements,
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

  HRESULT result = m_device->CreateInputLayout(pInputElementDescs,
    NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);

  if (FAILED(result)) {
    ERROR("Device", "CreateInputLayout",
      ("Failed to create input layout. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("Device", "CreateInputLayout", "SUCCESS");
  return result;
}

/**
 * @brief Genera un recurso tipo buffer de memoria (Vertex Buffer, Index Buffer, Constant Buffer).
 *
 * @param pDesc Puntero a la estructura que detalla el uso, tamaño y tipo de buffer.
 * @param pInitialData Puntero opcional con los datos para rellenar el buffer al momento de crearlo.
 * @param ppBuffer Puntero doble donde se guardará el buffer creado.
 * @return HRESULT S_OK si el buffer se generó sin errores.
 */
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

/**
 * @brief Crea un objeto que encapsula un conjunto de estados del rasterizador.
 *
 * @param pRasterizerDesc Puntero a la descripción del estado (Culling, modo de relleno, etc.).
 * @param ppRasterizerState Puntero doble donde se guardará el objeto Rasterizer State creado.
 * @return HRESULT S_OK si el estado fue inicializado correctamente.
 */
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