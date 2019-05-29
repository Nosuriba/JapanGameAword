#pragma once
#include <memory>
#include <array>
#include <vector>
#include <list>
#include "../Processing/Geometry.h"
#include "../Particle/Water.h"


class Camera;
class Player;

using sqr_vert = std::array<Vector2, 4>;

struct JointInfo
{
	// �֐߂̒��ԓ_
	std::vector<Vector2> _points;
	// ����_
	Vector2 _ctlPoint;
	std::vector<Vector2> _center;
	Vector2 _vel;
	Size   _size;
	std::array<sqr_vert, 2> _vert;		// �֐߂̒��_
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// �I�{�̂̒��_
	std::vector<JointInfo> _legs;	// ���̊֐�
	std::vector<JointInfo> _arms;	// �r�̊֐�

	CrabInfo() : _pos(0, 0), _size(0, 0) {};
	CrabInfo(const Position2& _pos, const Size& _size)
	{
		this->_pos = _pos;
		this->_size = _size;
	}
};


struct AttackInfo
{
	Vector2 pos;
	float r;
};

class Boss
{
protected:
	std::list<AttackInfo> at;

	const std::shared_ptr<Camera>& _camera;
	const std::shared_ptr<Player>& _player;

	Boss(const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Player>& _player);

public:
	~Boss();
	virtual void Draw() = 0;
	virtual void Update() = 0;

	std::list<AttackInfo> GetAttackInfo();

	virtual void OnDamage() = 0;
};