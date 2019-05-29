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
	Vector2 tip;		// 足の先端座標
	Vector2 pos;		// 足の位置
	Vector2 vel;		// 速度
	const int T = 10;	// 中間地点の数（分割数）
	std::vector<Vector2> halfway_point;	// 中間地点
	LEG_STATE state;	// 足の状態
};

struct Star {
	int level;				// レベル
	float r;				// 足の長さ
	Vector2 center;			// 中心
	std::vector<Leg> legs;	// 足
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

