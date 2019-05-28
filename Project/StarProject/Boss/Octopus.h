#pragma once
#include "Boss.h"
#include <vector>
#include <array>

class Camera;

enum class E_LEG_STATE {
	NORMAL,
	PUNCH,
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
	int angle;		//足の角度
	int cnt;		//足ごとの動き出すタイミング調整用
};

struct Oct {
	float r ;		//足の長さ
	Vector2 center;	//中心座標
	Vector2 movePos;	//回転する向きの座標
	Vector2 hedPos;		//頭の座標
	std::array<Vector2,2> eyePos;		//目の座標
	std::vector<Vector2> root;	//足の根元
	std::vector<E_Leg> legs;	//足
	int helth = 200;		//体力
	int interval = 0;		//無敵時間

};

class Octopus :
	public Boss
{
private:
	bool _damageFlag;
	int _wait;
	int _maxAngle;
	int _idx;
	int _timer;
	Vector2 _targetPos;
	Vector2 _vec;

	std::vector<std::shared_ptr<Particle>> _particle;

	void IkCcd(Vector2 pos,int idx,int numMaxItaration);

	void Die();
	void DieUpdate();
	void Normal(int idx);
	void Punch(int idx);
	void OctInk();
	void Chase(int idx);
	void ReMove(int idx);

	void LegMove(E_Leg& leg, int idx);
	//void Move();

	void NeturalUpdate();

	void (Octopus::*_updater)();
	std::shared_ptr<Camera>& _camera;
	Oct _oct;
public:
	Octopus(std::shared_ptr<Camera>& camera);
	~Octopus();
	void Damage();
	void Draw();
	void SelectDraw(const Vector2 p, const float s);
	void Update();
	BossInfo GetInfo();
	void CalTrackVel(const Vector2& pos);
};

