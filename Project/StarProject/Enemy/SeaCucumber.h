#pragma once
#include "Enemy.h"
#include "../Particle/Bubble.h"

class SeaCucumber :
	public Enemy
{
private:
	std::vector<std::shared_ptr<Particle>> _particle;

	void WaitUpdate();
	void MoveUpdate();
	void CounterUpdate();
	
	void (SeaCucumber::*_updater)();

	Vector2 _pL;	// ç∂í[
	Vector2 _pR;	// âEí[
	const std::shared_ptr<Camera>& _camera;
	void DebugDraw();

public:
	SeaCucumber(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos);
	~SeaCucumber();

	void Update();
	void Draw();

	void OnDamage();

	void CreateDamagePoints();
	void CreateAttackPoints();
};
