#pragma once
#include <vector>
#include <array>
#include <list>
#include <memory>
#include "Processing/Geometry.h"
#include "Particle/Water.h"

class Camera;
class Input;

enum class LEG_STATE {
	NORMAL,
	SELECT,
	SHOT,
	HOLD
};

struct Leg {
	Vector2 tip;		// ���̐�[���W
	Vector2 pos;		// ���̈ʒu
	Vector2 vel;		// ���x
	const int T = 10;	// ���Ԓn�_�̐��i�������j
	std::vector<Vector2> halfway_point;	// ���Ԓn�_
	LEG_STATE state;	// ���̏��
};

struct Star {
	int level;				// ���x��
	float r;				// ���̒���
	Vector2 center;			// ���S
	std::vector<Leg> legs;	// ��
};

struct Laser {
	int count;
	Vector2 pos;
	Vector2 vel;
	float size;
	bool isHit;
	bool isEnd;

	Laser(Vector2 p, Vector2 v,bool isEnd = false) : pos(p), vel(v) ,isEnd(isEnd){ count = 0; size = 5; isHit = false;}
	void Hit() { isHit = true; }
	void End() { isEnd = true; }
};

class Player
{
private:
	const std::shared_ptr<Camera>& _camera;
	std::vector<std::shared_ptr<Particle>> _particle;

	Star _star;
	int	_life;
	int	_interval;
	bool _isAlive;
	bool _isDie;

	std::array<int, 2> select_idx;
	std::array<std::list<Laser>,2> _laser;

	int _anim_frame;

	Vector2 _target;
	Vector2 _goal;

	int _img_STICK;
	int _img_TRIGGER;

	char Buf[256];

	int _eatCnt;

	void (Player::*_updater)(const Input& in);
	void Normal(const Input& in);
	void Predation(const Input& in);
	void Die(const Input& in);
	void Move(const Input& in);

	//void SetStar(const Vector2& p, const float& s);
public:
	Player(const std::shared_ptr<Camera>& c, const Vector2& p = Vector2(200, 200));
	~Player();

	void Update(const Input& in);
	void Draw();
	void ShadowDraw();
	void SelectDraw(const Vector2 p, const float s);

	Star GetInfo();
	const std::vector<Vector2> GetShot();
	const std::array<std::list<Laser>, 2> GetLaser();
	void LevelUP();
	void ToCatch(const Vector2& t);
	void OnDamage();
	void LetsGo(const Vector2 p);
	void SetStar(const Vector2& p, const float& s);
	const bool CheckDie() const { return _isDie; }
};

