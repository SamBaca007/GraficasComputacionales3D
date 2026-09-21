#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

class Window final {
public:
  Window() = default;
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  bool
  Create(HINSTANCE instance, const wchar_t* title,
         UINT clientWidth, UINT clientHeight) noexcept;

  void
  Show(int showCommand) noexcept;

  void
  Destroy() noexcept;

  // Devlueve false cuando se recibe WM_QUIT.
  bool
  ProcessMessages() noexcept;

  HWND
  GetHandle() const noexcept { return m_handle; }

  bool
  IsMinimized() const noexcept;

private:
  static LRESULT CALLBACK
  WindowProcedure(HWND handle, UINT message,
    WPARAM wparam, LPARAM lparam);

  static constexpr const wchar_t* ClassName =
    L"SurrealEngine3DWindow";

  HINSTANCE m_instance = nullptr;
  HWND m_handle = nullptr;
  bool m_classRegistered = false;
};