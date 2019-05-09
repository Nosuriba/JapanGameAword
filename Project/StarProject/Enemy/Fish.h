#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>

// 三角関数用の構造体
//struct Trigono
//{
//	float _sin;
//	float _cos;
//	float _tan;
//
//	Trigono() : _sin(0), _cos(0), _tan(0) {};
//	Trigono(const float& _sin, const float& _cos, const float& _tan)
//	{
//		this->_cos = _cos;
//		this->_sin = _sin;
//		this->_tan = _tan;
//	};
//};

class Player;
class Camera;

class Fish :
	public Enemy
{
private:
	void Swim();
	void Die();

	void SwimUpdate();
	void DieUpdate();

	void (Fish::*updater)();

	std::shared_ptr<Camera>& _camera;
	std::vector<Vector2> searchPos;			/// ﾌﾟﾚｲﾔｰを探知する座標
public:
	Fish(std::shared_ptr<Camera>& camera);
	~Fish();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

