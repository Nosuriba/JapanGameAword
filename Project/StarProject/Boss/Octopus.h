#pragma once
#include "Boss.h"
#include <vector>

class Camera;

enum class E_LEG_STATE {
	NORMAL,
	PUNCH,
	OCT_INK,
	SWEEP,
	DAMAGE,
	DETH,
};

struct E_Leg {
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	const int T = 12;	//�֐ߐ�
	E_LEG_STATE state;	//���
	int angle;		//�ڕW�܂ł̊p�x
};

struct Oct {
	float r;		//���̒���
	Vector2 center;	//���S���W
	Vector2 root;	//���̍���
	std::vector<E_Leg> legs;	//��
};

class Octopus :
	public Boss
{
private:
	int angle;
	Vector2 targetPos;
	void Die();
	void DieUpdate();
	void Normal(E_Leg& leg,Vector2 pos);
	void Punch(E_Leg& leg, Vector2 pos);
	void OctInk(E_Leg& leg, Vector2 pos);
	void Sweep(E_Leg& leg, Vector2 pos);
	void Damage();

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

