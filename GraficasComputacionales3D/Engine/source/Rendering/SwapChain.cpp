/**
 * @file SwapChain.cpp
 * @brief Implementación de la clase SwapChain para gestionar la cadena de intercambio de DXGI.
 */

#include "Rendering/SwapChain.h"
#include "Rendering/Device.h"

 /**
  * @brief Libera los recursos de memoria asignados a la cadena de intercambio.
  *
  * Utiliza la macro SafeRelease (o SAFE_RELEASE) para disminuir el conteo de referencias
  * del puntero COM y asignarlo a nulo.
  */
void
SwapChain::destroy() {
  SafeRelease(m_SwapChain);
}

/**
 * @brief Configura e inicializa el SwapChain (Cadena de intercambio).
 *
 * Define las características del back buffer (tamaño, formato, tasa de refresco, MSAA),
 * extrae las interfaces DXGI (Device, Adapter y Factory) a partir del dispositivo de Direct3D,
 * y finalmente crea la cadena de intercambio vinculándola a la ventana proporcionada.
 *
 * @param device Objeto Device que contiene el ID3D11Device inicializado.
 * @param windowHandle Handle (HWND) de la ventana de salida donde se presentará el render.
 * @param width Ancho en píxeles del back buffer.
 * @param height Alto en píxeles del back buffer.
 * @return HRESULT S_OK si la creación es exitosa, o un código de error detallado en caso contrario.
 */
HRESULT
SwapChain::init(Device device, HWND windowHandle, unsigned int width, unsigned int height) {
  ID3D11Device* pDevice = device.getDevice();
  if (!pDevice) {
    ERROR("SwapChain", "init", "Provided Device is null.");
    return E_INVALIDARG;
  }

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  // Asignamos directamente el HWND que recibimos por parámetro
  sd.OutputWindow = windowHandle;

  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  // Navegación COM para obtener el DXGIFactory subyacente
  IDXGIDevice* pDXGIDevice = nullptr;
  HRESULT hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
  if (FAILED(hr)) return hr;

  IDXGIAdapter* pDXGIAdapter = nullptr;
  hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
  if (FAILED(hr)) { SafeRelease(pDXGIDevice); return hr; }

  IDXGIFactory* pDXGIFactory = nullptr;
  hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
  if (FAILED(hr)) { SafeRelease(pDXGIAdapter); SafeRelease(pDXGIDevice); return hr; }

  hr = pDXGIFactory->CreateSwapChain(pDevice, &sd, &m_SwapChain);

  // Liberar recursos intermedios
  SafeRelease(pDXGIFactory);
  SafeRelease(pDXGIAdapter);
  SafeRelease(pDXGIDevice);

  if (FAILED(hr)) {
    ERROR("SwapChain", "init", ("Failed to create SwapChain. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  MESSAGE("SwapChain", "init", "SUCCESS");
  return S_OK;
}

/**
 * @brief Destructor de la clase SwapChain.
 * Llama a destroy() para garantizar que no haya fugas de memoria (memory leaks).
 */
SwapChain::~SwapChain() {
  destroy();
}

/**
 * @brief Obtiene el puntero a uno de los buffers administrados por el SwapChain.
 *
 * Normalmente se utiliza para obtener el Back Buffer (Buffer 0) y crear a partir de él
 * el RenderTargetView principal.
 *
 * @param Buffer Índice del buffer que se quiere obtener.
 * @param riid Identificador de interfaz COM (REFIID) que se espera recuperar.
 * @param ppSurface Puntero doble donde se guardará la dirección de memoria del buffer.
 * @return HRESULT S_OK si se obtiene el buffer exitosamente.
 */
HRESULT
SwapChain::GetBuffer(unsigned int Buffer,
  REFIID riid,
  void** ppSurface) {
  if (ppSurface == nullptr) {
    ERROR("SwapChain", "GetBuffer", "Output pointer ppSurface is null.");
    return E_POINTER;
  }

  *ppSurface = nullptr;

  if (!m_SwapChain) {
    ERROR("SwapChain", "GetBuffer", "SwapChain is not initialized.");
    return E_FAIL;
  }

  HRESULT result = m_SwapChain->GetBuffer(Buffer, riid, ppSurface);

  if (FAILED(result)) {
    ERROR("SwapChain", "GetBuffer",
      ("Failed to get buffer from swap chain. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("SwapChain", "GetBuffer", "SUCCESS");
  return result;
}

/**
 * @brief Presenta la imagen renderizada en el back buffer llevándola a la pantalla frontal.
 *
 * @param SyncInterval Determina cómo se sincroniza la presentación con el monitor (0 = sin VSync, 1 = activado).
 * @param Flags Opciones de presentación de DXGI.
 * @return HRESULT S_OK si el frame fue presentado de manera correcta.
 */
HRESULT
SwapChain::Present(unsigned int SyncInterval,
  unsigned int Flags) {
  // Verificamos que el puntero al SwapChain exista antes de llamar a Present
  if (!m_SwapChain) {
    ERROR("SwapChain", "Present", "SwapChain is not initialized.");
    return E_FAIL; // Retornamos un HRESULT de falla estándar
  }

  // Llamamos al método Present del Swap Chain nativo de DXGI
  return m_SwapChain->Present(SyncInterval, Flags);
}

/**
 * @brief Cambia el tamaño de los buffers del SwapChain.
 *
 * Este método se debe llamar cuando la ventana de la aplicación cambie de tamaño
 * para actualizar las dimensiones de la superficie de renderizado sin tener que
 * recrear por completo la cadena de intercambio.
 *
 * @param width Nuevo ancho de los buffers.
 * @param height Nuevo alto de los buffers.
 * @return HRESULT S_OK si se redimensionan exitosamente.
 */
HRESULT
SwapChain::ResizeBuffers(unsigned int width,
  unsigned int height) {
  if (!m_SwapChain) {
    ERROR("SwapChain", "ResizeBuffers", "SwapChain is not initialized.");
    return E_FAIL;
  }

  // Los parámetros son: BufferCount, Width, Height, NewFormat, SwapChainFlags
  // Pasando 0 en BufferCount y DXGI_FORMAT_UNKNOWN (0) preservamos los ajustes originales.
  HRESULT result = m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  if (FAILED(result)) {
    ERROR("SwapChain", "ResizeBuffers",
      ("Failed to resize buffers. HRESULT: " + std::to_string(result)).c_str());
    return result;
  }

  MESSAGE("SwapChain", "ResizeBuffers", "SUCCESS");
  return result;
}