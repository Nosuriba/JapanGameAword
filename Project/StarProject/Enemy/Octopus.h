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
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	const int T = 12;	//�֐ߐ�
	E_LEG_STATE state;	//���
	EnemyInfo cell;
};

struct Oct {
	float r;		//���̒���
	Vector2 center;	//���S���W
	Vector2 root;	//���̍���
	std::vector<E_Leg> legs;	//��
};

class Octopus :
	public Enemy
{
private:
	int angle;

	void Die();
	void DieUpdate();

	void NeturalUpdate();

	void (Octopus::*_updater)();
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
