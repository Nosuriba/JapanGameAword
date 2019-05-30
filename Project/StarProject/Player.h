#pragma once
#include <vector>
#include <array>
#include <list>
#include <memory>
#include "Processing/Geometry.h"
#include "Particle/Water.h"

class Camera;
class Input;

enum class LEG_STATE {
	NORMAL,
	SELECT,
	SHOT,
	HOLD
};

struct Leg 
{
	Vector2 tip;		// ���̐�[���W
	Vector2 pos;		// ���̈ʒu
	Vector2 vel;		// ���x
	const int T = 10;	// ���Ԓn�_�̐��i�������j
	std::vector<Vector2> halfway_point;	// ���Ԓn�_
	LEG_STATE state;	// ���̏��
};

struct Star 
{
	int level;				// ���x��
	float r;				// ���̒���
	Vector2 center;			// ���S
	std::vector<Leg> legs;	// ��
};

struct Laser 
{
	int count;
	Vector2 pos;
	Vector2 vel;
	float size;
	bool isHit;
	bool isEnd;

	Laser(Vector2 p, Vector2 v,bool isEnd = false) : pos(p), vel(v) ,isEnd(isEnd){ count = 0; size = 5; isHit = false;}
	void Hit() { isHit = true; }
	void End() { isEnd = true; }
};

class Player
{
private:
	const std::shared_ptr<Camera>& _camera;
	std::vector<std::shared_ptr<Particle>> _particle;

	Star	_star;
	int		_life;
	int		_interval;
	bool	_isAlive;
	bool	_isDie;

	std::array<int, 2>				select_idx;
	std::array<std::list<Laser>,2>	_laser;
	std::list<DamageRec>			_damage;

	int _anim_frame;

	Vector2 _target;	// �ߐH�Ώ�
	Vector2 _goal;		// �ړ��ڕW�n�_

	int _img_STICK;		// �X�e�B�b�N�̉摜
	int _img_TRIGGER;	// �g���K�[�̉摜

	int _eatCnt;

	void (Player::*_updater)(const Input& in);
	void Normal(const Input& in);
	void Predation(const Input& in);
	void Die(const Input& in);
	void Move(const Input& in);

	// �x�W�F�Ȑ��̍��W
	void CreateBezier();
	// �����蔻��p
	void CreateHitPoints();

public:
	Player(const std::shared_ptr<Camera>& c, const Vector2& p = Vector2(200, 200));
	~Player();

	// �X�V
	void Update(const Input& in);
	// �`��
	void Draw();
	// �e�̕`��
	void ShadowDraw();
	// �Z���N�g�p
	void SelectDraw(const Vector2 p, const float s);

	// �X�e�[�^�X��Ԃ�
	Star GetInfo() const									{ return _star; }
	// ���S�C�̃X�e�[�^�X��Ԃ�
	const std::array<std::list<Laser>, 2> GetLaser() const	{ return _laser; }
	// �������`��Ԃ�
	const std::list<DamageRec> GetDamageRec() const			{ return _damage; }

	// ���x���A�b�v
	void LevelUP();
	// �ߐH
	void ToCatch(const Vector2& t);

	// �_���[�W
	void OnDamage();

	// �w����W�Ɉړ�
	void LetsGo(const Vector2 p);
	// �ʒu��ݒ�
	void SetStar(const Vector2& p, const float& s);

	// ���S�m�F
	const bool CheckDie() const { return _isDie; }
	// �ߐH��
	const int GetEatCnt() const { return _eatCnt; }
};

