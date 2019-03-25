#pragma once
#include "Object.h"

class Input;

class Enemy :
	public Object
{
public:
	Enemy();
	~Enemy();
	void Update(const Input & p);
	void Draw();
};

