#pragma once
#include "Enemy.h"

class Player;
class Camera;

struct CtlInfo
{
	Vector2 pos;
	Vector2 vel;
	bool flag;

	CtlInfo() : pos(0, 0), vel(0, 0), flag(false) {};
	CtlInfo(const Vector2& pos, const Vector2& vel, bool flag)
	{
		this->pos  = pos;
		this->vel  = vel;
		this->flag = flag;
	}

};

class Fish :
	public Enemy
{
private:
	void Swim();
	void Escape();
	void Die();

	void SwimUpdate();
	void EscapeUpdate();
	void DieUpdate();

	void searchMove();			// �T�m����͈͂��ړ������Ă���
	void CalCurve();			// �޼ު�Ȑ��̌v�Z�p(3���޼ު)

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<std::vector<Vector2>> dCtlPos;
	std::vector<Vector2> midPoint;			// ����_�o�^�p(debug�p)
	std::vector<CtlInfo> ctlPoint;
	std::vector<int> divImage;
	int image;			// �摜ID

	
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	shot_vector ShotGetInfo();
	void CalEscapeDir(const Vector2& vec);
	void ChangeShotColor(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

