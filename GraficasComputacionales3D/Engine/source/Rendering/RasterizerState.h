#pragma once
#include "Engine/Prerequisites.h"
#include "Rendering/Device.h"

/**
 * @brief Clase que encapsula el estado de rasterización (ID3D11RasterizerState).
 *
 * Define el comportamiento de la etapa de rasterizado, como el modo de relleno (sólido/wireframe)
 * y el culling (ocultamiento de caras traseras).
 */
class
RasterizerState {
public:
  RasterizerState() = default;
  ~RasterizerState();

  /**
   * @brief Inicializa el estado de rasterización.
   *
   * @param device Referencia al dispositivo para la creación.
   * @param pDesc Puntero a la descripción del estado (Culling, modo de relleno, etc.).
   * @return HRESULT S_OK si se creó correctamente.
   */
  HRESULT
  init(Device& device, const D3D11_RASTERIZER_DESC* pDesc);

  /**
   * @brief Libera la memoria del estado.
   */
  void
  destroy();

  /**
   * @brief Obtiene el puntero nativo al estado de rasterización.
   */
  ID3D11RasterizerState*
  getState() const noexcept {
    return m_rasterizerState;
  }

  /**
   * @brief Obtiene la dirección de memoria del estado.
   */
  ID3D11RasterizerState**
  getStateAddress() noexcept {
    return &m_rasterizerState;
  }

private:
  ID3D11RasterizerState* m_rasterizerState = nullptr;
};