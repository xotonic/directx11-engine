#include "ResourceManager.h"

ResourceManager::ResourceManager(ID3D11Device *dev, string filename /*= "resources.txt"*/):
DeviceDependent(dev)
{
	readResources(filename);
}

ResourceManager::~ResourceManager()
{
	for (auto it : vertexBuffers)
		delete it.second;
	for (auto it : shaders)
		delete it.second;
	for (auto it : textures)
		delete it.second;
}

void ResourceManager::loadMesh(string name, string filename)
{
	vertexBuffers.insert(pair<string, VertexBuffer*>(name, new VertexBuffer(device,filename)));
}

void ResourceManager::loadTexture(string name, string filename)
{
	textures.insert(pair<string, Texture*>(name, new Texture(device, filename)));
}

void ResourceManager::loadShader(string name, string vs_name, string ps_name,bool color, bool normal, bool uv)
{
	shaders.insert(pair<string, Shader*>(name, new Shader(device, vs_name, ps_name, color, normal, uv)));
}

void ResourceManager::readResources(string filename)
{
	ifstream in(filename);
	if (in)
	{
		while (!in.eof())
		{
			string a;
			in >> a;

			if (a.front() == 'm')
			{
				string name, file;
				in >> name >> file;
				loadMesh(name, file);
			}

			else if (a.front() == 's')
			{
				string name, file_vs, file_ps;
				bool color, normal, uv;
				in >> name >> file_vs >> file_ps >> color >> normal >> uv;
				loadShader(name, file_vs, file_ps, color, normal, uv);
			}
			else if (a.front() == 't')
			{
				string name, file;
				in >> name >> file;
				loadTexture(name, file);
			}
		}
		in.close();
	}
	else	MESSAGE("Resource file " + filename + " not found");
}

VertexBuffer* ResourceManager::mesh(string name)
{
	auto it = vertexBuffers.find(name);
	
	if (it == vertexBuffers.end()) MESSAGE("Can't find mesh in resource manager : " + name);

	return it->second;
}

Texture* ResourceManager::texture(string name)
{
	auto it = textures.find(name);

	if (it == textures.end()) MESSAGE("Can't find texture in resource manager : " + name);

	return it->second;
}

Shader* ResourceManager::shader(string name)
{
	auto it = shaders.find(name);

	if (it == shaders.end()) MESSAGE("Can't find shader in resource manager : " + name);

	return it->second;
}

