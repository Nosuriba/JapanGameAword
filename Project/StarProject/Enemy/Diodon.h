#pragma once
#include "Enemy.h"

class Diodon :
	public Enemy
{
private:
	void SwimUpdate();		
	void SwellUpdate();		/// ñcÇÁÇÒÇæèÛë‘
	void EscapeUpdate();
	void DieUpdate();

	void (Diodon::*_updater)();

	int	_escapeTime;	// ì¶ëñåoâﬂéûä‘

	void Move();
	void Search();

	void DebugDraw();
	void DrawNeedle(const Vector2& p, const Vector2& v, const float r);

public:
	Diodon(std::shared_ptr<Camera>& c, std::shared_ptr<Player>& p);
	~Diodon();

	void Update();
	void Draw();

	void OnDamage();
};

