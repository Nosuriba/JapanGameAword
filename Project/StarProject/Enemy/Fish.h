#pragma once
#include "Enemy.h"

class Player;
class Camera;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Tracking();
	void Die();

	void SwimUpdate();
	void TrackingUpdate();
	void DieUpdate();

	void searchMove();

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;

	Vector2 _starPos;			// ��ڲ԰�̍��W�ۊǗp
	int invCnt = 0;				// �ړ����~�߂�Ԋu�p
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
	void CalTrackVel(const Vector2& pos);
};

