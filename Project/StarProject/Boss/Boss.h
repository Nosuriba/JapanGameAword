#pragma once
#include <memory>
#include <array>
#include <vector>
#include <list>
#include "../Processing/Geometry.h"
#include "../Particle/Water.h"
#include "../Camera.h"
#include "../Player.h"

class Camera;
class Player;

struct AttackInfo
{
	Vector2 _pos;
	float _r;

	AttackInfo() : _pos(0, 0), _r(0) {};
	AttackInfo(const Vector2& p, const float& r)
	{
		_pos = p;
		_r = r;
	}
};

class Boss
{
protected:
	std::list<AttackInfo> at;

	const std::shared_ptr<Camera>& _camera;
	const std::shared_ptr<Player>& _player;

	Boss(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p);

public:
	~Boss();
	virtual void Draw() = 0;
	virtual void Update() = 0;

	std::list<AttackInfo> GetAttackInfo();

	virtual void OnDamage() = 0;
};