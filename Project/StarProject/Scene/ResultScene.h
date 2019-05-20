#pragma once
#include "Scene.h"
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

	int enemy;
	int bite;
	int breakobj;
	float time;

public:
	ResultScene(const int& enemy, const int& bite, const int & breakobj, const float& time);
	~ResultScene();

	void Update(const Input &p);
	void Draw();
};

