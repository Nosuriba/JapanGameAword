#pragma once
#include "../Processing/Input.h"
#include "../Bubble.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	//XV
	virtual void Update(const Input& p) = 0;
};

