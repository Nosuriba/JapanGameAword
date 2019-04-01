#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

class Input;
class Object;

class GameScene : public Scene
{
private:
	int gameimg;

	std::shared_ptr<Object> _player;
	std::vector<std::shared_ptr<Object>> _enemy;

	void (GameScene::*_updater)(const Input &p);
	void Wait(const Input &p);

public:
	GameScene();
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

