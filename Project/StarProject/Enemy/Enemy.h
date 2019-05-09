#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <vector>
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

struct ShotInfo {
	Position2 _pos;
	Vector2 _vel;
	Size _size;
	Rect _rect;
	
	int debugColor;		/// debug用(後で消す)

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
	bool _turnFlag;			// true:右方向, false:左方向
	bool _dieFlag;			// true:死亡, false:生存

	Enemy(std::shared_ptr<Camera>& camera);

public:
	~Enemy();
	virtual void Draw();
	virtual void Update();
	virtual EnemyInfo GetInfo();
	virtual shot_vector GetShotInfo();
	virtual void ChangeShotColor(const int& num);
	virtual void ChangeColor();
	virtual void ResetColor();
	virtual void CalTrackVel(const Vector2& pos, bool col);		// 追尾する速度の計算用
};

