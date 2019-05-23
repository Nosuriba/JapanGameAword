#pragma once
#include "Boss.h"
#include <vector>

class Camera;

enum class E_LEG_STATE {
	NORMAL,
	PUNCH,
	OCT_INK,
	CHASE,
	RE_MOVE,
	DAMAGE,
	DETH,
};

struct E_Leg {
	Vector2 tip;	//先端座標
	std::vector<Vector2> joint;	//関節座標
	MATRIX mat;		//回転角
	const int T = 12;	//関節数
	E_LEG_STATE state;	//状態
	int angle;		//目標までの角度
	int cnt;		//足ごとの動き出すタイミング調整用
};

struct Oct {
	const float r =500;		//足の長さ
	Vector2 center;	//中心座標
	std::vector<Vector2> root;	//足の根元
	std::vector<E_Leg> legs;	//足
};

class Octopus :
	public Boss
{
private:
	int _wait;
	int _maxAngle;
	int _idx;
	Vector2 _targetPos;
	Vector2 _vec;
	void Die();
	void DieUpdate();
	void Normal(int idx);
	void Punch(E_Leg& leg, int idx);
	void OctInk(E_Leg& leg, int idx);
	void Chase(E_Leg& leg, int idx);
	void Damage();
	void ReMove(E_Leg& leg, int idx);

	void LegMove(E_Leg& leg, int idx);

	void NeturalUpdate();

	void (Octopus::*_updater)();
	std::shared_ptr<Camera>& _camera;
	Oct _oct;
public:
	Octopus(std::shared_ptr<Camera>& camera);
	~Octopus();
	void Draw();
	void Update();
	BossInfo GetInfo();
	void CalTrackVel(const Vector2& pos);
};

