#pragma once
#include "../Processing/Geometry.h"
#include <vector>
#include <memory>

class Camera;

struct ObjectInfo {
	Vector2 _pos;
	Size _size;
	Rect _rect;
	int _level;
	bool _breakflag;
	bool _predatoryflag;
	ObjectInfo() :_pos(0, 0), _size(0, 0), _rect(_pos, _size), _breakflag(false), _predatoryflag(false){};
	ObjectInfo(const Vector2 &pos, const Size & size, const Rect& rect, const int& level, const bool& breakflag) {
		_pos = pos;
		_size = size;
		_rect = rect;
		_level = level;
		_breakflag = breakflag;
	}
	ObjectInfo(const Vector2 &pos, const Size & size, const Rect& rect, const int & level, const bool& breakflag, const bool& predatoryflag) {
		_pos = pos;
		_size = size;
		_rect = rect;
		_level = level;
		_breakflag = breakflag;
		_predatoryflag = predatoryflag;
	}
};

class Obstacle
{
private:
	std::vector<ObjectInfo> _obj;
	std::shared_ptr<Camera>& _camera;

protected:
	ObjectInfo obj;
public:
	Obstacle(std::shared_ptr<Camera>& camera);
	~Obstacle();
	void Draw();
	void Update();
};

