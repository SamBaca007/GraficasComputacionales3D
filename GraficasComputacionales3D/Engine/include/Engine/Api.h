/**
 * @file Api.h
 * @brief Macro de definición para la exportación e importación de símbolos de la DLL.
 *
 * Gestiona la visibilidad de la API pública del motor mediante las directivas
 * especificaciones de DLL de Microsoft Visual C++ (__declspec).
 */
#pragma once

#if defined(_WIN32)

#if defined(ENGINE_BUILD_DLL)
 /**
  * @def ENGINE_API
  * @brief Macro para la visibilidad de símbolos de la biblioteca.
  *
  * Se evalúa a `__declspec(dllexport)` cuando se compila la biblioteca del motor
  * (definiendo `ENGINE_BUILD_DLL`), o a `__declspec(dllimport)` cuando es consumida
  * por un ejecutable o proyecto cliente externo. En plataformas que no son Windows,
  * se define vacía.
  */
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#else
#define ENGINE_API
#endif