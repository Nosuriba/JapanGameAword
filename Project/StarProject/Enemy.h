#pragma once
#include "Geometry.h"

struct EnemyInfo {
	Position2 _pos;
	Size _size;
	EnemyInfo() :_pos(0, 0), _size(0, 0) {};
	EnemyInfo(const Position2& _pos, const Size& _size) {
		this->_pos = _pos;
		this->_size = _size;
	}
};

class Enemy
{
private:
	EnemyInfo enemy;
	int color;
public:
	Enemy();
	~Enemy();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

