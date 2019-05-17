#pragma once
#include <vector>
#include "Boss.h"

// UŒ‚‚Ìí—Ş
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

struct Joint
{
	// 3‚Â‚ÌŠÖß
	Vector2 sPoint, mPoint, ePoint;
	// OŠp”ä—p•Ï”
	float cos, sin;

	Size _size;
	Rect _rect;

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

	void CalRect();
	void LegMove(const Vector2& pos, const int& i);

	// ŠOÏ‚ÌŒvZ
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
			va.z * vb.x - va.x - vb.z,
			va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Joint> joints;		// ŠÖß
	std::vector<Vector2> ctlPoints;
	Vector2 _vel;
	int moveCnt;
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void DebugDraw();
	void Update();
	BossInfo GetInfo();
};
