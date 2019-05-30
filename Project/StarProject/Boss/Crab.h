#pragma once
#include "Boss.h"

class Particle;

// �U���̎��
enum class AtkType
{
	NORMAL,	// �ʏ�
	SHOT,	// ���čU��
	MOVE,	// �r�̈ړ�
	PITCH,	// ���ލU��
	MAX
};

struct CrabSE
{
	int damage;
	int die;
	int pitch;
	int shot;
	int walk;
	int swing;
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

	void RegistAtkInfo();
	void RegistDamageInfo();
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
	CrabSE SE;
	int BGM;
	Vector2 _plPos;						// ��ڲ԰�̍��W�ۑ��p
	Vector2 _armPrePos;

	CrabInfo _crab;
	std::shared_ptr<Particle> _particle;

	std::vector<sqr_vert> _scissors;	// �͂��݂̒܂̐�
	std::vector<Vector2> _scisCenter;
	std::vector<Vector2> _legMovePos;
	std::vector<Vector2> _legPrePos;
	std::vector<Vector2> _legAccel;		// �r�̉����x�p
	Vector2 center;

	int atkCnt;			// �U������܂ł̊Ԋu
	int pitchCnt;		// ��]����Ԋu
	int shotCnt;
	int inviCnt;		// ���G����
	int _lifeCnt;

	bool _isAlive;		// 

	float length;
	float aLength;
	Size lSize;			// �r�̑傫��
	Size scisSize;		// �͂��݂̑傫��				
public:
	Crab(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos = Vector2(0,0));
	~Crab();

	void Draw();
	void ShadowDraw();
	void SelectDraw(const Vector2& pos, const float& scale);
	void DebugDraw(const Vector2& camera);
	void OnDamage();
	void HitBlock();
	void Update();

};