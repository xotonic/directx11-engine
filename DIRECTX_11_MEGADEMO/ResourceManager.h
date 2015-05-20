#pragma once
#include <map>
#include <string>
#include <fstream>

#include "stdafx.h"
#include "DeviceDependent.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"

using namespace std;

class ResourceManager : private DeviceDependent
{
public:
	ResourceManager(ID3D11Device *dev, string filename = "resources.txt");
	~ResourceManager();

	void loadMesh(string name, string filename);
	void loadTexture(string name, string filename);
	void loadShader(string name, string vs_name, string ps_name, bool color, bool normal, bool uv);
	void readResources(string filename);

	VertexBuffer* mesh(string name);
	Texture* texture(string name);
	Shader* shader(string name);

private:
	map<string, VertexBuffer*> vertexBuffers;
	map<string, Texture*> textures;
	map<string, Shader*> shaders;
};