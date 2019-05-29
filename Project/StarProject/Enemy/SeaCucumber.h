#pragma once
#include "Enemy.h"

class SeaCucumber :
	public Enemy
{
private:
	void WaitUpdate();
	void MoveUpdate();
	void CounterUpdate();
	
	void MovePoint();			// ����_�̈ړ��p

	void (SeaCucumber::*_updater)();

	Vector2 _pL;	// ���[
	Vector2 _pR;	// �E�[


	int moveCnt;			// �ړ�����p�̶���

	void DebugDraw();

public:
	SeaCucumber(std::shared_ptr<Camera>& c, std::shared_ptr<Player>& p);
	~SeaCucumber();

	void Update();
	void Draw();

	void OnDamage();
};

