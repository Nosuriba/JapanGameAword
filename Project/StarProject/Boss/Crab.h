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

struct Joint
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

	void CalVert();			// 頂点の計算
	void LegMove(const Vector2& pos, const int& i);

	// 外積の計算
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
			va.z * vb.x - va.x - vb.z,
			va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Joint> joints;		// 関節
	std::vector<Vector2> ctlPoints;
	Vector2 _vel;
	int moveCnt;

	Vector2 dCenter;			/// ﾃﾞﾊﾞｯｸﾞ用の中心点
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void DebugDraw();
	void Update();
	BossInfo GetInfo();
};
