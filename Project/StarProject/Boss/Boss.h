#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <array>
#include <vector>

class Camera;

using sqr_vert = std::array<Vector2, 4>;

struct JointInfo
{
	// �֐߂̒��ԓ_
	std::vector<Vector2> _points;			/// ��Œ��ԓ_�̕��������̒��ɂԂ�����ł�邺
	// ����_
	Vector2 _ctlPoint;
	Vector2 _center;
	Vector2 _vel;
	Size   _size;
	std::array<sqr_vert, 2> _vert;		// ���̊֐߂̒��_
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// �I�{�̂̒��_
	std::vector<JointInfo> _legs;	// ���̊֐�
	std::vector<JointInfo> _arms;	// �r�̊֐�

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
	virtual void CalTrackVel(const Vector2& pos);
};