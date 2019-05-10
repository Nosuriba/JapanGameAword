#pragma once
#include <vector>
#include <memory>
#include "Processing/Geometry.h"

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
	float r;				// ���̒���
	Vector2 center;			// ���S
	std::vector<Leg> legs;	// ��
};

class Player
{
private:
	const std::shared_ptr<Camera>& _camera;

	Star _star;

	Vector2 _vel;

	char Buf[256];

public:
	Player(const std::shared_ptr<Camera>& c);
	~Player();

	void Update(const Input& in);
	void Draw();
	Star GetInfo();
	const std::vector<Vector2> GetShot();

};

