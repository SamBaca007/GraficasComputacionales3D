/**
 * @file Prerequisites.h
 * @brief Archivo de cabecera principal con las dependencias y la API base del motor.
 *
 * Incluye las librerías fundamentales del sistema y expone las funciones principales
 * del ciclo de vida del motor mediante enlaces C (extern "C") para evitar el name mangling,
 * facilitando su uso como biblioteca dinámica (DLL).
 */
#pragma once

#include "Api.h"
#include <cstdint>
#include <Windows.h>

extern "C" {
  /**
   * @brief Inicializa el motor gráfico y todos sus subsistemas.
   *
   * @param hand Handle de la ventana de Windows (HWND) donde se realizará el renderizado.
   * @param width Ancho del área de renderizado en píxeles.
   * @param height Alto del área de renderizado en píxeles.
   * @return true si la inicialización fue exitosa, false en caso de error.
   */
  ENGINE_API bool
    Engine_Initialize(HWND hand, int width, int height) noexcept;
  /**
   * @brief Actualiza la lógica interna y el estado del motor.
   *
   * Esta función debe ser llamada una vez por cada iteración del bucle principal
   * (frame), previo a la fase de renderizado.
   */
  ENGINE_API void
    Engine_Update() noexcept;
  /**
   * @brief Ejecuta el pipeline gráfico y renderiza el frame actual.
   *
   * Procesa los comandos de dibujo y los presenta en la ventana proporcionada
   * durante la inicialización.
   */
  ENGINE_API void
    Engine_Render() noexcept;
  /**
   * @brief Libera los recursos y apaga el motor de forma segura.
   *
   * Debe llamarse al finalizar el ciclo de vida de la aplicación para garantizar
   * que todos los recursos (memoria, handles, dispositivos gráficos) se destruyan
   * correctamente.
   */
  ENGINE_API void
    Engine_Shutdown() noexcept;
}