#include "stdafx.h"
#include "SkyBox.h"
#include <iostream>
#include "SceneManager.h"

void SkyBox::sendSpecificData()
{
	if (shader->u_cube_texture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]->texId);
		glUniform1i(shader->u_cube_texture, 0);
	}

	int err;
	err = glGetError();
}

void SkyBox::Update()
{
	
	if (followingCamera[0] != 0 || followingCamera[1] != 0 || followingCamera[2] != 0)
	{
		if (followingCamera[0] == 1)
			position.x = SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.x;
		if (followingCamera[1] == 1)
				position.y = SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.y + 300;
		if(followingCamera[2] == 1)
				position.z = SceneManager::getInstance()->Cameras[SceneManager::getInstance()->activeCamera]->position.z;
	}
}