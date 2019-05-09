#pragma once
#include "Scene.h"
#include <memory>

class Input;

class TitleScene : public Scene
{
private:
	int titleimg;

	void (TitleScene::*_updater)(const Input& p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input& p);
	void Run(const Input& p);

	int flame;
public:
	TitleScene();
	~TitleScene();
	void Update(const Input& p);
	void Draw();
};

