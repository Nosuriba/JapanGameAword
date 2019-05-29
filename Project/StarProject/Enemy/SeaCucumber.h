#pragma once
#include "Enemy.h"

class SeaCucumber :
	public Enemy
{
private:
	void WaitUpdate();
	void MoveUpdate();
	void CounterUpdate();
	
	void MovePoint();			// 制御点の移動用

	void (SeaCucumber::*_updater)();

	Vector2 _pL;	// 左端
	Vector2 _pR;	// 右端


	int moveCnt;			// 移動制御用のｶｳﾝﾄ

	void DebugDraw();

public:
	SeaCucumber(std::shared_ptr<Camera>& c, std::shared_ptr<Player>& p);
	~SeaCucumber();

	void Update();
	void Draw();

	void OnDamage();
};

