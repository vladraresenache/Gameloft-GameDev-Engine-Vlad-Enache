#pragma once
#include "../Utilities/Math.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;

	Vertex()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;

		norm.x = 0.0f;
		norm.y = 0.0f;
		norm.z = 0.0f;

		binorm.x = 0.0f;
		binorm.y = 0.0f;
		binorm.z = 0.0f;

		tgt.x = 0.0f;
		tgt.y = 0.0f;
		tgt.z = 0.0f;

		uv.x = 0.0f;
		uv.y = 0.0f;

	}

	Vertex(const Vertex &obj)
	{
		pos.x = obj.pos.x;
		pos.y = obj.pos.y;
		pos.z = obj.pos.z;

		norm.x = obj.norm.x;
		norm.y = obj.norm.y;
		norm.z = obj.norm.z;

		binorm.x = obj.binorm.x;
		binorm.y = obj.binorm.y;
		binorm.z = obj.binorm.z;

		tgt.x = obj.tgt.x;
		tgt.y = obj.tgt.y;
		tgt.z = obj.tgt.z;

		uv.x = obj.uv.x;
		uv.y = obj.uv.y;

	}
};

