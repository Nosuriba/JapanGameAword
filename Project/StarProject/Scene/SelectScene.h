#pragma once
#include "Scene.h"

class SelectScene :
	public Scene
{
private:
	void (SelectScene::*_updater)(const Input& p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input& p);
	void Run(const Input& p);

public:
	SelectScene();
	~SelectScene();
	void Draw();
	void Update(const Input& p);
};

