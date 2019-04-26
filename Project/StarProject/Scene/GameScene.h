#pragma once
#include "Scene.h"
#include <memory>

#include <vector>
class Input;
class Player;
class Enemy;
class Collision;

class GameScene : public Scene
{
private:
	int gameimg;

	void (GameScene::*_updater)(const Input &p);
	void Wait(const Input &p);

	std::shared_ptr<Player> _pl;
	std::shared_ptr<Enemy> _ene;
	std::shared_ptr<Collision> _col;

	std::vector<std::shared_ptr<Enemy>> _enemies;		/// ‰¼‚Ì•`‰æ

public:
	GameScene();
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

