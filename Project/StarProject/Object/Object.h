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
	VECTOR startPos, endPos, scrPos;
	Vector3		pos;
	Vector3		dbgRay;
	int			handle;
	bool dieFlag, hitFlag;
	void DebugDraw();
};

