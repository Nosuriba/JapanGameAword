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
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	MATRIX mat;		//��]�p
	const int T = 12;	//�֐ߐ�
	E_LEG_STATE state;	//���
	int angle;		//�ڕW�܂ł̊p�x
	int cnt;		//�����Ƃ̓����o���^�C�~���O�����p
};

struct Oct {
	const float r =500;		//���̒���
	Vector2 center;	//���S���W
	std::vector<Vector2> root;	//���̍���
	std::vector<E_Leg> legs;	//��
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

