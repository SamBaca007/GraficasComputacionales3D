#pragma once
#include "Engine/Prerequisites.h"
#include "Rendering/InputLayout.h"

class Device;
class DeviceContext;
class LayoutBuilder;

class
ShaderProgram {
public:

  ShaderProgram() = default;
  ~ShaderProgram() = default;

  HRESULT
  init(Device& device, const std::string& fileName, LayoutBuilder layoutBuilder);

  void
  update();

  void
  render(DeviceContext& deviceContext);

  void
  render(DeviceContext& deviceContext, ShaderType type);

  void
  destroy();

  HRESULT
  CreateInputLayout(Device& device, LayoutBuilder);

  HRESULT
  CreateShader(Device& device, ShaderType type);

  HRESULT
  CreateShader(Device& device, ShaderType type, const std::string& fileName);

  HRESULT
  CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
    ID3DBlob** ppBlobOut);

public:

  ID3D11VertexShader* m_VertexShader = nullptr;

  ID3D11PixelShader* m_PixelShader = nullptr;

  InputLayout m_InputLayout;

private:

  std::string m_ShaderFileName;

  ID3DBlob* m_VertexShaderData = nullptr;

  ID3DBlob* m_PixelShaderData = nullptr;
};