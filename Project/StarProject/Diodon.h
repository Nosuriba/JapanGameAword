#pragma once
#include "Enemy.h"
#include <memory>

class Camera;

class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();
	void Die();

	void SwimUpdate();		
	void SwellUpdate();		/// 膨らんだ状態
	void DieUpdate();

	void (Diodon::*updater)();

	bool _turnFlag;			/// true:右方向, false:左方向

	std::shared_ptr<Camera> _camera;

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

