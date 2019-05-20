#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <array>

class Camera;

struct CrabInfo
{
	Position2 _pos;
	Position2 _prePos;
	Size _size;

	std::array<Vector2, 4> _vert;

	CrabInfo() : _pos(0, 0), _size(0, 0){};
	CrabInfo(const Position2& _pos, const Size& _size)
	{
		this->_pos = _pos;
		this->_size = _size;
	}
};

struct BossInfo
{
	CrabInfo _crab;
};

class Boss
{
private:

	std::shared_ptr<Camera>& _camera;
protected:
	BossInfo boss;

	Boss(std::shared_ptr<Camera>& camera);

public:
	~Boss();
	virtual void Draw();
	virtual void Update();
	virtual BossInfo GetInfo();

};