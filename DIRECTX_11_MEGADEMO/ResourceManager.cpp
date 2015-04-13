#include "ResourceManager.h"

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

void ResourceManager::loadShader(string name, string vs_name, string ps_name)
{
	shaders.insert(pair<string, Shader*>(name, new Shader(device, vs_name, ps_name)));
}

VertexBuffer* ResourceManager::mesh(string name)
{
	auto it = vertexBuffers.find(name);
	
	if (it == vertexBuffers.end()) throw "Can't find mesh in resource manager : " + name;

	return it->second;
}

Texture* ResourceManager::texture(string name)
{
	auto it = textures.find(name);

	if (it == textures.end()) throw "Can't find texture in resource manager : " + name;

	return it->second;
}

Shader* ResourceManager::shader(string name)
{
	auto it = shaders.find(name);

	if (it == shaders.end()) throw "Can't find shader in resource manager : " + name;

	return it->second;
}
