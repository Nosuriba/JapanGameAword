#pragma once
#include "Boss.h"

// UŒ‚‚Ìí—Ş
enum class atkType
{
	BUBBLE,		// –AUŒ‚
	PITCH,		// ‹²‚ŞUŒ‚
	FIST,		// ‰£‚éUŒ‚
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
	void Neutral();
	void Shot();
	void Die();

	void NeutralUpdate();
	void ShotUpdate();
	void DieUpdate();

	void CalVert(const int& i);							// ’¸“_‚ÌŒvZ
	void Rotation();									// ‰ñ“]—p(ŠI–{‘Ì)
	void Rotation(const int& i);						// ‰ñ“]—p(‘«‚È‚Ç)
	void MovePoint(const int& p);						// §Œä“_‚ÌˆÚ“®
	void MoveLeg(const Vector2& pos, const int& l);		// ‘«‚ÌˆÚ“®

	// ŠOÏ‚ÌŒvZ
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
			va.z * vb.x - va.x - vb.z,
			va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Vector2> ctlPoints;
	std::vector<Vector2> _legVel;

	Vector2 _vel;

public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	BossInfo GetInfo();
};
