#pragma once

#include <memory>

#include "../Processing/Geometry.h"
#include "../Camera.h"

struct ObjectInfo {
	Vector2 _pos;
	Size	_size;
	Rect	_rect;
	int		_level;
	bool	_breakflag;
	bool	_hitflag;

	ObjectInfo() :_pos(0, 0), _size(0, 0), _rect(_pos, _size), _breakflag(false), _hitflag(false){};
	ObjectInfo(const Vector2 &pos, const Size & size, const Rect& rect, const int& level, const bool& breakflag) {
		_pos	= pos;
		_size	= size;
		_rect	= rect;
		_level	= level;
		_breakflag	= breakflag;
	}
	ObjectInfo(const Vector2 &pos, const Size & size, const Rect& rect, const int & level, const bool& breakflag, const bool& hitflag) {
		_pos	= pos;
		_size	= size;
		_rect	= rect;
		_level	= level;
		_breakflag	= breakflag;
		_hitflag	= hitflag;
	}
};

class Obstacle
{
protected:
	const std::shared_ptr<Camera>& _camera;

	ObjectInfo obj;

public:
	Obstacle(const std::shared_ptr<Camera>& camera);
	~Obstacle();

	virtual void Draw() = 0;
	virtual void Update() = 0;
};

