#pragma once
#include "Enemy.h"

class Player;
class Camera;

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

	void SearchMove();			// �T�m����͈͂��ړ������Ă���
	void CalBezier();			// �޼ު�Ȑ��̌v�Z�p(3���޼ު)

	void (Fish::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Vector2> midPoints;			// �޼ު�Ȑ��̒��ԓ_
	std::vector<CtlInfo> cPoints;			// �޼ު�Ȑ��̐���_
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	EnemyInfo GetInfo();
	shot_vector ShotGetInfo();
	void CalEscapeDir(const Vector2& vec);
	void ShotDelete(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

