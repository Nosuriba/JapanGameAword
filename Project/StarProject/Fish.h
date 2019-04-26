#pragma once
#include "Enemy.h"
class Fish :
	public Enemy
{
public:
	Fish();
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

