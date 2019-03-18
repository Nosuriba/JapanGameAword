#pragma once
#include "Scene.h"
#include <memory>

class Input;

class TitleScene : public Scene
{
private:
	int titleimg;

	void (TitleScene::*_updater)(const Input& p);
	void Wait(const Input& p);

public:
	TitleScene();
	~TitleScene();
	void Update(const Input& p);
	void Draw();
};

