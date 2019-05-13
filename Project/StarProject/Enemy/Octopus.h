#pragma once
#include "Enemy.h"

class Camera;

enum class LEG_STATE {
	NORMAL,
	ATTACK,
	DAMAGE,
	DETH,
};

struct Leg {
	Vector2 tip;	//先端座標
	std::vector<Vector2> joint;	//関節座標
	const int T = 6;	//関節数
	LEG_STATE state;	//状態
};

struct Oct {
	float r;		//足の長さ
	Vector2 center;	//中心座標
	Vector2 root;	//足の根元
	std::vector<Leg> legs;	//足
};

class Octopus :
	public Enemy
{
private:

	void Die();
	void DieUpdate();

	void FootMove();

	void (Octopus::*updater)();
	std::shared_ptr<Camera>& _camera;
	Oct _oct;
public:
	Octopus(std::shared_ptr<Camera>& camera);
	~Octopus();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	shot_vector GetShotInfo();
	void CalEscapeDir(const Vector2& vec);
	void ChangeShotColor(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

