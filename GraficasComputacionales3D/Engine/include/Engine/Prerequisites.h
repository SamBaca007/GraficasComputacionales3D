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
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <sstream>
#include <cstddef>
#include <chrono>
#include <cstdint>
#include <new>

 // MACROS

  /**
   * @brief Macro clásica para liberar y anular una interfaz COM de DirectX.
   * @param x Puntero a la interfaz COM.
   */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;
   /**
    * @brief Macro de registro para notificar la creación de recursos en la salida de depuración de Visual Studio.
    */
#define MESSAGE( classObj, method, state )   \
{                                             \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}
    /**
     * @brief Macro para registrar mensajes de error en la ventana de depuración de Win32.
     */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}
     /**
      * @brief Helper genérico para liberar de forma segura punteros de interfaz COM de DirectX.
      *
      * @tparam T Tipo del objeto COM que implementa IUnknown.
      * @param object Referencia al puntero del objeto a liberar.
      */
template<typename T>
void
SafeRelease(T*& object) noexcept {
  if (object != nullptr)
  {
    object->Release();
    object = nullptr;
  }
}

extern
"C" {
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