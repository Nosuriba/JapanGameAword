#pragma once
#include "Geometry.h"
#include <vector>

struct ObjectInfo {
	Vector2 _pos;
	Size _size;
	ObjectInfo() :_pos(0, 0), _size(0, 0) {};
	ObjectInfo(const Vector2 &pos, const Size & size) {
		_pos = pos;
		_size = size;
	}
};

class Obstacle
{
private:
	std::vector<ObjectInfo> _obj;
public:
	Obstacle();
	~Obstacle();
	void Draw();
	void Update();
	void ObjCreate(const Vector2 &_pos, const Size & _size);
	std::vector<ObjectInfo> GetObjectInfo();
};

