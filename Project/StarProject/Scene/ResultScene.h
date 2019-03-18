#pragma once
#include "Scene.h"

class Input;

class ResultScene : public Scene
{
private:
	int resultimg;
	void (ResultScene::*_updater)(const Input &p);
	void Wait(const Input &p);

public:
	ResultScene();
	~ResultScene();

	void Update(const Input &p);
	void Draw();
};

