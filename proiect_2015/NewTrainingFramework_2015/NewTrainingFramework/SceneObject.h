#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"
#include "Camera.h"

class SceneObject {
public:
	int id;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	ModelR* model;
	ShaderR* shader;
	std::string type;
	std::vector<TextureR*> textures;
	bool depth_text;

	void Draw();
	void sendCommonData();
	void virtual sendSpecificData();
	void virtual Update();

};