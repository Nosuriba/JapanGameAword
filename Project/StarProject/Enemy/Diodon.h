#pragma once
#include "Enemy.h"
#include <memory>

class Camera;

class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();

	void SwimUpdate();		
	void SwellUpdate();		/// �c��񂾏��

	void (Diodon::*updater)();

	std::shared_ptr<Camera> _camera;

	int riseCnt;			/// ���シ�����ݸނ𒲐����邽�߂̶���
	bool _turnFlag;			/// true:�E����, false:������

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};
