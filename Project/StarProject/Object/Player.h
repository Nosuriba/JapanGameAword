#pragma once
#include "Object.h"
class Player :
	public Object
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
	void DebugDraw();
private:
	void CheckHitModel();
	void MoveModel();
};

