#pragma once
#include "../Processing/Geometry.h"
#include "../Particle/Water.h"
#include <memory>

class Camera;

struct CrabInfo
{
	Position2 _pos;
	Position2 _prePos;
	Size _size;
	Rect _rect;

	CrabInfo() : _pos(0, 0), _size(0, 0), _rect(_pos, _size) {};
	CrabInfo(const Position2& _pos, const Size& _size, const Rect& _rect)
	{
		this->_pos = _pos;
		this->_size = _size;
		this->_rect = _rect;
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
	virtual void CalTrackVel(const Vector2& pos);

};