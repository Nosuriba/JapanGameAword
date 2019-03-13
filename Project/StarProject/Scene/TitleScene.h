#pragma once
#include "Scene.h"

class Input;

class TitleScene : public Scene
{
private:
	void (TitleScene::*_updater)(const Input& p);
	void Wait(const Input& p);

public:
	TitleScene();
	~TitleScene();
	void Update(const Input& p);
};

