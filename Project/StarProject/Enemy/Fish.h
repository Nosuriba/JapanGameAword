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
	void CalCurve();			// ﾍﾞｼﾞｪ曲線の計算用
	void CalNormalVec();		// 法線ﾍﾞｸﾄﾙ計算用

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;

	std::vector<Vector2> midPos;
	std::array<EnemyInfo, 5> divEnemy;		/// 分割した座標
	std::array<int, 5> divImage;
	Vector2 ctlPos;		// 制御点
	Vector2 ctlVel;		// 制御点の速度加算用
	int image;			// 画像ID
	bool ctlFlag;		// true:加算, false:減算

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

