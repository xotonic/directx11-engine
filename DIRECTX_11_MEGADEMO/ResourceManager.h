#pragma once
#include <map>
#include <string>

#include "common.h"
#include "DeviceDependent.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
using namespace std;

class ResourceManager : private DeviceDependent
{
public:
	ResourceManager(ID3D11Device *dev) : DeviceDependent(dev){};
	~ResourceManager();

	void loadMesh(string name, string filename);
	void loadTexture(string name, string filename);
	void loadShader(string name, string vs_name, string ps_name);

	VertexBuffer* mesh(string name);
	Texture* texture(string name);
	Shader* shader(string name);

private:
	map<string, VertexBuffer*> vertexBuffers;
	map<string, Texture*> textures;
	map<string, Shader*> shaders;

};

