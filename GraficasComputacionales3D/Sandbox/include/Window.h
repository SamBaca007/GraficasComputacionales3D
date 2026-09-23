/**
 * @file Window.h
 * @brief Encapsula la creación, gestión y procesamiento de eventos de una ventana en Win32.
 *
 * Define la clase Window, encargada de registrar la clase de ventana del sistema operativo,
 * instanciarla y manejar su ciclo de vida y bucle de mensajes.
 */
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
 /**
  * @class Window
  * @brief Clase final que representa y gestiona la ventana principal del motor.
  *
  * Esta clase maneja la interacción directa con la API de Win32 para la ventana.
  * Está diseñada para no ser copiable ni heredable.
  */
class Window final {
public:
  /**
   * @brief Constructor por defecto.
   */
  Window() = default;
  /**
   * @brief Destructor de la ventana. Asegura la correcta destrucción de los recursos.
   */
  ~Window();
  /** @name Constructores eliminados
   *  Evitan la copia de la clase para garantizar que el Handle (HWND) sea único.
   */
   ///@{
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ///@}

  /**
   * @brief Crea e inicializa la ventana de Windows.
   *
   * Registra la clase de ventana en el sistema (si no lo está ya) y crea la instancia
   * ajustando el tamaño del área cliente al tamaño especificado.
   *
   * @param instance Handle de la instancia de la aplicación (HINSTANCE).
   * @param title Título que se mostrará en la barra superior de la ventana.
   * @param clientWidth Ancho deseado para el área cliente (sin bordes) en píxeles.
   * @param clientHeight Alto deseado para el área cliente (sin bordes) en píxeles.
   * @return true si la ventana se creó exitosamente, false en caso de error.
   */
  bool
    Create(HINSTANCE instance, const wchar_t* title,
      UINT clientWidth, UINT clientHeight) noexcept;
  /**
   * @brief Muestra u oculta la ventana según el comando especificado.
   *
   * @param showCommand Comando de visualización de Win32 (ej. SW_SHOW, SW_HIDE).
   */
  void
    Show(int showCommand) noexcept;
  /**
   * @brief Destruye la ventana y desregistra su clase en el sistema operativo.
   */
  void
    Destroy() noexcept;
  /**
   * @brief Extrae y procesa los mensajes de la cola de mensajes de Windows.
   *
   * Este método debe llamarse en cada ciclo del bucle principal para mantener
   * la ventana activa y responsiva.
   *
   * @return true si los mensajes se procesaron con normalidad. false si se recibió WM_QUIT (solicitud de cierre).
   */
  bool
    ProcessMessages() noexcept;
  /**
   * @brief Obtiene el handle nativo (HWND) de la ventana.
   *
   * @return HWND Handle de la ventana actual.
   */
  HWND
    GetHandle() const noexcept { return m_handle; }
  /**
   * @brief Comprueba si la ventana se encuentra actualmente minimizada.
   *
   * @return true si la ventana está minimizada, false de lo contrario.
   */
  bool
    IsMinimized() const noexcept;

private:
  /**
   * @brief Función callback estática que maneja los eventos del sistema operativo (Window Procedure).
   *
   * @param handle Handle de la ventana que recibe el mensaje.
   * @param message El identificador del mensaje recibido.
   * @param wparam Información adicional específica del mensaje.
   * @param lparam Información adicional específica del mensaje.
   * @return LRESULT Resultado del procesamiento del mensaje.
   */
  static LRESULT CALLBACK
    WindowProcedure(HWND handle, UINT message,
      WPARAM wparam, LPARAM lparam);

  /** @brief Nombre identificador para la clase de ventana registrada en Win32. */
  static constexpr const wchar_t* ClassName = L"SurrealEngine3DWindow";

  HINSTANCE m_instance = nullptr;      /**< Instancia de la aplicación de Windows. */
  HWND m_handle = nullptr;             /**< Handle nativo de la ventana creada. */
  bool m_classRegistered = false;      /**< Bandera que indica si la clase de ventana ya fue registrada. */
};