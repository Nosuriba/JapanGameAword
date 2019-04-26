#pragma once
#include "Enemy.h"

class Player;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Die();

	void SwimUpdate();
	void DieUpdate();

	void (Fish::*updater)();
public:
	Fish();
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

