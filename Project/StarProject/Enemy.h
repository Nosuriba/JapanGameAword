#pragma once
#include "Geometry.h"
#include <memory>

class Camera;

struct EnemyInfo {
	Position2 _pos;
	Size _size;
	Rect _rect;
	
	EnemyInfo() :_pos(0, 0), _size(0, 0), _rect(_pos, _size){};
	EnemyInfo(const Position2& _pos, const Size& _size, const Rect& _rect) {
		this->_pos  = _pos;
		this->_size = _size;
		this->_rect = _rect;
	}
};

class Enemy
{
private:
	std::shared_ptr<Camera>& _camera;

protected:
	EnemyInfo enemy;
	Vector2 _vel;
	int color;
	Enemy(std::shared_ptr<Camera>& camera);

public:
	~Enemy();
	virtual void Draw();
	virtual void Update();
	virtual EnemyInfo GetInfo();
	virtual void ChangeColor();
	virtual void ResetColor();
};

