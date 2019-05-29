#pragma once
#include <memory>
#include <array>
#include <vector>
#include <list>
#include "../Camera.h"
#include "../Player.h"
#include "../Processing/Geometry.h"
#include "../Particle/Water.h"

struct AttackInfo
{
	Vector2 _pos;
	float _r;

	AttackInfo() :_pos(0,0), _r(0) {}
	AttackInfo(const Vector2& p, const float& r) :_pos(p), _r(r) {}
};

struct DamageInfo
{
	Vector2 _pos;
	float _r;

	DamageInfo() :_pos(0, 0), _r(0) {}
	DamageInfo(const Vector2& p, const float& r) :_pos(p), _r(r) {}
};

struct ShotInfo
{
	Position2 _pos;
	Vector2 _vel;
	Size _size;
	ShotInfo() : _pos(0, 0), _vel(0, 0), _size(0, 0) {};
	ShotInfo(const Position2& p, const Vector2& v, const Size& s)
	{
		_pos = p;
		_vel = v;
		_size = s;
	}
};

class Boss
{
protected:
	std::list<AttackInfo> at;
	std::list <DamageInfo> da;

	const std::shared_ptr<Camera>& _camera;
	const std::shared_ptr<Player>& _player;

	Boss(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p);

public:
	~Boss();
	virtual void Draw() = 0;
	virtual void ShadowDraw() = 0;
	virtual void Update() = 0;

	std::list<AttackInfo> GetAttackInfo();
	std::list<DamageInfo> GetDamageInfo();

	virtual void OnDamage() = 0;
};