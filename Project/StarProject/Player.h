#pragma once
#include <vector>
#include <array>
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

struct Leg {
	Vector2 tip;		// ���̐�[���W
	Vector2 pos;		// ���̈ʒu
	Vector2 vel;		// ���x
	const int T = 10;	// ���Ԓn�_�̐��i�������j
	std::vector<Vector2> halfway_point;	// ���Ԓn�_
	LEG_STATE state;	// ���̏��
};

struct Star {
	int level;				// ���x��
	float r;				// ���̒���
	Vector2 center;			// ���S
	std::vector<Leg> legs;	// ��
};

class Player
{
private:
	const std::shared_ptr<Camera>& _camera;
	std::vector<std::shared_ptr<Particle>> _particle;

	Star _star;
	std::array<int, 2> select_idx;

	Vector2 _vel;

	char Buf[256];

	void (Player::*_updater)(const Input& in);
	void Normal(const Input& in);
	void Move(const Input& in);
	void Die(const Input& in);

public:
	Player(const std::shared_ptr<Camera>& c);
	~Player();

	void Update(const Input& in);
	void Draw();
	void ShadowDraw();

	Star GetInfo();
	const std::vector<Vector2> GetShot();
	void LevelUP();
};

