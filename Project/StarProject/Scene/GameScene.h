#pragma once
#include "Scene.h"
#include <memory>

class Input;
class Object;

class GameScene : public Scene
{
private:
	int gameimg;

	std::shared_ptr<Object> _obj;

	void (GameScene::*_updater)(const Input &p);
	void Wait(const Input &p);

public:
	GameScene();
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

