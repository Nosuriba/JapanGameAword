#pragma once
#include "Enemy.h"

class Player;
class Camera;

struct CtlInfo
{
	Vector2 pos;
	Vector2 vel;
	bool flag;

	CtlInfo() : pos(0, 0), vel(0, 0), flag(false) {};
	CtlInfo(const Vector2& pos, const Vector2& vel, bool flag)
	{
		this->pos  = pos;
		this->vel  = vel;
		this->flag = flag;
	}

};

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
	void CalCurve();			// ﾍﾞｼﾞｪ曲線の計算用(3次ﾍﾞｼﾞｪ)

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<std::vector<Vector2>> dCtlPos;
	std::vector<Vector2> midPoint;			// 制御点登録用(debug用)
	std::vector<CtlInfo> ctlPoint;
	std::vector<int> divImage;
	int image;			// 画像ID

	
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

