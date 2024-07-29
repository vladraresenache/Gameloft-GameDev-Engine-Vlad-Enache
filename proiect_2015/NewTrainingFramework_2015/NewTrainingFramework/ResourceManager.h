#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include <map>
#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"
class ResourceManager
{
private:
	static ResourceManager* spInstance;
	ResourceManager();
public:
	std::map<int, ModelResource*> mm;
	std::map<int, ShaderResource*> sm;
	std::map<int, TextureResource*> tm;

	std::map<int, ModelR*> loaded_mm;
	std::map<int, ShaderR*> loaded_sm;
	std::map<int, TextureR*> loaded_tm;

	void Init(const std::string& filePath);
	static ResourceManager* getInstance();
	TextureR* loadTexture(int dest_id);
	ShaderR* loadShader(int dest_id);
	ModelR* loadModel(int dest_id);
	void freeResources();
	void showEverything();

};

