#pragma once
#include "Object.h"
class Enemy :
	public Object
{
public:
	Enemy();
	~Enemy();
	void Update();
	void Draw();
};

