#pragma once
#include "Enemy.h"

class Player;
class Camera;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Tracking();
	void Die();

	void SwimUpdate();
	void TrackingUpdate();
	void DieUpdate();

	void searchMove();

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;

	Vector2 _starPos;			// ﾌﾟﾚｲﾔｰの座標保管用
	int invCnt = 0;				// 移動を止める間隔用
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
	void CalTrackVel(const Vector2& pos);
};

