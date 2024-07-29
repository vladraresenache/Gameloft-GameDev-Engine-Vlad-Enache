#include "stdafx.h"
#include "Camera.h"
#include "SceneManager.h"
#include <iostream>
#include "Vertex.h"
#include "Camera.h"



void SceneObject::Draw()
{
	glUseProgram(shader->programId);
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	

	sendCommonData();
	sendSpecificData();

	glDrawElements(GL_TRIANGLES, model->nrIndici, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneObject::sendCommonData()
{


	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	int err;
	err = glGetError();
	Camera* c;
	int ac = SceneManager::getInstance()->activeCamera;
	c = SceneManager::getInstance()->Cameras[ac];

	Matrix transform, s, r_x, r_y, r_z, t;
	s.SetScale(scale);
	r_x.SetRotationX(rotation[0]);
	r_y.SetRotationY(rotation[1]);
	r_z.SetRotationZ(rotation[2]);
	t.SetTranslation(position);
	transform = s * r_x * r_y * r_z * t;

	if (shader->cameraUniform != -1)
	{
		glUniformMatrix4fv(shader->cameraUniform, 1, GL_FALSE, (float*)(transform * c->viewMatrix * c->perspectiveMatrix).m);
	}
	err = glGetError();
	if (shader->normAttribute != -1)
	{
		glEnableVertexAttribArray(shader->normAttribute);
		glVertexAttribPointer(shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}

	err = glGetError();
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->texId);
		if (this->shader->textureUniform[i] != -1)
			glUniform1i(this->shader->textureUniform[i], i);
	}

	if (shader->s_r != -1)
	{
		glUniform1f(shader->s_r, SceneManager::getInstance()->s_r);
	}

	if (shader->b_r != -1)
	{
		glUniform1f(shader->b_r, SceneManager::getInstance()->b_r);
	}

	if (shader->fog_color != -1)
	{
		glUniform3f(shader->fog_color, SceneManager::getInstance()->fog_color.x, SceneManager::getInstance()->fog_color.y, SceneManager::getInstance()->fog_color.z);
	}

	if (shader->camera_pos != -1)
	{
		glUniform3f(shader->camera_pos, SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.x, SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.y, SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.z);
	}

	if (shader->modelMatrix != -1)
	{
		glUniformMatrix4fv(shader->modelMatrix, 1, GL_FALSE, (float*)transform.m);
	}

	if (shader->amb_color != -1)
	{
		glUniform3f(shader->amb_color, SceneManager::getInstance()->ambientalColor.x, SceneManager::getInstance()->ambientalColor.y, SceneManager::getInstance()->ambientalColor.z);
	}

	if (shader->amb_ratio != -1)
	{
		glUniform1f(shader->amb_ratio, SceneManager::getInstance()->ambientalRatio);
	}

	if (shader->c_diff != -1)
	{
		glUniform3f(shader->c_diff, SceneManager::getInstance()->lights[1]->diffuseColor.x, SceneManager::getInstance()->lights[1]->diffuseColor.y, SceneManager::getInstance()->lights[1]->diffuseColor.z);
	}
	if (shader->c_spec != -1)
	{
		glUniform3f(shader->c_spec, SceneManager::getInstance()->lights[1]->specularColor.x, SceneManager::getInstance()->lights[1]->specularColor.y, SceneManager::getInstance()->lights[1]->specularColor.z);
	}
	if (shader->light_direction != -1)
	{
		glUniform3f(shader->light_direction, SceneManager::getInstance()->lights[1]->direction.x, SceneManager::getInstance()->lights[1]->direction.y, SceneManager::getInstance()->lights[1]->direction.z);
	}

	if (shader->specPower != -1)
	{
		glUniform1f(shader->specPower, SceneManager::getInstance()->lights[1]->specPower);
	}

	err = glGetError();
}

void SceneObject::sendSpecificData()
{
}

void SceneObject::Update()
{
	
}
