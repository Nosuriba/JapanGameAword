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

struct Leg 
{
	Vector2 tip;		// 足の先端座標
	Vector2 pos;		// 足の位置
	Vector2 vel;		// 速度
	const int T = 10;	// 中間地点の数（分割数）
	std::vector<Vector2> halfway_point;	// 中間地点
	LEG_STATE state;	// 足の状態
};

struct Star 
{
	int level;				// レベル
	float r;				// 足の長さ
	Vector2 center;			// 中心
	std::vector<Leg> legs;	// 足
};

struct Laser 
{
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

	Star	_star;
	int		_life;
	int		_interval;
	bool	_isAlive;
	bool	_isDie;

	std::array<int, 2>				select_idx;
	std::array<std::list<Laser>,2>	_laser;
	std::list<DamageRec>			_damage;

	int _anim_frame;

	Vector2 _target;	// 捕食対象
	Vector2 _goal;		// 移動目標地点

	int _img_STICK;		// スティックの画像
	int _img_TRIGGER;	// トリガーの画像

	int _eatCnt;

	void (Player::*_updater)(const Input& in);
	void Normal(const Input& in);
	void Predation(const Input& in);
	void Die(const Input& in);
	void Move(const Input& in);

	// ベジェ曲線の座標
	void CreateBezier();
	// 当たり判定用
	void CreateHitPoints();

public:
	Player(const std::shared_ptr<Camera>& c, const Vector2& p = Vector2(200, 200));
	~Player();

	// 更新
	void Update(const Input& in);
	// 描画
	void Draw();
	// 影の描画
	void ShadowDraw();
	// セレクト用
	void SelectDraw(const Vector2 p, const float s);

	// ステータスを返す
	Star GetInfo() const									{ return _star; }
	// 水鉄砲のステータスを返す
	const std::array<std::list<Laser>, 2> GetLaser() const	{ return _laser; }
	// 当たり矩形を返す
	const std::list<DamageRec> GetDamageRec() const			{ return _damage; }

	// レベルアップ
	void LevelUP();
	// 捕食
	void ToCatch(const Vector2& t);

	// ダメージ
	void OnDamage();

	// 指定座標に移動
	void LetsGo(const Vector2 p);
	// 位置を設定
	void SetStar(const Vector2& p, const float& s);

	// 死亡確認
	const bool CheckDie() const { return _isDie; }
	// 捕食回数
	const int GetEatCnt() const { return _eatCnt; }
};

