#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <array>
#include <vector>

class Camera;

using sqr_vert = std::array<Vector2, 4>;

struct LegInfo
{
	// 3つの関節と制御点
	Vector2 _sPoint, _mPoint, _ePoint, _ctlPoint;
	Vector2 _vel;
	Size _size;
	float cos, sin;						// 三角比用変数
	float _liftVel;
	std::array<sqr_vert, 2> legVert;	// 足の関節の頂点を設定するもの

};

struct CrabInfo
{
	Position2 _pos;
	Position2 _prePos;
	Size _size;

	sqr_vert _vert;					// 蟹本体の頂点
	std::vector<LegInfo> legs;		// 足の関節

	CrabInfo() : _pos(0, 0), _size(0, 0){};
	CrabInfo(const Position2& _pos, const Size& _size)
	{
		this->_pos = _pos;
		this->_size = _size;
	}
};

struct BossInfo
{
	CrabInfo _crab;
};

class Boss
{
private:

	std::shared_ptr<Camera>& _camera;
protected:
	BossInfo boss;

	Boss(std::shared_ptr<Camera>& camera);

public:
	~Boss();
	virtual void Draw();
	virtual void Update();
	virtual BossInfo GetInfo();

};