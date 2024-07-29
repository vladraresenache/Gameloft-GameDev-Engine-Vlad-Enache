#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"
#include "Camera.h"

class Lights {
public:
	
	Vector3 direction;
	int object_id;
	int light_id;
	std::string type;
	Vector3 diffuseColor;
	Vector3 specularColor;
	float specPower;

	

};