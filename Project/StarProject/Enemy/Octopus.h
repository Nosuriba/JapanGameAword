#pragma once
#include "Enemy.h"

class Camera;

enum class E_LEG_STATE {
	NORMAL,
	ATTACK,
	DAMAGE,
	DETH,
};

struct E_Leg {
	Vector2 tip;	//先端座標
	std::vector<Vector2> joint;	//関節座標
	const int T = 6;	//関節数
	E_LEG_STATE state;	//状態
};

struct Oct {
	float r;		//足の長さ
	Vector2 center;	//中心座標
	Vector2 root;	//足の根元
	std::vector<E_Leg> legs;	//足
};

class Octopus :
	public Enemy
{
private:
	int angle;

	const int numMaxItr = 6;

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

