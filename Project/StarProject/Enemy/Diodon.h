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
	void Die();

	void SwimUpdate();		
	void SwellUpdate();		/// 膨らんだ状態
	void ShotUpdate();
	void DieUpdate();

	void (Diodon::*updater)();

	std::shared_ptr<Camera> _camera;
	std::array<Vector2, 8>  _dirPos;		// ｼｮｯﾄする方向

	int riseCnt;			// 浮上するﾀｲﾐﾝｸﾞを調整するためのｶｳﾝﾄ
	int blastCnt;			// 爆破するまでの間隔

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

