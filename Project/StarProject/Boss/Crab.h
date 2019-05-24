#pragma once
#include "Boss.h"

// �U���̎��
enum class AtkType
{
	NORMAL,	// �ʏ�
	BUBBLE,	// �A�U��
	PITCH,	// ���ލU��
	FIST,	// ����U��
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
	/// ���O����C������!!!
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

	/// ������S���C���������邼�B
	void CalVert();		// ��`�̒��_�v�Z
	void Rotation();	
	void MoveLeg();		// ����_�̈ړ�
	void MoveJoint();	// �֐߂̈ړ�

	bool StopCheck(const Vector2& sPos, const Vector2& ePos, const Vector2& vel);

	// �O�ς̌v�Z
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
					   va.z * vb.x - va.x - vb.z,
					   va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	std::shared_ptr<Camera>& _camera;
	AtkType _type;
	Vector2 _plPos;						// ��ڲ԰�̍��W�ۑ��p
	Vector2 _armPrePos;					// �r�̈ړ��O�̐���_���W

	std::vector<sqr_vert> _scissors;	// �͂��݂̒܂̐�(�`��p)
	std::vector<Vector2> _scisCenter;	// �͂��݂̒��S�_(�����蔻��Ŏg������)
	std::vector<Vector2> _legMovePos;	// �r�̈ړ���̐���_���W
	std::vector<Vector2> _legPrePos;	// �r�̈ړ��O�̐���_���W

	int atkInvCnt;			// �U������܂ł̊Ԋu

public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	BossInfo GetInfo();
	void CalTrackVel(const Vector2& pos);
	void Draw();
	void DebugDraw(const Vector2& camera);
	void Update();
	
};