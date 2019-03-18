#pragma once
#include <string>
#include "../Geometry.h"

class Object
{
public:
	Object();
	~Object();
	void Init(std::string fileName, const Vector3& pos);
	virtual void Update();
	virtual void Draw();
protected:
	std::string fileName;
	Vector3		pos;
	int			handle;
	bool dieFlag, hitFlag;
	
};

