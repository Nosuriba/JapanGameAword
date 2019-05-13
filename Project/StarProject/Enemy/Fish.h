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
	void CalCurve();			// �޼ު�Ȑ��̌v�Z�p
	void CalNormalVec();		// �@���޸�ٌv�Z�p

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<std::vector<Vector2>> dCtlPos;
	std::vector<Vector2> sPoints;
	std::vector<CtlInfo> ctlPoint;
	std::vector<Vector2> midPos;
	std::array<EnemyInfo, 5> divEnemy;		/// �����������W
	std::array<int, 5> divImage;
	Vector2 ctlPos;		// ����_
	Vector2 ctlVel;		// ����_�̑��x���Z�p
	int image;			// �摜ID
	bool ctlFlag;		// true:���Z, false:���Z

	
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

