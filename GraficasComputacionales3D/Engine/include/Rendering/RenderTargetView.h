#pragma once
#include "Engine/Prerequisites.h"
#include "Rendering/Device.h"

/**
 * @brief Clase que encapsula la vista de destino de renderizado (ID3D11RenderTargetView).
 *
 * Permite a la pipeline gráfica saber dónde escribir los colores finales (por ejemplo, el backbuffer).
 */
class
RenderTargetView {
public:
  RenderTargetView() = default;
  ~RenderTargetView();

  /**
   * @brief Inicializa el Render Target View usando un recurso base.
   *
   * @param device Referencia al dispositivo (abstracción) para crearlo.
   * @param pResource Puntero al recurso en memoria (ej. textura 2D del SwapChain).
   * @param pDesc Descripción opcional de la vista.
   * @return HRESULT S_OK si tuvo éxito.
   */
  HRESULT
  init(Device& device, ID3D11Resource* pResource,
  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc = nullptr);

  /**
   * @brief Libera la memoria de la vista.
   */
  void
  destroy();

  /**
   * @brief Obtiene el puntero nativo al RTV.
   */
  ID3D11RenderTargetView*
  getRTV() const noexcept {
    return m_rtv;
  }

  /**
   * @brief Obtiene la dirección de memoria del RTV (útil para OMSetRenderTargets).
   */
  ID3D11RenderTargetView**
  getRTVAddress() noexcept {
    return &m_rtv;
  }

private:
  ID3D11RenderTargetView* m_rtv = nullptr;
};