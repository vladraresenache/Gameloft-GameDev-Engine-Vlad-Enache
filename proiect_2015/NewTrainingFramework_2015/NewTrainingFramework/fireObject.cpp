#include "stdafx.h"
#include "fireObject.h"
#include <iostream>
#include "SceneManager.h"

void fireObject::sendSpecificData()
{
	if (shader->dispMax != -1)
	{
		glUniform1f(shader->dispMax, displacementMax);
	}

	if (shader->u_Time != -1)
	{
		glUniform1f(shader->u_Time, u_Time);
	}

}

void fireObject::Update()
{
	u_Time = clock()/1000.0;
}