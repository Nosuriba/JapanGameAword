#pragma once
#include "Boss.h"

// �U���̎��
enum class AtkType
{
	NORMAL,	// �ʏ�
	SHOT,	// ���čU��
	MOVE,	// �r�̈ړ�
	PITCH,	// ���ލU��
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

	void CalVert();		// ��`�̒��_�v�Z
	void scisRota();	
	void Rotation();	
	void MoveLeg();		// ����_�̈ړ�
	void MoveJoint();	// �֐߂̈ړ�
	void ShotDelete();	// ���Ă̍폜�p	

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
	Vector2 _armPrePos;			

	std::vector<ShotDebug> _shot;		// ���̼��ėp�ϐ�(CrabInfo�Ɏ����Ă����\��)

	std::vector<sqr_vert> _scissors;	// �͂��݂̒܂̐�
	std::vector<Vector2> _scisCenter;	
	std::vector<Vector2> _legMovePos;	
	std::vector<Vector2> _legPrePos;	
	std::vector<Vector2> _legAccel;		// �r�̉����x�p

	int atkCnt;			// �U������܂ł̊Ԋu
	int pitchCnt;		// ��]����Ԋu
	int shotCnt;
	int inviCnt;		// ���G����

	float length;
	float aLength;
	Size lSize;			// �r�̑傫��
	Size scisSize;		// �͂��݂̑傫��				
public:
	Crab(std::shared_ptr<Camera>& camera);
	~Crab();
	BossInfo GetInfo() { return boss; };
	void CalTrackVel(const Vector2& pos);
	void Draw();
	void SelectDraw(const Vector2& pos, const Size& size);
	void DebugDraw(const Vector2& camera);
	void Update();
	
};