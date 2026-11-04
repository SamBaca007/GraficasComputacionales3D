/**
 * @file Texture2D.cpp
 * @brief Implementación de la clase Texture2D para la gestión de texturas en Direct3D 11.
 */

 // #define STB_IMAGE_IMPLEMENTATION
 // #include "stb_image.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

/**
 * @brief Inicializa la textura a partir de un nombre de archivo y su extensión.
 *
 * Valida los parámetros y delega la carga real de la imagen al método
 * InitTextureFromImage, concatenando la extensión adecuada al nombre base.
 *
 * @param device Referencia al dispositivo de Direct3D.
 * @param TextureName Nombre base del archivo de textura (sin extensión).
 * @param extensionType Tipo de extensión de la imagen (PNG, JPG, DDS).
 * @return HRESULT S_OK si se procesó correctamente, o un código de error de lo contrario.
 */
HRESULT
Texture2D::init(Device& device,
  const std::string& TextureName,
  ExtensionType extensionType) {
  if (!device.getDevice()) {
    ERROR("Texture", "init", "Device is null");
    return E_POINTER;
  }
  if (TextureName.empty()) {
    ERROR("Texture", "init", "Texture name cannot be empty");
    return E_INVALIDARG;
  }

  HRESULT hr = S_OK;

  switch (extensionType) {
  case DDS:
    // Lógica para DDS pendiente
    break;
  case PNG: {
    m_textureName = TextureName + ".png";
    hr = InitTextureFromImage(device, m_textureName, *this);
    break;
  }
  case JPG: {
    m_textureName = TextureName + ".jpg";
    hr = InitTextureFromImage(device, m_textureName, *this);
    break;
  }
  default:
    ERROR("Texture", "init", "Unsupported extension type");
    return E_INVALIDARG;
  }

  return hr;

}

/**
 * @brief Inicializa una textura desde cero configurando su descriptor internamente.
 *
 * Este método es fundamental para crear Render Targets, Depth Stencils o
 * cualquier textura que no provenga directamente de un archivo en disco.
 *
 * @param device Referencia al dispositivo (Device).
 * @param width Ancho en píxeles de la textura.
 * @param height Alto en píxeles de la textura.
 * @param Format Formato de datos de la textura (DXGI_FORMAT).
 * @param BindFlags Propósito de la textura (D3D11_BIND_RENDER_TARGET, D3D11_BIND_DEPTH_STENCIL, etc.).
 * @param SampleCount Cantidad de muestras para MSAA (Multisample Anti-Aliasing).
 * @param QualityLevels Nivel de calidad para el MSAA.
 * @return HRESULT S_OK en caso de creación exitosa, código de error en fallo.
 */
HRESULT
Texture2D::init(Device& device,
  unsigned int width,
  unsigned int height,
  DXGI_FORMAT Format,
  unsigned int BindFlags,
  unsigned int SampleCount,
  unsigned int QualityLevels) {
  if (!device.getDevice()) {
    ERROR("Texture", "init", "Device is null");
    return E_POINTER;
  }
  if (width == 0 || height == 0) {
    ERROR("Texture", "init", "Width and height must be greater than 0");
    return E_INVALIDARG;
  }

  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = Format;
  desc.SampleDesc.Count = SampleCount;
  desc.SampleDesc.Quality = QualityLevels;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = BindFlags;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  HRESULT hr = device.CreateTexture2D(&desc, nullptr, &m_Texture);

  if (FAILED(hr)) {
    ERROR("Texture", "init",
      ("Failed to create texture with specified params. RESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

/**
 * @brief Enlaza la vista del recurso de la textura (SRV) al Pixel Shader.
 *
 * @param deviceContext Contexto del dispositivo responsable de los comandos de renderizado.
 * @param StartSlot Ranura (slot) de textura en el shader donde se enlazará (generalmente 0).
 * @param NumViews Cantidad de texturas que se están enlazando (generalmente 1).
 */
void
Texture2D::render(DeviceContext& deviceContext,
  unsigned int StartSlot,
  unsigned int NumViews) {
  if (!deviceContext.getDeviceContext()) {
    ERROR("Texture", "render", "Device Context is mull.");
    return;
  }

  if (m_TextureFromImg) {
    deviceContext.PSSetShaderResources(StartSlot, NumViews, &m_TextureFromImg);
  }
}

/**
 * @brief Libera los recursos nativos y vistas creadas asociadas a la textura.
 */
void
Texture2D::destroy() {
  if (m_Texture != nullptr) {
    SAFE_RELEASE(m_Texture);
  }
  if (m_TextureFromImg != nullptr) {
    SAFE_RELEASE(m_TextureFromImg);
  }
}

/**
 * @brief Destructor de la clase, llama internamente a destroy().
 */
Texture2D::~Texture2D() noexcept {
  destroy();
}

/**
 * @brief Carga una imagen del disco y la convierte en una textura de Direct3D.
 *
 * @note Actualmente esta función es un placeholder. Retorna E_NOTIMPL para
 *       evitar errores de linker (LNK2019) hasta que se integre la librería
 *       correspondiente (ej. stb_image).
 *
 * @param device Referencia al dispositivo.
 * @param fullPath Ruta completa al archivo de imagen.
 * @param texture Objeto Texture2D donde se guardará el resultado de la carga.
 * @return HRESULT E_NOTIMPL debido a que la implementación no está completa.
 */
HRESULT
Texture2D::InitTextureFromImage(Device& device,
  const std::string& fullPath,
  Texture2D& texture) {
  // IMPORTANTE: Tu profesor dejó comentadas las líneas de "stb_image.h" al inicio.
  // Es muy probable que él tenga el código real para cargar la imagen que va aquí adentro.
  // Por el momento, retornaremos E_NOTIMPL (Error: No implementado) para que 
  // tu proyecto compile sin los errores LNK2019.

  ERROR("Texture2D", "InitTextureFromImage", "Function not yet implemented.");
  return E_NOTIMPL; // Retorna un código de que aún no está hecha esta parte
}