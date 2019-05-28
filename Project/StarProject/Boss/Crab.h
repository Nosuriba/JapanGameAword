#pragma once
#include "Boss.h"

// 攻撃の種類
enum class AtkType
{
	NORMAL,	// 通常
	SHOT,	// ｼｮｯﾄ攻撃
	MOVE,	// 腕の移動
	PITCH,	// 挟む攻撃
	MAX
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

struct ShotDebug
{
	Position2 _pos;
	Vector2 _vel;
	ShotDebug() : _pos(0, 0), _vel(0, 0){};
	ShotDebug(const Position2& _pos, const Vector2& _vel)
	{
		this->_pos = _pos;
		this->_vel = _vel;
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

	// 外積の計算
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
					   va.z * vb.x - va.x - vb.z,
					   va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	AtkType _type;
	Vector2 _plPos;						// ﾌﾟﾚｲﾔｰの座標保存用
	Vector2 _armPrePos;			

	std::vector<ShotDebug> _shot;		// 仮のｼｮｯﾄ用変数(CrabInfoに持っていく予定)

	std::vector<sqr_vert> _scissors;	// はさみの爪の数
	std::vector<Vector2> _scisCenter;	
	std::vector<Vector2> _legMovePos;	
	std::vector<Vector2> _legPrePos;	
	std::vector<Vector2> _legAccel;		// 脚の加速度用

	int atkCnt;			// 攻撃するまでの間隔
	int pitchCnt;		// 回転する間隔
	int shotCnt;
	int inviCnt;		// 無敵時間
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	BossInfo GetInfo() { return boss; };
	void CalTrackVel(const Vector2& pos);
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	
};