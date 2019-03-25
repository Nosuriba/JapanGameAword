#pragma once
#include <string>
#include "../Geometry.h"

class Input;

class Object
{
public:
	Object();
	~Object();
	void Init(std::string fileName, const Vector3& pos);
	virtual void Update(const Input & p);
	virtual void Draw();
protected:
	std::string fileName;
	VECTOR		startPos, endPos, hitPos2D, hitPos3D, modelPos3D;
	Vector3		pos;
	Vector3		catchPos;
	int			mouseX, mouseY;
	int			handle;
	bool dieFlag, hitFlag, catchFlag;
};

