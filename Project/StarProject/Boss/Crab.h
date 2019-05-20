#pragma once
#include <array>
#include <vector>
#include "Boss.h"

// 攻撃の種類
enum class atkType
{
	BUBBLE_LASER,
	PITCH,
	FIST,
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

using sqr_vert = std::array<Vector2, 4>;

struct LegInfo
{
	// 3つの関節
	Vector2 sPoint, mPoint, ePoint;
	// 三角比用変数
	float cos, sin;

	Size _size;
	std::array<sqr_vert, 2> sqrVert;

};

class Crab :
	public Boss
{
private:
	void Neutral();
	void Shot();
	void Die();

	void NeutralUpdate();
	void ShotUpdate();
	void DieUpdate();

	void CalVert(const int& i);							// 頂点の計算
	void Rotation(const int& i);						// 回転用
	void LegMove(const Vector2& pos, const int& i);		// 足の移動

	// 外積の計算
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
			va.z * vb.x - va.x - vb.z,
			va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<LegInfo> legs;		// 関節
	std::vector<Vector2> ctlPoints;

	std::array<Vector2, 4> debugPos;		// 回転で使う座標(debug用)
	Vector2 _vel;
	Vector2 rotCenter;						// 回転するための中心座標
	int moveCnt;
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void DebugDraw();
	void Update();
	BossInfo GetInfo();
};
