#pragma once
#include "Engine/Prerequisites.h"
#include "Rendering/Device.h"

/**
 * @brief Clase que encapsula la vista de profundidad y esténcil (ID3D11DepthStencilView).
 *
 * Necesario para manejar el Z-Buffer y asegurar que la geometría se dibuje
 * en el orden correcto de profundidad.
 */
class
DepthStencilView {
public:
  DepthStencilView() = default;
  ~DepthStencilView();

  /**
   * @brief Inicializa la vista de Depth/Stencil.
   *
   * @param device Referencia al dispositivo para la creación.
   * @param pResource Puntero al recurso de textura base (Depth Stencil Buffer).
   * @param pDesc Descripción de la vista (puede ser nullptr).
   * @return HRESULT S_OK si se creó correctamente.
   */
  HRESULT
  init(Device& device, ID3D11Resource* pResource,
    const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc = nullptr);

  /**
   * @brief Libera la memoria de la vista.
   */
  void
  destroy();

  /**
   * @brief Obtiene el puntero nativo al Depth Stencil View.
   */
  ID3D11DepthStencilView*
  getDSV() const noexcept {
    return m_dsv;
  }

  /**
   * @brief Obtiene la dirección de memoria del DSV.
   */
  ID3D11DepthStencilView**
  getDSVAddress() noexcept {
    return &m_dsv;
  }

private:
  ID3D11DepthStencilView* m_dsv = nullptr;
};