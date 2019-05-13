#pragma once
#include "../Processing/Input.h"
#include "../Bubble.h"

// ÃŞÊŞ¯¸Ş‚Ì’·‚¢‚©‚ç’Z‚­‚·‚é
#ifdef _DEBUG
const int WAITFRAME = 60;
#else
const int WAITFRAME = 120;
#endif // DEBUG


class Scene
{
public:
	Scene();
	virtual ~Scene();

	//XV
	virtual void Update(const Input& p) = 0;
};

