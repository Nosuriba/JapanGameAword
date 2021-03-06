#pragma once
#include "Enemy.h"

class Diodon :
	public Enemy
{
private:
	void SwimUpdate();		
	void SwellUpdate();		/// 膨らんだ状態
	void EscapeUpdate();
	void DieUpdate();

	void (Diodon::*_updater)();

	int	_escapeTime;	// 逃走経過時間

	void Move();
	void Search();

	void DebugDraw();
	void DrawNeedle(const Vector2& p, const Vector2& v, const float r);

public:
	Diodon(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos);
	~Diodon();

	void Update();
	void Draw();
	void Shadow();

	void OnDamage();

	void CreateDamagePoints();
	void CreateAttackPoints();
};

