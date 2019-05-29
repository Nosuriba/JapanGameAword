#pragma once
#include "Enemy.h"

// 制御点用
struct CtlInfo
{
	Vector2 _pos;
	bool	_isTurn;

	CtlInfo() : _pos(0, 0), _isTurn(false) {};
	CtlInfo(const Vector2& p, bool f)
	{
		this->_pos = p;
		this->_isTurn = f;
	}
};

class Fish :
	public Enemy
{
private:
	// 泳ぐ
	void SwimUpdate();
	// 旋回
	void TurnUpdate();
	// 追跡
	void TrackUpdate();
	// 逃げる
	void EscapeUpdate();
	// 死亡
	void DieUpdate();

	void (Fish::*_updater)();

	std::vector<Vector2> midPoints;			// ﾍﾞｼﾞｪ曲線の中間点
	std::vector<CtlInfo> cPoints;			// ﾍﾞｼﾞｪ曲線の制御点

	Vector2 _target;		// 追跡対象
	int		_escapeTime;	// 逃走経過時間

	Vector2 _axis;	// 旋回の軸
	float	_angle;	// 1フレームの旋回角度

	void Search();		// 索敵
	void CalBezier();	// ﾍﾞｼﾞｪ曲線の計算用(3次ﾍﾞｼﾞｪ)
	void LookAt(const Vector2& v);

public:
	Fish(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos);
	~Fish();

	void Update();

	void Draw();
	void DebugDraw();

	void OnDamage();

	//////////// いらない子 //////////////

	//////////////////////////////////////
};

