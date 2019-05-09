#pragma once
#include "Enemy.h"

class Camera;

class Diodon :
	public Enemy
{
private:
	void Swim();
	void Swell();

	void SwimUpdate();		
	void SwellUpdate();		/// –c‚ç‚ñ‚¾ó‘Ô

	void (Diodon::*updater)();

	std::shared_ptr<Camera> _camera;

	int riseCnt;			/// •‚ã‚·‚éÀ²Ğİ¸Ş‚ğ’²®‚·‚é‚½‚ß‚Ì¶³İÄ

public:
	Diodon(std::shared_ptr<Camera>& camera);
	~Diodon();
	void Draw();
	void Update();
	EnemyInfo GetInfo();
	void ChangeColor();
	void ResetColor();
};

