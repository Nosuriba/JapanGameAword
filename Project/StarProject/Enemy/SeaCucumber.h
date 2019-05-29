#pragma once
#include "Enemy.h"

class SeaCucumber :
	public Enemy
{
private:
	void WaitUpdate();
	void MoveUpdate();
	void CounterUpdate();
	
	void (SeaCucumber::*_updater)();

	Vector2 _pL;	// ç∂í[
	Vector2 _pR;	// âEí[

	void DebugDraw();

public:
	SeaCucumber(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos);
	~SeaCucumber();

	void Update();
	void Draw();

	void OnDamage();
};

