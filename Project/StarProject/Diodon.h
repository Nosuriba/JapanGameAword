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
	void SwellUpdate();		/// –c‚ç‚ñ‚¾ó‘Ô
	void DieUpdate();

	void (Diodon::*updater)();

	bool _turnFlag;			/// true:‰E•ûŒü, false:¶•ûŒü

public:
	Diodon();
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

