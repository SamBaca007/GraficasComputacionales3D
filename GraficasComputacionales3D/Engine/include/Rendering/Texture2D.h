#pragma once
#include "Engine/Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Clase que encapsula una textura 2D en Direct3D 11.
 *
 * Gestiona tanto el recurso de textura (ID3D11Texture2D) como su vista de
 * recurso de shader (ID3D11ShaderResourceView) cuando es necesario.
 * Soporta la carga de texturas desde archivos, creación de texturas vacías
 * (para Render Targets o Depth Stencils) y la generación de Cubemaps.
 */
class
Texture2D {
public:
  /**
   * @brief Constructor por defecto.
   */
  Texture2D() noexcept = default;

  /**
   * @brief Destructor por defecto.
   */
  ~Texture2D() noexcept;

  // Se elimina la copia y asignación para evitar problemas con la liberación de punteros COM.
  Texture2D(const Texture2D&) = delete;
  Texture2D& operator=(const Texture2D&) = delete;

  // Se elimina el movimiento temporalmente en el diseño actual.
  Texture2D(Texture2D&&) = delete;
  Texture2D& operator=(Texture2D&&) = delete;

  /**
   * @brief Inicializa la textura cargándola a partir de su nombre y extensión.
   *
   * @param device Referencia al dispositivo para crear la textura.
   * @param textureName Nombre del archivo de textura.
   * @param extensionType Enumerador que define si es PNG, JPG, DDS, etc.
   * @return HRESULT S_OK si se cargó correctamente, o un código de error.
   */
  HRESULT
  init(Device& device,
       const std::string& textureName,
       ExtensionType extensionType);

  /**
   * @brief Inicializa una textura vacía con dimensiones y propiedades específicas.
   *
   * Muy útil para crear texturas de Render Target, Depth Stencil o buffers de datos.
   *
   * @param device Referencia al dispositivo.
   * @param width Ancho de la textura en píxeles.
   * @param height Alto de la textura en píxeles.
   * @param Format Formato de píxel (ej. DXGI_FORMAT_R8G8B8A8_UNORM).
   * @param BindFlags Banderas de propósito (ej. D3D11_BIND_RENDER_TARGET).
   * @param sampleCount Número de muestras para antialiasing (MSAA), por defecto 1.
   * @param qualityLevels Niveles de calidad para el MSAA, por defecto 0.
   * @return HRESULT S_OK si se creó correctamente.
   */
  HRESULT
  init(Device& device,
       unsigned int width,
       unsigned int height,
       DXGI_FORMAT Format,
       unsigned int BindFlags,
       unsigned int sampleCount = 1,
       unsigned int qualityLevels = 0);

  /**
   * @brief Inicializa la textura tomando como base otra textura existente.
   *
   * @param device Referencia al dispositivo.
   * @param textureRef Referencia a otra Texture2D existente.
   * @param format Formato DXGI que utilizará la nueva vista o textura.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  init(Device& device, Texture2D& textureRef, DXGI_FORMAT format);

  /**
   * @brief Carga y crea un recurso de textura directamente desde la ruta de una imagen.
   *
   * @param device Referencia al dispositivo.
   * @param fullPath Ruta completa del archivo en disco.
   * @param texture Objeto Texture2D que almacenará el resultado.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT
  InitTextureFromImage(Device& device,
                       const std::string& fullPath,
                       Texture2D& texture);

  /**
   * @brief Método para actualizar lógicamente la textura (actualmente en desarrollo/placeholder).
   */
  void
  update();

  /**
   * @brief Enlaza el Shader Resource View (SRV) de la textura al pipeline del Pixel Shader.
   *
   * @param deviceContext Contexto del dispositivo (DeviceContext).
   * @param StartSlot Ranura (slot) inicial para hacer el binding de la textura.
   * @param NumViews Cantidad de vistas a enlazar (generalmente 1).
   */
  void
  render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);

  /**
   * @brief Libera los recursos nativos COM asociados a esta textura.
   */
  void
  destroy();

  /**
   * @brief Crea un Cubemap (textura cúbica) a partir de 6 texturas separadas.
   *
   * Útil para implementar sistemas de Skybox o reflejos ambientales.
   *
   * @param device Referencia al dispositivo.
   * @param deviceContext Referencia al contexto del dispositivo.
   * @param facePaths Arreglo estático con las 6 rutas de las texturas para las caras del cubo.
   * @param generateMips Indica si se deben generar mipmaps automáticamente (por defecto false).
   * @return HRESULT S_OK si el Cubemap se creó exitosamente.
   */
  HRESULT
  CreateCubemap(Device& device,
                DeviceContext& deviceContext,
                const std::array<std::string, 6> facePaths,
                bool generateMips /*= false*/);

  /**
   * @brief Crea un Shader Resource View específico para una de las caras del Cubemap.
   *
   * @param device Puntero nativo al ID3D11Device.
   * @param cubemapTex Puntero a la textura que actúa como cubemap.
   * @param format Formato de datos de la textura.
   * @param faceIndex Índice de la cara a la que se apuntará (0 a 5).
   * @param mipLevels Cantidad de mipmaps (por defecto 1).
   * @return ID3D11ShaderResourceView* Puntero a la vista generada, o nullptr si falla.
   */
  ID3D11ShaderResourceView*
  CreateCubemapFaceSRV(ID3D11Device* device,
                       ID3D10Texture2D* cubemapTex, // Nota: usa ID3D10Texture2D en código base
                       DXGI_FORMAT format,
                       unsigned int faceIndex,
                       unsigned int mipLevels = 1);

  /**
   * @brief Obtiene el puntero nativo de la textura.
   * @return ID3D11Texture2D* Puntero al recurso COM de Direct3D 11.
   */
  ID3D11Texture2D* getTexture() const noexcept { return m_Texture; }

private:
  ID3D11Texture2D* m_Texture = nullptr; ///< Puntero al recurso de textura 2D nativo.
  ID3D11ShaderResourceView* m_TextureFromImg = nullptr; ///< Vista de recurso para leer la textura en los shaders (SRV).
  std::string m_textureName; ///< Nombre o identificador en cadena de la textura.
};