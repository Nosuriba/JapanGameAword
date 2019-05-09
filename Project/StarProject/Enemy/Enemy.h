#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <array>

class Camera;

struct EnemyInfo {
	Position2 _pos;
	Size _size;
	Rect _rect;
	std::array<Vector2, 3> _searchVert;
	
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
	bool _turnFlag;			// true:�E����, false:������

	Enemy(std::shared_ptr<Camera>& camera);

public:
	~Enemy();
	virtual void Draw();
	virtual void Update();
	virtual EnemyInfo GetInfo();
	virtual void ChangeColor();
	virtual void ResetColor();
	virtual void CalTrackVel(const Vector2& pos);		// �ǔ����鑬�x�̌v�Z�p
};

