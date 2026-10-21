// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

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

void
Texture2D::destroy() {
  if (m_Texture != nullptr) {
    SAFE_RELEASE(m_Texture);
  }
  if (m_TextureFromImg != nullptr) {
    SAFE_RELEASE(m_TextureFromImg);
  }
}

Texture2D::~Texture2D() noexcept {
  destroy();
}

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