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

using sqr_vert = std::array<Vector2, 4>;

struct JointInfo
{
	// �֐߂̒��ԓ_
	std::vector<Vector2> _points;
	// ����_
	Vector2 _ctlPoint;
	std::vector<Vector2> _center;
	Vector2 _vel;
	std::array<sqr_vert, 2> _vert;		// �֐߂̒��_
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// �I�{�̂̒��_
	std::vector<JointInfo> _legs;	// ���̊֐�
	std::vector<JointInfo> _arms;	// �r�̊֐�

	CrabInfo() : _pos(0, 0), _size(0, 0) {};
	CrabInfo(const Position2& p, const Size& s)
	{
		_pos = p;
		_size = s;
	}
};

//struct ShotInfo
//{
//	Position2 _pos;
//	Vector2 _vel;
//	Size _size;
//	ShotInfo() : _pos(0, 0), _vel(0, 0), _size(0, 0) {};
//	ShotInfo(const Position2& p, const Vector2& v, const Size& s)
//	{
//		_pos = p;
//		_vel = v;
//		_size = s;
//	}
//};

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

	void CalVert();		// ��`�̒��_�v�Z
	void scisRota();	
	void Rotation();	
	void MoveLeg();		// ����_�̈ړ�
	void MoveJoint();	// �֐߂̈ړ�
	void ShotDelete();	// ���Ă̍폜�p	

	bool StopCheck(const Vector2& sPos, const Vector2& ePos, const Vector2& vel);

	void ChangeAtkMode();

	// �O�ς̌v�Z
	Vector3 Cross(const Vector3& va, const Vector3& vb)
	{
		return Vector3(va.y * vb.z - va.z * vb.y,
					   va.z * vb.x - va.x - vb.z,
					   va.x * vb.y - va.y * va.x);
	}

	void (Crab::*_updater)();

	AtkType _type;
	Vector2 _plPos;						// ��ڲ԰�̍��W�ۑ��p
	Vector2 _armPrePos;			

	CrabInfo _crab;
	//std::vector<ShotInfo> _shot;		// ���̼��ėp�ϐ�(CrabInfo�Ɏ����Ă����\��)

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
	Crab(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p);
	~Crab();
	
	void Draw();
	void SelectDraw(const Vector2& pos, const float& scale);
	void DebugDraw(const Vector2& camera);
	void OnDamage();
	void Update();
	
};