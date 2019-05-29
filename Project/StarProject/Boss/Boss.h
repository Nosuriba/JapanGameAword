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