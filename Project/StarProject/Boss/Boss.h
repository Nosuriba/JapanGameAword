#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <array>
#include <vector>

class Camera;

using sqr_vert = std::array<Vector2, 4>;

struct LegInfo
{
	// 3�̊֐߂Ɛ���_
	Vector2 _sPoint, _mPoint, _ePoint, _ctlPoint;
	Vector2 _vel;
	Size _size;
	float cos, sin;						// �O�p��p�ϐ�
	float _liftVel;
	std::array<sqr_vert, 2> legVert;	// ���̊֐߂̒��_��ݒ肷�����

};

struct CrabInfo
{
	Position2 _pos;
	Position2 _prePos;
	Size _size;

	sqr_vert _vert;					// �I�{�̂̒��_
	std::vector<LegInfo> legs;		// ���̊֐�

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