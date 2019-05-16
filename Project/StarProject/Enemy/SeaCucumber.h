#pragma once
#include "Enemy.h"

class SeaCucumber :
	public Enemy
{
private:
	void Crawl();
	void Escape();
	void Die();

	void CrawlUpdate();
	void EscapeUpdate();
	void DieUpdate();
	
	void MovePoint();			// §Œä“_‚ÌˆÚ“®—p

	void (SeaCucumber::*_updater)();

	std::shared_ptr<Camera> _camera;

	CtlInfo cPoint;

	int moveCnt;			// ˆÚ“®§Œä—p‚Ì¶³İÄ
public:
	SeaCucumber(std::shared_ptr<Camera>& camera);
	~SeaCucumber();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	EnemyInfo GetInfo();
	shot_vector GetShotInfo();
	void CalEscapeDir(const Vector2& vec);
	void ShotDelete(const int& num);
	void CalTrackVel(const Vector2& pos, bool col);
};

