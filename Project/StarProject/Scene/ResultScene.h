#pragma once
#include "Scene.h"

class Input;


struct ScoreInfo {
	int enemy;
	int bite;
	int breakobj;
	float time;
	ScoreInfo() :enemy(0), bite(0), breakobj(0), time(0) {};
	ScoreInfo(const int& enemy, const int& bite, const int & breakobj, const float& time) {
		this->enemy = enemy;
		this->bite = bite;
		this->breakobj = breakobj;
		this->time = time;
	}
};

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Input &p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input &p);
	void Run(const Input &p);

	int flame;

	ScoreInfo info;

public:
	ResultScene(const int& enemy, const int& bite, const int & breakobj, const float& time);
	~ResultScene();

	void Update(const Input &p);
	void Draw();
};

