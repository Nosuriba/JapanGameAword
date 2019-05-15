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

	void SearchMove();			// 探知する範囲を移動させている
	void CalBezier();			// ﾍﾞｼﾞｪ曲線の計算用(3次ﾍﾞｼﾞｪ)

	void (Fish::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Vector2> midPoints;			// ﾍﾞｼﾞｪ曲線の中間点
	std::vector<CtlInfo> cPoints;			// ﾍﾞｼﾞｪ曲線の制御点
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	EnemyInfo GetInfo();
	shot_vector ShotGetInfo();
	void CalEscapeDir(const Vector2& vec);
	void ShotDelete(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

