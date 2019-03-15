#pragma once
#include <string>
#include "../Geometry.h"

class Object
{
public:
	Object();
	~Object();
	void Init(std::string fileName, const Vector3& pos, int handle);
	virtual void Update();
protected:
	Vector3 pos;
	bool dieFlag, hitFlag;
	
};

