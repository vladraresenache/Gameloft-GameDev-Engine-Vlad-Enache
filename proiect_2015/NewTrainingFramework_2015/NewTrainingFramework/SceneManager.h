#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"
#include "Camera.h"
#include "SceneObject.h"
#include "Lights.h"
#include "../Include/rapidxml.hpp"
#include "../Include/rapidxml_utils.hpp"
#include "../Include/rapidxml_iterators.hpp"
#include "../Include/rapidxml_print.hpp"
#include <map>

struct Control {
	std::string key;
	std::string action;
};

struct Screen {
	int width;
	int height;
	int r;
	int g;
	int b;
	std::vector<Control*> controls;
};



class SceneManager
{
private:
	static SceneManager* spInstance;
	SceneManager() ;
public:
	std::string gameName;
	Screen screenProp;
	int activeCamera;
	std::map<int, Camera*> Cameras;
	std::map<int, SceneObject*> objects;

	int s_r;
	int b_r;
	Vector4 fog_color;

	Vector3 ambientalColor;
	float ambientalRatio;

	std::map<int, Lights*> lights;

	void Init(const std::string& xmlFile);
	void Draw();
	void Update();
	static SceneManager* getInstance();
	Camera* getActiveCamera();
	void showEverything();
};