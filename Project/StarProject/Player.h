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
	Vector2 tip;		// 足の先端座標
	Vector2 pos;		// 足の位置
	Vector2 vel;		// 速度
	const int T = 10;	// 中間地点の数（分割数）
	std::vector<Vector2> halfway_point;	// 中間地点
	LEG_STATE state;	// 足の状態
};

struct Star {
	float r;				// 足の長さ
	Vector2 center;			// 中心
	std::vector<Leg> legs;	// 足
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

