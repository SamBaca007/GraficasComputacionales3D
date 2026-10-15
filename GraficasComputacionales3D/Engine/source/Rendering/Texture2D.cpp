#include "Rendering/Texture2D.h"

void
Texture2D::destroy() {
  SafeRelease(m_Texture);
}

Texture2D::~Texture2D() {
  destroy();
}

ID3D11Texture2D&
Texture2D::requireTexture() const noexcept {
  if (!m_Texture) {
    ERROR("Texture2D", "RequireTexture", "Texture resource is not initialized.");
  }
  return *m_Texture;
}