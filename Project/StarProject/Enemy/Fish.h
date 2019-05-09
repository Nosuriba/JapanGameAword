#pragma once
#include "Enemy.h"

class Player;
class Camera;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Die();

	void SwimUpdate();
	void DieUpdate();

	void searchMove();

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

