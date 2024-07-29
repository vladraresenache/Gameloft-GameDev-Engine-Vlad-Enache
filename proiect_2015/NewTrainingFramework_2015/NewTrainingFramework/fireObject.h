#pragma once
#include "../Utilities/Math.h"
#include "SceneObject.h"


class fireObject : public SceneObject {
public:

	float displacementMax;
	float u_Time;
	
	void sendSpecificData();
	void Update();
};