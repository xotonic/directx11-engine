#pragma once
#include "stdafx.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transformable.h"
#include "ResourceManager.h"

using namespace std;

class Entity
{
public:
	Entity(ResourceManager* _rm, string mesh_name = 0, string shader_name = 0, string texture_diffuse = 0, string texture_normal = 0);

	Transformable* transform() { return &tr; }

	void SetMesh(string);
	void SetShader(string);
	void SetDiffuse(string);
	void SetNormal(string);
	void Draw();
private:
	ResourceManager* rm;
	Transformable tr;
	VertexBuffer* vb;
	Texture* diffuse;
	Texture* normal;
	Shader* shader;

	bool is_mesh, is_shader, is_textures;
};
