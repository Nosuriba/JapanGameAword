#pragma once
#include "Scene.h"
#include <memory>

#include <vector>
#include <DxLib.h>

class Input;
class Player;
class Enemy;
class Collision;
class Camera;

class Obstacle;
class DestroyableObject;
class PredatoryObject;
class ImmortalObject;

class GameScene : public Scene
{
private:
	void (GameScene::*_updater)(const Input &p);
	void Wait(const Input &p);
	void Run(const Input& p);

	std::shared_ptr<Player> _pl;
	std::shared_ptr<Enemy> _ene;
	std::shared_ptr<Collision> _col;
	std::shared_ptr<Camera> _camera;

	std::vector<std::shared_ptr<Enemy>> _enemies;		/// ‰¼‚Ì•`‰æ

	std::shared_ptr<DestroyableObject> _destroy;
	std::shared_ptr<PredatoryObject> _predatory;
	std::shared_ptr<ImmortalObject> _immortal;

	std::vector<std::shared_ptr<DestroyableObject>> _destroyObj;
	std::vector<std::shared_ptr<PredatoryObject>> _predatoryObj;
	std::vector<std::shared_ptr<ImmortalObject>> _immortalObj;

	int flame;
	int time;
	int totaltime;

public:
	GameScene();
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

