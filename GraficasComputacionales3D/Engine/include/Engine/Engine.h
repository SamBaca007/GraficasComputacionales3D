/**
 * @file Engine.h
 * @brief Clase principal del motor gráfico.
 *
 * Expone la interfaz orientada a objetos del motor. Utiliza el modismo Pimpl
 * (Pointer to Implementation) para ocultar los detalles internos de la API gráfica
 * (como DirectX) y evitar propagar dependencias pesadas al código cliente.
 */
#pragma once
#include "Prerequisites.h"
 /**
  * @class Engine
  * @brief Clase final que gestiona el ciclo de vida central del motor 3D.
  *
  * Actúa como la fachada principal para inicializar el contexto gráfico,
  * renderizar los frames y limpiar los recursos. Su ciclo de vida está
  * estrictamente controlado (no se puede copiar ni mover).
  */
class ENGINE_API Engine final {
public:
  /**
   * @brief Constructor por defecto.
   */
  Engine() noexcept;
  /**
   * @brief Destructor. Libera la memoria de la implementación interna.
   */
  ~Engine() noexcept;
  /** @name Constructores y operadores eliminados
   *  Previenen la copia y el movimiento de la instancia del motor para
   *  garantizar que el estado interno y los punteros a recursos sean únicos.
   */
   ///@{
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&&) = delete;
  Engine& operator=(Engine&&) = delete;
  ///@}

  /**
   * @brief Inicializa el motor y su implementación subyacente.
   *
   * @param nativeWindow Puntero genérico al handle de la ventana nativa (ej. HWND en Windows).
   *                     Se usa void* para no exponer APIs del SO en esta cabecera.
   * @param width Ancho del área de renderizado en píxeles.
   * @param height Alto del área de renderizado en píxeles.
   * @return true si el motor se inicializó correctamente, false si ocurrió un error crítico.
   */
  bool Initialize(
    void* nativeWindow,
    std::uint32_t width,
    std::uint32_t height
  ) noexcept;
  /**
   * @brief Ejecuta las rutinas de dibujado para el frame actual.
   *
   * Coordina la limpieza del back buffer, el envío de comandos de dibujado
   * a la GPU y la presentación (swap) en la pantalla.
   */
  void Render() noexcept;
  /**
   * @brief Apaga el motor y destruye los subsistemas activos.
   *
   * Garantiza que todos los recursos alojados en la GPU y estructuras internas
   * se liberen de forma segura antes de cerrar la aplicación.
   */
  void Shutdown() noexcept;

private:
  /**
   * @brief Estructura opaca que contiene los verdaderos datos y lógica de la API gráfica.
   */
  struct Implementation;

  /** @brief Puntero a la implementación oculta (Pimpl idiom). */
  Implementation* m_implementation = nullptr;
};