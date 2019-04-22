#pragma once
#include "Scene.h"

class Input;
class Object;

class GameScene : public Scene
{
private:
	int gameimg;

	void (GameScene::*_updater)(const Input &p);
	void Wait(const Input &p);

public:
	GameScene();
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

