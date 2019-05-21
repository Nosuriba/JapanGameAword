#pragma once
#include "Boss.h"

// �U���̎��
enum class atkType
{
	BUBBLE,		// �A�U��
	PITCH,		// ���ލU��
	FIST,		// ����U��
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

	void CalVert(const int& i);							// ���_�̌v�Z
	void Rotation();									// ��]�p(�I�{��)
	void Rotation(const int& i);						// ��]�p(���Ȃ�)
	void MovePoint(const int& p);						// ����_�̈ړ�
	void MoveLeg(const Vector2& pos, const int& l);		// ���̈ړ�

	// �O�ς̌v�Z
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
