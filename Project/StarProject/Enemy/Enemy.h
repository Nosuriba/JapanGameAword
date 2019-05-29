#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <vector>
#include <array>

class Camera;
class Player;

#define POS		_enemy._pos
#define SIZE	_enemy._size
#define ALIVE	_enemy._isAlive
#define CC		_camera->CameraCorrection()

struct EnemyInfo {
	Vector2	_pos;		// “G‚Ì’†S“_
	Size	_size;
	bool	_isAlive;

	EnemyInfo() :_pos(0, 0), _size(0, 0) { _isAlive = true; }
	EnemyInfo(const Position2& p, const Size& s) {
		_pos		= p;
		_size		= s;
		_isAlive	= true;
	}
};


class Enemy
{
private:

protected:
	const std::shared_ptr<Camera>& _camera;
	const std::shared_ptr<Player>& _player;

	EnemyInfo	_enemy;
	Vector2		_vel;
	bool		_isTurn;			// true:‰E•ûŒü, false:¶•ûŒü		
	int			_anim_frame;

	Enemy(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p);

public:
	~Enemy();
	virtual void Update() = 0;
	virtual void Draw() = 0;

	EnemyInfo GetInfo();

	virtual void OnDamage() = 0;
};

