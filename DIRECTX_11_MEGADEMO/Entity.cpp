#include "Entity.h"


Entity::Entity(ResourceManager* _rm, string mesh_name /*= 0*/, string shader_name /*= 0*/, string texture_diffuse /*= 0*/, string texture_normal /*= 0*/)
	: rm(_rm)
{
	if (!mesh_name.empty())
	{
		SetMesh(mesh_name);
		is_mesh = true;
	}

	if (!shader_name.empty())
	{
		SetShader(shader_name);
		is_shader = true;

		if (shader->HasUVComponent())
		{
			if (!texture_diffuse.empty() | !texture_normal.empty())
			{
				SetDiffuse(texture_diffuse);
				SetNormal(texture_normal);
				is_textures = true;
			}
		}

	}
}

void Entity::SetMesh(string name)
{
	vb = rm->mesh(name);
	is_mesh = true;
}

void Entity::SetShader(string name)
{
	shader = rm->shader(name);
	is_shader = true;

}

void Entity::SetDiffuse(string name)
{
	diffuse = rm->texture(name);
}

void Entity::SetNormal(string name)
{
	normal = rm->texture(name);
}

void Entity::Draw()
{
	is_textures = diffuse != NULL & normal !=NULL;
	
	if (is_mesh & is_shader)
	{
		shader->bind();
		vb->bind();
	}
	else MESSAGE("Entity " + (is_mesh ? string(" has not mesh") : string("has mesh"))
			+ (is_shader ? string("and has not shader") : string("and has shader, but something wrong")));
	if (shader->HasUVComponent())
	{

		if (is_textures)
		{
			diffuse->bind(0);
			normal->bind(1);
		}
		else MESSAGE("Entity hasn't textures");
	}

	vb->draw();
}
