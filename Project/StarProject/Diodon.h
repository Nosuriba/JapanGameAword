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
	void SwellUpdate();		/// 膨らんだ状態
	void DieUpdate();

	void (Diodon::*updater)();

	bool _turnFlag;			/// true:右方向, false:左方向

public:
	Diodon();
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

