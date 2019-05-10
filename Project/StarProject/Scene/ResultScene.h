#pragma once
#include "Scene.h"

class Input;

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Input &p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input &p);
	void Run(const Input &p);

	int flame;

public:
	ResultScene();
	~ResultScene();

	void Update(const Input &p);
	void Draw();
};

