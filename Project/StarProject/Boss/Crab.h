#pragma once
#include "Boss.h"

class Particle;

// UŒ‚‚Ìí—Ş
enum class AtkType
{
	NORMAL,	// ’Êí
	SHOT,	// ¼®¯ÄUŒ‚
	MOVE,	// ˜r‚ÌˆÚ“®
	PITCH,	// ‹²‚ŞUŒ‚
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
	// ŠÖß‚Ì’†ŠÔ“_
	std::vector<Vector2> _points;
	// §Œä“_
	Vector2 _ctlPoint;
	std::vector<Vector2> _center;
	Vector2 _vel;
	std::array<sqr_vert, 2> _vert;		// ŠÖß‚Ì’¸“_
};

struct CrabInfo
{
	Position2 _pos;
	Size _size;

	sqr_vert _vert;					// ŠI–{‘Ì‚Ì’¸“_
	std::vector<JointInfo> _legs;	// ‘«‚ÌŠÖß
	std::vector<JointInfo> _arms;	// ˜r‚ÌŠÖß

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

	void CalVert();		// ‹éŒ`‚Ì’¸“_ŒvZ
	void scisRota();
	void Rotation();
	void MoveLeg();		// §Œä“_‚ÌˆÚ“®
	void MoveJoint();	// ŠÖß‚ÌˆÚ“®
	void ShotDelete();	// ¼®¯Ä‚Ìíœ—p	

	bool StopCheck(const Vector2& sPos, const Vector2& ePos, const Vector2& vel);

	void RegistAtkInfo();
	void RegistDamageInfo();
	void ChangeAtkMode();

	// ŠOÏ‚ÌŒvZ
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
	Vector2 _plPos;						// ÌßÚ²Ô°‚ÌÀ•W•Û‘¶—p
	Vector2 _armPrePos;

	CrabInfo _crab;
	std::shared_ptr<Particle> _particle;

	std::vector<sqr_vert> _scissors;	// ‚Í‚³‚İ‚Ì’Ü‚Ì”
	std::vector<Vector2> _scisCenter;
	std::vector<Vector2> _legMovePos;
	std::vector<Vector2> _legPrePos;
	std::vector<Vector2> _legAccel;		// ‹r‚Ì‰Á‘¬“x—p
	Vector2 center;

	int atkCnt;			// UŒ‚‚·‚é‚Ü‚Å‚ÌŠÔŠu
	int pitchCnt;		// ‰ñ“]‚·‚éŠÔŠu
	int shotCnt;
	int inviCnt;		// –³“GŠÔ
	int _lifeCnt;

	bool _isAlive;		// 

	float length;
	float aLength;
	Size lSize;			// ‹r‚Ì‘å‚«‚³
	Size scisSize;		// ‚Í‚³‚İ‚Ì‘å‚«‚³				
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