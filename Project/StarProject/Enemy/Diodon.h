#pragma once
#include "Enemy.h"

class Camera;

class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();
	void Shot();
	void Escape();
	void Die();

	void SwimUpdate();		
	void SwellUpdate();		/// �c��񂾏��
	void ShotUpdate();
	void EscapeUpdate();
	void DieUpdate();

	bool CheckOutScreen();		 // ���Ă���ʊO�ɍs�������̔���p

	void (Diodon::*updater)();

	std::shared_ptr<Camera> _camera;
	std::array<Vector2, 8>  _dirPos;		// ���Ă������

	int riseCnt;			// ���シ�����ݸނ𒲐����邽�߂̶���
	int blastCnt;			// ���j����܂ł̊Ԋu

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	shot_vector GetShotInfo();
	void ChangeShotColor(const int& num);
	void ChangeColor();
	void ResetColor();
	void CalTrackVel(const Vector2& pos, bool col);
};

