#pragma once
#include "Object.h"

class Input;

class Player :
	public Object
{
public:
	Player();
	~Player();
	void Update(const Input & p);
	void Draw();
	void DebugDraw();
private:
	void CheckHitModel(const Input & p);
	void MoveModel(const Input & p);
};

