#pragma once
#include <memory>
#include <array>
#include <vector>
#include <list>
#include "../Processing/Geometry.h"
#include "../Particle/Water.h"


class Camera;
class Player;

using sqr_vert = std::array<Vector2, 4>;

struct JointInfo
{
	// 関節の中間点
	std::vector<Vector2> _points;
	// 制御点
	Vector2 _ctlPoint;
	std::vector<Vector2> _center;
	Vector2 _vel;
	Size   _size;
	std::array<sqr_vert, 2> _vert;		// 関節の頂点
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// 蟹本体の頂点
	std::vector<JointInfo> _legs;	// 足の関節
	std::vector<JointInfo> _arms;	// 腕の関節

	CrabInfo() : _pos(0, 0), _size(0, 0) {};
	CrabInfo(const Position2& _pos, const Size& _size)
	{
		this->_pos = _pos;
		this->_size = _size;
	}
};


struct AttackInfo
{
	Vector2 pos;
	float r;
};

class Boss
{
protected:
	std::list<AttackInfo> at;

	const std::shared_ptr<Camera>& _camera;
	const std::shared_ptr<Player>& _player;

	Boss(const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Player>& _player);

public:
	~Boss();
	virtual void Draw() = 0;
	virtual void Update() = 0;

	std::list<AttackInfo> GetAttackInfo();

	virtual void OnDamage() = 0;
};