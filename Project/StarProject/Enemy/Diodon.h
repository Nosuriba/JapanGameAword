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
	void SwellUpdate();		/// –c‚ç‚ñ‚¾ó‘Ô
	void ShotUpdate();
	void EscapeUpdate();
	void DieUpdate();

	void (Diodon::*_updater)();

	std::shared_ptr<Camera> _camera;
	std::array<Vector2, 8>  _dirPos;		// ¼®¯Ä‚·‚é•ûŒü

	int riseCnt;			// •‚ã‚·‚éÀ²Ğİ¸Ş‚ğ’²®‚·‚é‚½‚ß‚Ì¶³İÄ
	int blastCnt;			// ”š”j‚·‚é‚Ü‚Å‚ÌŠÔŠu

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	EnemyInfo GetInfo();
	void CalEscapeDir(const Vector2& vec);
	void CalTrackVel(const Vector2& pos);
};

