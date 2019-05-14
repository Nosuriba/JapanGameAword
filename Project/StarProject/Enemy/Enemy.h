#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <vector>
#include <array>

class Camera;

struct EnemyInfo {
	Position2 _pos;		// �G�̒��S�_
	Size _size;
	Rect _rect;
	std::array<Vector2, 3> _searchVert;
	bool _dieFlag;		// true:���S, false:����
	
	EnemyInfo() :_pos(0, 0), _size(0, 0), _rect(_pos, _size){};
	EnemyInfo(const Position2& _pos, const Size& _size, const Rect& _rect) {
		this->_pos  = _pos;
		this->_size = _size;
		this->_rect = _rect;
	}
};

struct ShotInfo {
	Position2 _pos;
	Vector2 _vel;
	Size _size;
	Rect _rect;
	
	int debugColor;		/// debug�p(��ŏ���)

	ShotInfo() : _pos(0, 0), _vel(0,0),_size(0, 0), _rect(_pos, _size) {};
	ShotInfo(const Position2& _pos, const Vector2& _vel, const Size& _size, const Rect& _rect)
	{
		this->_pos  = _pos;
		this->_vel = _vel;
		this->_size = _size;
		this->_rect = _rect;
	}
};

using shot_vector = std::vector<ShotInfo>;
class Enemy
{
private:
	std::shared_ptr<Camera>& _camera;

protected:
	EnemyInfo enemy;
	shot_vector shot;
	Vector2 _vel;
	int color;
	bool _turnFlag;			// true:�E����, false:������		

	Enemy(std::shared_ptr<Camera>& camera);

	const float maxSpeed = 3.0f;

public:
	~Enemy();
	virtual void Draw();
	virtual void Update();
	virtual EnemyInfo GetInfo();
	virtual shot_vector GetShotInfo();
	virtual void ShotDelete(const int& num);					// ���Ă������������폜���邽�߂̂���
	virtual void CalEscapeDir(const Vector2& vec);
	virtual void CalTrackVel(const Vector2& pos, bool col);		// �ǔ����鑬�x�̌v�Z�p
};

