#pragma once
#include "Enemy.h"
class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();
	void Die();

	void SwimUpdate();
	void SwellUpdate();
	void DieUpdate();

	void (Diodon::*updater)();

public:
	Diodon();
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

