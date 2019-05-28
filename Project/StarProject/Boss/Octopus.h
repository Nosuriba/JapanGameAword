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
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	MATRIX mat;		//��]�p
	const int T = 12;	//�֐ߐ�
	E_LEG_STATE state;	//���
	int angle;		//���̊p�x
	int cnt;		//�����Ƃ̓����o���^�C�~���O�����p
};

struct Oct {
	float r ;		//���̒���
	Vector2 center;	//���S���W
	Vector2 movePos;	//��]��������̍��W
	Vector2 hedPos;		//���̍��W
	std::array<Vector2,2> eyePos;		//�ڂ̍��W
	std::vector<Vector2> root;	//���̍���
	std::vector<E_Leg> legs;	//��
	int helth = 200;		//�̗�
	int interval = 0;		//���G����

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

