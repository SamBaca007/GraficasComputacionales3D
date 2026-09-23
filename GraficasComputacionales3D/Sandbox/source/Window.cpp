/**
 * @file Window.cpp
 * @brief Implementación de la clase Window para la creación y gestión de ventanas de Win32.
 */

#include <Engine/Engine.h>
#include "Window.h"
 /**
  * @brief Destruye la instancia de la ventana invocando la limpieza de recursos.
  */
Window::~Window()
{
  Destroy();
}
/**
 * @brief Registra la clase de ventana y crea la instancia correspondiente en Win32.
 *
 * Configura los estilos de la ventana, ajusta las dimensiones del área cliente
 * y llama a la API de Windows para generar el handle (HWND).
 *
 * @param instance Instancia del módulo (HINSTANCE).
 * @param title Título en la barra de la ventana.
 * @param clientWidth Ancho deseado del área cliente.
 * @param clientHeight Alto deseado del área cliente.
 * @return true Si la creación fue exitosa.
 * @return false Si los parámetros son inválidos o falla el registro/creación en Win32.
 */
bool
Window::Create(HINSTANCE instance, const wchar_t* title,
  UINT clientWidth, UINT clientHeight) noexcept {
  if (m_handle || !instance || !title ||
    clientWidth == 0 || clientHeight == 0)
  {
    return false;
  }

  WNDCLASSEXW windowClass{};
  windowClass.cbSize = sizeof(windowClass);
  windowClass.lpfnWndProc = WindowProcedure;
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.hInstance = instance;
  windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
  windowClass.lpszClassName = ClassName;

  if (!RegisterClassExW(&windowClass))
    return false;

  m_instance = instance;
  m_classRegistered = true;

  // Ventana fija hasta implementar el redimensionamiento
  // del swap chain y del depth buffer.

  constexpr DWORD style =
    WS_OVERLAPPED |
    WS_CAPTION |
    WS_SYSMENU |
    WS_MINIMIZEBOX;

  RECT rectangle
  {
    0,
    0,
    static_cast<LONG>(clientWidth),
    static_cast<LONG>(clientHeight)
  };

  if (!AdjustWindowRect(&rectangle, style, FALSE))
  {
    Destroy();
    return false;
  }

  const int outerWidth = rectangle.right - rectangle.left;
  const int outerHeight = rectangle.bottom - rectangle.top;

  m_handle = CreateWindowExW(
    0,
    ClassName,
    title,
    style,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    outerWidth,
    outerHeight,
    nullptr,
    nullptr,
    instance,
    nullptr
  );

  if (!m_handle)
  {
    Destroy();
    return false;
  }

  return true;
}
/**
 * @brief Muestra y actualiza el estado de visibilidad de la ventana.
 *
 * @param showCommand Comando de visualización de Win32 (ej. SW_SHOW).
 */
void Window::Show(int showCommand) noexcept
{
  if (m_handle)
  {
    ShowWindow(m_handle, showCommand);
    UpdateWindow(m_handle);
  }
}
/**
 * @brief Destruye la ventana de Win32 y desregistra la clase asociada si estaba registrada.
 */
void Window::Destroy() noexcept
{
  if (m_handle)
  {
    DestroyWindow(m_handle);
    m_handle = nullptr;
  }

  if (m_classRegistered)
  {
    UnregisterClassW(ClassName, m_instance);
    m_classRegistered = false;
  }

  m_instance = nullptr;
}
/**
 * @brief Procesa de forma no bloqueante todos los mensajes pendientes en la cola de la ventana.
 *
 * @return true Si se procesaron los mensajes correctamente.
 * @return false Si se interceptó el mensaje WM_QUIT (indicando el cierre de la aplicación).
 */
bool Window::ProcessMessages() noexcept
{
  MSG message{};

  while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
  {
    if (message.message == WM_QUIT)
    {
      return false;
    }

    TranslateMessage(&message);
    DispatchMessageW(&message);
  }

  return true;
}
/**
 * @brief Consulta si la ventana está actualmente minimizada.
 *
 * @return true Si la ventana está minimizada.
 * @return false Si la ventana está visible o no se ha creado.
 */
bool Window::IsMinimized() const noexcept
{
  return m_handle && IsIconic(m_handle);
}
/**
 * @brief Procedimiento de ventana principal (WndProc) para interceptar mensajes de Win32.
 *
 * @param handle Handle de la ventana que recibe el mensaje.
 * @param message Identificador del mensaje de evento.
 * @param wparam Parámetro adicional de mensaje.
 * @param lparam Parámetro adicional de mensaje.
 * @return LRESULT Resultado del procesamiento del mensaje.
 */
LRESULT Window::WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_ERASEBKGND:
    // DirectX limpia y dibuja toda la ventana.
    return 1;

  default:
    return DefWindowProcW(handle, message, wparam, lparam);
  }
}