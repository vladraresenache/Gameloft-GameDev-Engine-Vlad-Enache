#pragma once
#include "../Utilities/Math.h"
#include "SceneObject.h"


class SkyBox : public SceneObject {
public:

	Vector3 followingCamera;

	void sendSpecificData();
	void Update();
	
};