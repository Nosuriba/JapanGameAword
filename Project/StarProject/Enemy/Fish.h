#pragma once
#include "Enemy.h"

class Player;
class Camera;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Escape();
	void Die();

	void SwimUpdate();
	void EscapeUpdate();
	void DieUpdate();

	void searchMove();			// 探知する範囲を移動させている

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;

	std::vector<Vector2> midPos;
	Vector2 ctlPos;		/// 制御点
	float ctlVel;		/// 制御点の速度加算用

	const int mPoint = 10;
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	shot_vector ShotGetInfo();
	void CalEscapeDir(const Vector2& vec);
	void ChangeShotColor(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

