#pragma once
#include "Enemy.h"

class Camera;

class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();

	void SwimUpdate();		
	void SwellUpdate();		/// 膨らんだ状態

	void (Diodon::*updater)();

	std::shared_ptr<Camera> _camera;

	int riseCnt;			/// 浮上するﾀｲﾐﾝｸﾞを調整するためのｶｳﾝﾄ

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

