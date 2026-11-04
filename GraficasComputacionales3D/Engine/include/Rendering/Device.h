#pragma once
#include "Engine/Prerequisites.h"

/**
 * @brief Clase que encapsula el dispositivo de Direct3D 11 (ID3D11Device).
 *
 * El Device actúa como la "fábrica" principal de recursos. Se encarga de alojar
 * memoria en la tarjeta gráfica (VRAM) y crear objetos como buffers, texturas,
 * shaders, vistas y estados, los cuales luego son utilizados por el DeviceContext.
 */
class
Device {
public:
  /**
   * @brief Constructor por defecto.
   */
  Device() = default;

  /**
   * @brief Destructor de la clase.
   */
  ~Device();

  /**
   * @brief Inicializa el dispositivo de Direct3D 11.
   *
   * Configura los niveles de características (Feature Levels) y crea la instancia
   * nativa de ID3D11Device.
   */
  void
  init();

  /**
   * @brief Libera la memoria y destruye el dispositivo.
   */
  void
  destroy();

  /**
   * @brief Obtiene el puntero directo al recurso nativo ID3D11Device.
   * @return ID3D11Device* Puntero al dispositivo.
   */
  ID3D11Device*
  getDevice() const noexcept {
    return m_device;
  }

  /**
   * @brief Obtiene la dirección de memoria del puntero del dispositivo.
   *
   * Útil para funciones de la API de DirectX que solicitan un puntero doble
   * (void** o ID3D11Device**) durante la creación de recursos o interfaces COM.
   *
   * @return ID3D11Device** Dirección de memoria de m_device.
   */
  ID3D11Device**
  getDeviceAdress() noexcept {
    return &m_device;
  }

  /**
   * @brief Crea una vista de Render Target (RTV) para un recurso.
   *
   * Permite que el pipeline pueda dibujar sobre un recurso (generalmente una textura 2D
   * que actúa como el back buffer).
   *
   * @param pResource Puntero al recurso base (ej. el buffer extraído del SwapChain).
   * @param pDesc Descripción de la vista (puede ser nullptr para crear una vista predeterminada).
   * @param ppRTView Puntero doble donde se almacenará el RTV creado.
   * @return HRESULT S_OK si se creó exitosamente, o un código de error de lo contrario.
   */
  HRESULT
  CreateRenderTargetView(ID3D11Resource* pResource,
                         const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                         ID3D11RenderTargetView** ppRTView);

  /**
   * @brief Crea un recurso de textura 2D en la memoria gráfica.
   *
   * @param pDesc Descripción con las dimensiones, formato y propiedades de la textura.
   * @param pInitialData Datos iniciales con los que se rellenará la textura (opcional).
   * @param ppTexture2D Puntero doble donde se guardará la textura generada.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                  const D3D11_SUBRESOURCE_DATA* pInitialData,
                  ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Crea una vista de Depth/Stencil (DSV).
   *
   * Necesario para el buffer de profundidad (Z-Buffer) que determina qué píxeles
   * están delante o detrás en el espacio 3D.
   *
   * @param pResource Puntero al recurso de textura que servirá de buffer de profundidad.
   * @param pDesc Descripción de la vista (puede ser nullptr para valores por defecto).
   * @param ppDepthStencilView Puntero doble donde se almacenará el DSV creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateDepthStencilView(ID3D11Resource* pResource,
                         const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                         ID3D11DepthStencilView** ppDepthStencilView);

  /**
   * @brief Crea un programa Vertex Shader a partir de su código compilado (bytecode).
   *
   * @param pShaderBytecode Puntero al bytecode del shader compilado (.cso).
   * @param BytecodeLength Longitud en bytes del bytecode.
   * @param pClassLinkage Puntero a interfaz de enlace dinámico (generalmente nullptr).
   * @param ppVertexShader Puntero doble donde se guardará el Vertex Shader creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateVertexShader(const void* pShaderBytecode,
                     unsigned int BytecodeLength,
                     ID3D11ClassLinkage* pClassLinkage,
                     ID3D11VertexShader** ppVertexShader);

  /**
   * @brief Crea un programa Pixel Shader a partir de su código compilado (bytecode).
   *
   * @param pShaderBytecode Puntero al bytecode del shader compilado (.cso).
   * @param BytecodeLength Longitud en bytes del bytecode.
   * @param pClassLinkage Puntero a interfaz de enlace dinámico (generalmente nullptr).
   * @param ppPixelShader Puntero doble donde se guardará el Pixel Shader creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreatePixelShader(const void* pShaderBytecode,
                    unsigned int BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage,
                    ID3D11PixelShader** ppPixelShader);

  /**
   * @brief Crea un diseño de entrada (Input Layout) para los vértices.
   *
   * Define cómo se estructura la información (posición, normales, UVs) que viajará
   * desde la CPU hasta la etapa de Input Assembler en la GPU.
   *
   * @param pInputElementDescs Arreglo con la descripción de cada elemento del vértice.
   * @param NumElements Cantidad de elementos en el arreglo.
   * @param pShaderBytecodeWithInputSignature Bytecode del Vertex Shader asociado para validar la firma.
   * @param BytecodeLength Longitud del bytecode asociado.
   * @param ppInputLayout Puntero doble donde se guardará el Input Layout creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                    unsigned int NumElements,
                    const void* pShaderBytecodeWithInputSignature,
                    SIZE_T BytecodeLength,
                    ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Crea un buffer en memoria de video (VRAM).
   *
   * Puede ser un Vertex Buffer, Index Buffer o Constant Buffer dependiendo
   * de los Flags proporcionados en su descripción.
   *
   * @param pDesc Descripción de las características del buffer (tamaño, propósito).
   * @param pInitialData Datos con los que se llenará el buffer (opcional).
   * @param ppBuffer Puntero doble donde se guardará el recurso buffer creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
               const D3D11_SUBRESOURCE_DATA* pInitialData,
               ID3D11Buffer** ppBuffer);

  /**
   * @brief Crea un estado de rasterización.
   *
   * Permite definir cómo se rasteriza la geometría (ej. dibujar en modo Wireframe,
   * realizar Culling de caras traseras o frontales, etc.).
   *
   * @param pRasterizerDesc Descripción del estado de rasterización a crear.
   * @param ppRasterizerState Puntero doble donde se guardará el estado creado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
                        ID3D11RasterizerState** ppRasterizerState);

private:
  ID3D11Device* m_device = nullptr; ///< Puntero nativo al dispositivo de Direct3D 11.
  D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0; ///< Nivel de características (Feature Level) mínimo soportado (DirectX 11.0).
};