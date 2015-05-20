#pragma once
#include "stdafx.h"
#include "debug.h"
#include "DeviceDependent.h"
#include <vector>
#include <d3dcompiler.h>
class Shader : private DeviceDependent
{
public:
	Shader(ID3D11Device* dev, std::string vs_name, std::string ps_name, bool, bool, bool);
	~Shader();

	void bind();
	ID3D11VertexShader* vertex() { return vertexShader; }
	ID3D11PixelShader* pixel() { return pixelShader; }
	ID3D11InputLayout* layout() { return inputLayout; }
	
	bool HasColorComponent() { return colorComponent; }
	bool HasNormalComponent() { return normalComponent; }
	bool HasUVComponent() { return UVComponent; }

private:

	void compileFromFile(std::string fileName,
		const LPCSTR ep,
		const LPCSTR model,
		ID3D10Blob** blob);

	const LPCSTR VS_entryPoint = "VS";
	const LPCSTR PS_entryPoint = "PS";
	const LPCSTR VS_model = "vs_4_0";
	const LPCSTR PS_model = "ps_4_0";

	ID3D11VertexShader* vertexShader = NULL;
	ID3D11PixelShader* pixelShader = NULL;
	ID3D11InputLayout* inputLayout = NULL;

	bool colorComponent, UVComponent, normalComponent;
};
