/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación.
 *
 * Contiene la función principal de Windows (wWinMain) que inicializa la ventana,
 * arranca el motor gráfico y ejecuta el bucle principal (game loop) de la aplicación.
 */

#include <Window.h>
#include <Engine/Engine.h>

 /** @brief Nombre de la aplicación y título por defecto de la ventana. */
constexpr wchar_t WINDOW_CLASS_NAME[] = L"Surreal Engine 3D";
/**
 * @brief Punto de entrada principal para aplicaciones Windows basadas en interfaz gráfica (Unicode).
 *
 * Configura las dimensiones del cliente, crea la ventana de Win32, inicializa el motor 3D
 * y mantiene activo el bucle de mensajes y de renderizado hasta que el usuario cierra la aplicación.
 *
 * @param instance Identificador de la instancia actual de la aplicación.
 * @param previousInstance Identificador de la instancia previa (obsoleto, siempre NULL en Win32 moderno).
 * @param commandLine Cadena con los argumentos pasados por línea de comandos.
 * @param showCommand Bandera que indica cómo debe mostrarse la ventana inicialmente.
 * @return int Código de salida de la aplicación (0 si terminó correctamente, 1 en caso de error).
 */
int WINAPI
wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR commandLine,
  int showCommand) {

  // Se ignoran explícitamente los parámetros no utilizados para evitar warnings del compilador.
  UNREFERENCED_PARAMETER(previousInstance);
  UNREFERENCED_PARAMETER(commandLine);

  /** @brief Ancho del área cliente de la ventana en píxeles. */
  constexpr UINT CLIENT_WIDTH = 1280;

  /** @brief Alto del área cliente de la ventana en píxeles. */
  constexpr UINT CLIENT_HEIGHT = 720;

  Window window;

  // 1. Inicializar el subsistema de la ventana
  if (!window.Create(instance, L"Surreal Engine 3D", CLIENT_WIDTH, CLIENT_HEIGHT))
  {
    MessageBoxW(
      nullptr,
      L"No se pudo crear la ventana.",
      L"Window Error",
      MB_OK | MB_ICONERROR
    );
    return 1;
  }

  Engine engine;

  // 2. Inicializar el motor gráfico y sus recursos de DirectX
  if (!engine.Initialize(window.GetHandle(), CLIENT_WIDTH, CLIENT_HEIGHT))
  {
    MessageBoxW(
      window.GetHandle(),
      L"No se pudo inicializar el Engine.\n\n"
      L"Verifica que exista:\n"
      L"shaders\\Cube.hlsl\n\n"
      L"Revisa también la ventana Output.",
      L"Engine Error",
      MB_OK | MB_ICONERROR
    );

    return 1;
  }

  // 3. Mostrar la ventana al usuario
  window.Show(showCommand);

  // 4. Bucle Principal (Game Loop)
  while (window.ProcessMessages())
  {
    // Si la aplicación está minimizada, suspende el hilo hasta recibir un mensaje
    // para evitar el consumo innecesario de CPU y GPU.
    if (window.IsMinimized())
    {
      WaitMessage();
      continue;
    }

    // Dibuja el frame actual en pantalla
    engine.Render();
  }

  // 5. Apagado y limpieza segura de recursos
  engine.Shutdown();
  window.Destroy();

  return 0;
}