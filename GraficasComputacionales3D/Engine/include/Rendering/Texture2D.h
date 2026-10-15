#pragma once
#include "Engine/Prerequisites.h"

class
  Texture2D {
public:
  Texture2D() = default;
  ~Texture2D();

  /**
   * @brief Devuelve el puntero directo al recurso de textura.
   * Utilizado para vincularlo a Vistas (ej. RenderTargetView, DepthStencilView).
   */
  ID3D11Texture2D*
    getTexture() const noexcept
  {
    return m_Texture;
  }

  /**
   * @brief Devuelve la dirección de memoria del puntero.
   * Utilizado al momento de crear la textura desde el Device o SwapChain.
   */
  ID3D11Texture2D**
    getTextureAdress() noexcept
  {
    return &m_Texture;
  }

  /**
   * @brief Libera el recurso de la GPU de forma segura.
   */
  void
  destroy();

  /**
   * @brief Valida que la textura exista antes de usarla.
   */
  ID3D11Texture2D& requireTexture() const noexcept;

private:
  ID3D11Texture2D* m_Texture = nullptr;
};