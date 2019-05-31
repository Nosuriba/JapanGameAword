#pragma once
#include "../Processing/Geometry.h"
#include <memory>
#include <vector>
#include <array>
#include <list>

class Camera;
class Player;

#define POS		_enemy._pos
#define SIZE	_enemy._size
#define DAMAGE	_enemy._isDamage
#define ALIVE	_enemy._isAlive
#define CC		_camera->CameraCorrection()
#define SS(x) 	_camera->GetShadowPos(x)

struct EnemyInfo {
	Vector2	_pos;		// 敵の中心点
	Size	_size;
	bool	_isDamage;
	bool	_isAlive;

	EnemyInfo() :_pos(0, 0), _size(0, 0) { _isDamage = false; _isAlive = true; }
	EnemyInfo(const Position2& p, const Size& s) {
		_pos		= p;
		_size		= s;
		_isDamage	= false;
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
	bool		_isTurn;			// true:右方向, false:左方向		
	int			_anim_frame;
	int damageSE;

	std::list<DamageRec> _damage;
	std::list<DamageRec> _attack;

	Enemy(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p);

public:
	~Enemy();
	virtual void Update()	= 0;
	virtual void Draw()		= 0;
	virtual void Shadow()	= 0;

	const EnemyInfo GetInfo() const { return _enemy; }
	const std::list<DamageRec> GetDamaRec() const { return _damage; };
	const std::list<DamageRec> GetAttaRec() const { return _attack; };

	virtual void OnDamage() = 0;
	// ダメージ判定用
	virtual void CreateDamagePoints() = 0;
	// 攻撃判定用
	virtual void CreateAttackPoints() = 0;
};

