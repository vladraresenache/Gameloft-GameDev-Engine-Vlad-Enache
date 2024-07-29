#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include "SceneObject.h"

class TerrainObject: public SceneObject {
public:
	int nr_cel;
	int dimCel;
	int offsetY;
	Vector3 heights; 
	std::vector<Vertex> TVert;
	std::vector<unsigned short int> TInd;
	Vector2 deplasament;
	void sendSpecificData();
	void Update();

};