#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Engine/Engine.h>

constexpr wchar_t WINDOW_CLASS_NAME[] =
L"Surreal Engine 3D";

LRESULT CALLBACK WindowProcedure(
  HWND window,
  UINT message,
  WPARAM wParam,
  LPARAM lParam
)
{
  switch (message)
  {
  case WM_CLOSE:
    DestroyWindow(window);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_ERASEBKGND:
    // DirectX limpia y dinuja toda la ventana.
    return 1;

  default:
    return DefWindowProcW(
      window,
      message,
      wParam,
      lParam
    );
  }
}

int WINAPI
wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR commandLine, int showCommand) {

  UNREFERENCED_PARAMETER(previousInstance);
  UNREFERENCED_PARAMETER(commandLine);

  constexpr UINT CLIENT_WIDTH = 1280;
  constexpr UINT CLIENT_HEIGHT = 720;

  WNDCLASSEXW windowClass{};
  windowClass.cbSize = sizeof(WNDCLASSEXW);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProcedure;
  windowClass.hInstance = instance;
  windowClass.hCursor = LoadCursorW(
    nullptr,
    IDC_ARROW
  );
  windowClass.lpszClassName = WINDOW_CLASS_NAME;

  if (!RegisterClassExW(&windowClass))
  {
    MessageBoxW(
      nullptr,
      L"No se pudo registrar la clase de ventana.",
      L"Sandbox Error",
      MB_OK | MB_ICONERROR
    );

    return 1;
  }

  // Ventana fija mientras no implementemos ResizeBuffers.
  constexpr DWORD windowStyle =
    WS_OVERLAPPED |
    WS_CAPTION |
    WS_SYSMENU |
    WS_MINIMIZEBOX;

  RECT windowRectangle
  {
    0,
    0,
    static_cast<LONG>(CLIENT_WIDTH),
    static_cast<LONG>(CLIENT_HEIGHT)
  };

  if (!AdjustWindowRect(
    &windowRectangle,
    windowStyle,
    FALSE))
  {
    UnregisterClassW(
      WINDOW_CLASS_NAME,
      instance
    );

    return 1;
  }

  const int windowWidth =
    windowRectangle.right -
    windowRectangle.left;

  const int windowHeight =
    windowRectangle.bottom -
    windowRectangle.top;

  HWND window = CreateWindowExW(
    0,
    WINDOW_CLASS_NAME,
    L"Surreal Engine 3D",
    windowStyle,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    windowWidth,
    windowHeight,
    nullptr,
    nullptr,
    instance,
    nullptr
  );

  if (!window)
  {
    MessageBoxW(
      nullptr,
      L"No se pudo crear la ventana.",
      L"Sandbox Error",
      MB_OK | MB_ICONERROR
    );

    UnregisterClassW(
      WINDOW_CLASS_NAME,
      instance
    );

    return 1;
  }

  Engine engine;
  if (!engine.Initialize(
    window,
    CLIENT_WIDTH,
    CLIENT_HEIGHT))
  {
    MessageBoxW(
      window,
      L"No se pudo inicializar el Engine.\n\n"
      L"Verifica que exista:\n"
      L"shaders\\Cube.hlsl\n\n"
      L"Revisa también la ventana Output.",
      L"Engine Error",
      MB_OK | MB_ICONERROR
    );

    DestroyWindow(window);

    UnregisterClassW(
      WINDOW_CLASS_NAME,
      instance
    );

    return 1;
  }

  ShowWindow(window, showCommand);
  UpdateWindow(window);

  MSG message{};
  bool running = true;

  while (running)
  {
    while (PeekMessageW(
      &message,
      nullptr,
      0,
      0,
      PM_REMOVE))
    {
      if (message.message == WM_QUIT)
      {
        running = false;
        break;
      }

      TranslateMessage(&message);
      DispatchMessageW(&message);
    }

    if (!running)
      break;

    if (IsIconic(window))
    {
      WaitMessage();
      continue;
    }

    engine.Render();
  }

  engine.Shutdown();

  UnregisterClassW(
    WINDOW_CLASS_NAME,
    instance
  );

  return 0;

}