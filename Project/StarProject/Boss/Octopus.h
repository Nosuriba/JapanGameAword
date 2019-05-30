#pragma once
#include "Boss.h"
#include <vector>
#include <array>


enum class E_LEG_STATE {
	NORMAL,
	PUNCH,
	CHASE,
	RE_MOVE,
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
	bool _returnFlag;
	bool _damageFlag;
	int _wait;
	int _maxAngle;
	int _idx;
	int _timer;
	int BGM;
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
	void HitUpd();

	void NeturalUpdate();
	void DebugDraw();

	void (Octopus::*_updater)();
	Oct _oct;
public:
	Octopus(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos = Vector2(1000, 600));
	~Octopus();
	void OnDamage();
	void Draw();
	void ShadowDraw();
	int GetDrawHandle();
	void SelectDraw(const Vector2 p, const float s);
	void Update();
	void HitBlock();
};

