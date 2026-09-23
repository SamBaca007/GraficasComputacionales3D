#pragma once
#include "Api.h"
#include <cstdint>
#include <Windows.h>

extern "C" {
  ENGINE_API bool
    Engine_Initialize(HWND hand, int width, int height) noexcept;

  ENGINE_API void
    Engine_Update() noexcept;

  ENGINE_API void
    Engine_Render() noexcept;

  ENGINE_API void
    Engine_Shutdown() noexcept;
}