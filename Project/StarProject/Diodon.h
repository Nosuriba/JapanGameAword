#pragma once
#include "Enemy.h"
class Diodon :
	public Enemy
{
private:
public:
	Diodon();
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

