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
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	const int T = 6;	//�֐ߐ�
	LEG_STATE state;	//���
};

struct Oct {
	float r;		//���̒���
	Vector2 center;	//���S���W
	Vector2 root;	//���̍���
	std::vector<Leg> legs;	//��
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

