#pragma once
#include "Boss.h"

class Particle;

// 攻撃の種類
enum class AtkType
{
	NORMAL,	// 通常
	SHOT,	// ｼｮｯﾄ攻撃
	MOVE,	// 腕の移動
	PITCH,	// 挟む攻撃
	MAX
};

using sqr_vert = std::array<Vector2, 4>;

struct JointInfo
{
	// 関節の中間点
	std::vector<Vector2> _points;
	// 制御点
	Vector2 _ctlPoint;
	std::vector<Vector2> _center;
	Vector2 _vel;
	std::array<sqr_vert, 2> _vert;		// 関節の頂点
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// 蟹本体の頂点
	std::vector<JointInfo> _legs;	// 足の関節
	std::vector<JointInfo> _arms;	// 腕の関節

	CrabInfo() : _pos(0, 0), _size(0, 0) {};
	CrabInfo(const Position2& p, const Size& s)
	{
		_pos = p;
		_size = s;
	}
};

struct Vector3
{
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Crab :
	public Boss
{
private:
	void BodyInit();
	void LegInit();
	void ArmInit();

	void Neutral();
	void Pitch();
	void Shot();
	void Die();

	void NeutralUpdate();
	void PitchUpdate();
	void ShotUpdate();
	void DieUpdate();

	void CalVert();		// 矩形の頂点計算
	void scisRota();
	void Rotation();
	void MoveLeg();		// 制御点の移動
	void MoveJoint();	// 関節の移動
	void ShotDelete();	// ｼｮｯﾄの削除用	

	bool StopCheck(const Vector2& sPos, const Vector2& ePos, const Vector2& vel);

	void RegistAtkInfo();
	void RegistDamageInfo();
	void ChangeAtkMode();

	// 外積の計算
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
			va.z * vb.x - va.x - vb.z,
			va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	AtkType _type;
	Vector2 _plPos;						// ﾌﾟﾚｲﾔｰの座標保存用
	Vector2 _armPrePos;

	CrabInfo _crab;
	std::shared_ptr<Particle> _particle;

	std::vector<sqr_vert> _scissors;	// はさみの爪の数
	std::vector<Vector2> _scisCenter;
	std::vector<Vector2> _legMovePos;
	std::vector<Vector2> _legPrePos;
	std::vector<Vector2> _legAccel;		// 脚の加速度用
	Vector2 center;

	int atkCnt;			// 攻撃するまでの間隔
	int pitchCnt;		// 回転する間隔
	int shotCnt;
	int inviCnt;		// 無敵時間
	int _lifeCnt;

	float length;
	float aLength;
	Size lSize;			// 脚の大きさ
	Size scisSize;		// はさみの大きさ	

public:
	Crab(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos = Vector2(0,0));
	~Crab();

	void Draw();
	void ShadowDraw();
	void SelectDraw(const Vector2& pos, const float& scale);
	void DebugDraw(const Vector2& camera);
	void OnDamage();
	void HitBlock();
	void Update();

};