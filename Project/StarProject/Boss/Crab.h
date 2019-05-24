#pragma once
#include "Boss.h"

// 攻撃の種類
enum class AtkType
{
	NORMAL,	// 通常
	BUBBLE,	// 泡攻撃
	PITCH,	// 挟む攻撃
	FIST,	// 殴る攻撃
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

class Crab :
	public Boss
{
private:
	/// お前らも修正じゃ!!!
	void BodyInit();
	void LegInit();
	void ArmInit();

	void Neutral();
	void Fist();
	void Shot();
	void Die();

	void NeutralUpdate();
	void FistUpdate();
	void ShotUpdate();
	void DieUpdate();

	/// こいつら全部修正をかけるぞ。
	void CalVert();		// 矩形の頂点計算
	void Rotation();	
	void MoveLeg();		// 制御点の移動
	void MoveJoint();	// 関節の移動

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
	Vector2 _armPrePos;					// 腕の移動前の制御点座標

	std::vector<sqr_vert> _scissors;	// はさみの爪の数(描画用)
	std::vector<Vector2> _scisCenter;	// はさみの中心点(当たり判定で使うかも)
	std::vector<Vector2> _legMovePos;	// 脚の移動先の制御点座標
	std::vector<Vector2> _legPrePos;	// 脚の移動前の制御点座標

	int atkInvCnt;			// 攻撃するまでの間隔

public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	BossInfo GetInfo();
	void CalTrackVel(const Vector2& pos);
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	
};