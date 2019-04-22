#pragma once
#include "Object.h"

class Input;

class Player :
	public Object
{
public:
	Player();
	~Player();
	void Update(const Input & p);
	void Draw();
private:
};

