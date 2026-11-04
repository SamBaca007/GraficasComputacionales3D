#pragma once
#include "Engine/Prerequisites.h"

class Device;
class Texture2D;

/**
 * @brief Clase que encapsula la funcionalidad del SwapChain (Cadena de intercambio) de DXGI.
 *
 * El SwapChain gestiona el intercambio de buffers (back buffer y front buffer)
 * para presentar la imagen renderizada en la ventana del sistema operativo
 * sin desgarro de pantalla (tearing).
 */
class SwapChain {
public:
  /**
   * @brief Constructor por defecto.
   */
  SwapChain() = default;

  /**
   * @brief Destructor de la clase. Llama internamente a destroy() para liberar recursos.
   */
  ~SwapChain();

  /**
   * @brief Obtiene el puntero directo al recurso IDXGISwapChain nativo.
   * @return IDXGISwapChain* Puntero a la cadena de intercambio.
   */
  IDXGISwapChain*
  getSwapChain() const noexcept {
    return m_SwapChain;
  }

  /**
   * @brief Obtiene la dirección de memoria del puntero del SwapChain.
   *
   * Útil cuando las funciones de creación de la API de DirectX requieren un puntero doble.
   * @return IDXGISwapChain** Dirección de memoria del puntero m_SwapChain.
   */
  IDXGISwapChain**
  getSwapChainAdress() noexcept {
    return &m_SwapChain;
  }

  // Se elimina la copia y asignación para prevenir liberación múltiple del puntero COM.
  SwapChain(const SwapChain&) = delete;
  SwapChain& operator = (const SwapChain&) = delete;

  // Se elimina el movimiento por el diseño actual de la clase.
  SwapChain(SwapChain&&) = delete;
  SwapChain& operator = (SwapChain&&) = delete;

  /**
   * @brief Inicializa el SwapChain configurando sus descriptores y vinculándolo a la ventana.
   *
   * @param device Objeto Device utilizado para la creación (Nota: usualmente se pasa por referencia).
   * @param windowHandle Manejador (HWND) de la ventana de Windows donde se dibujará.
   * @param width Ancho inicial del back buffer.
   * @param height Alto inicial del back buffer.
   * @return HRESULT S_OK si la creación del SwapChain fue exitosa, o un código de error.
   */
  HRESULT
  init(Device device, HWND windowHandle, unsigned int width, unsigned int height);

  /**
   * @brief Libera de manera segura la memoria del recurso COM del SwapChain.
   */
  void
  destroy();

  /**
   * @brief Accede a uno de los buffers del SwapChain (generalmente el Back Buffer).
   *
   * Actúa como wrapper de la función IDXGISwapChain::GetBuffer.
   *
   * @param Buffer Índice del buffer al que se desea acceder (0 para el back buffer actual).
   * @param riid Identificador de la interfaz (REFIID) solicitada (ej. __uuidof(ID3D11Texture2D)).
   * @param ppSurface Puntero doble donde se guardará la superficie obtenida.
   * @return HRESULT S_OK en caso de éxito.
   */
  HRESULT GetBuffer(unsigned int Buffer,
    REFIID riid,
    void** ppSurface);

  /**
   * @brief Presenta la imagen renderizada en la pantalla intercambiando los buffers.
   *
   * Actúa como wrapper de la función IDXGISwapChain::Present.
   *
   * @param SyncInterval Intervalo de sincronización vertical (VSync). 0 = sin VSync, 1 = activado.
   * @param Flags Opciones adicionales para la presentación de los frames.
   * @return HRESULT S_OK si el frame se presentó correctamente.
   */
  HRESULT
  Present(unsigned int SyncInterval,
    unsigned int Flags);

  /**
   * @brief Redimensiona los buffers del SwapChain, útil cuando la ventana cambia de tamaño.
   *
   * @param width Nuevo ancho de los buffers.
   * @param height Nuevo alto de los buffers.
   * @return HRESULT S_OK si la operación de redimensionado fue exitosa.
   */
  HRESULT
  ResizeBuffers(unsigned int width,
    unsigned int height);

private:
  IDXGISwapChain* m_SwapChain = nullptr; ///< Puntero nativo al recurso SwapChain de DXGI.
};