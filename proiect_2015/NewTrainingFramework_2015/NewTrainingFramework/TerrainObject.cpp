#include "stdafx.h"
#include "Camera.h"
#include "SceneManager.h"
#include <iostream>
#include "Vertex.h"
#include "Camera.h"
#include "TerrainObject.h"

void TerrainObject::sendSpecificData()
{
	if (shader->Nuniform != -1)
	{
		glUniform1f(shader->Nuniform, nr_cel);
	}

	if (shader->heightsUniform != -1)
	{
		glUniform3f(shader->heightsUniform, heights.x, heights.y, heights.z);
	}

	if (shader->deplasament != -1)
	{
		glUniform2f(shader->deplasament, deplasament.x, deplasament.y);
	}
}

void TerrainObject::Update()
{
	int x, z, dim_cel;
	std::map<int, SceneObject*>::iterator i = SceneManager::getInstance()->objects.begin();
	
	float dx = SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.x - this->position.x, dz = SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.z - this->position.z; 
	if (abs(dx) > dimCel)
	{
		if (dx > 0)
		{
			deplasament.x++;
		}
		else
			deplasament.x--;
	}
	if (abs(dz) > dimCel)
	{
		if (dz > 0)
		{
			deplasament.y++;
		}
		else
			deplasament.y--;
	}		

	position.x = deplasament.x * dimCel;
	position.z = deplasament.y * dimCel;

}
