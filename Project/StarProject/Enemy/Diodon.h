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
	void SwellUpdate();		/// 膨らんだ状態
	void ShotUpdate();
	void EscapeUpdate();
	void DieUpdate();

	bool CheckOutScreen();		 // ｼｮｯﾄが画面外に行ったかの判定用

	void (Diodon::*_updater)();

	std::shared_ptr<Camera> _camera;
	std::array<Vector2, 8>  _dirPos;		// ｼｮｯﾄする方向

	int riseCnt;			// 浮上するﾀｲﾐﾝｸﾞを調整するためのｶｳﾝﾄ
	int blastCnt;			// 爆破するまでの間隔

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	EnemyInfo GetInfo();
	shot_vector GetShotInfo();
	void CalEscapeDir(const Vector2& vec);
	void ShotDelete(const int& num);
	void CalTrackVel(const Vector2& pos);
};

