#pragma once
#include <string>
#include "../Geometry.h"

class Input;

class Object
{
public:
	Object();
	~Object();

	virtual void Update(const Input & p);
	virtual void Draw();
protected:
	
};

