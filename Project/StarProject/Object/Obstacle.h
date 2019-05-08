#pragma once
#include "../Processing/Geometry.h"
#include <vector>
#include <memory>

class Camera;

struct ObjectInfo {
	Vector2 _pos;
	Size _size;
	Rect _rect;
	ObjectInfo() :_pos(0, 0), _size(0, 0), _rect(_pos,_size) {};
	ObjectInfo(const Vector2 &pos, const Size & size, const Rect& rect) {
		_pos = pos;
		_size = size;
		_rect = rect;
	}
};

class Obstacle
{
private:
	std::vector<ObjectInfo> _obj;
	std::shared_ptr<Camera>& _camera;

	int rockimg;

public:
	Obstacle(std::shared_ptr<Camera>& camera);
	~Obstacle();
	void Draw();
	void Update();
};

