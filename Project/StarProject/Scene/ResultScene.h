#pragma once
#include "Scene.h"

class Input;

enum class R_Data
{
	enemy,
	bite,
	breakobj,
	time,
	blank,
	total,
	max
};

class ResultScene : public Scene
{
private:
	void (ResultScene::*_updater)(const Input &p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input &p);
	void Run(const Input &p);

	bool isEnd = false;
	int StampCnt;
	int flame;
	int imgbuff;
	int ResultCnt;

	int BGM;
	int SE;

	int ResultData[2][(int)R_Data::max];
	std::vector<std::string>ResultStr;

public:
	ResultScene(const int& enemy, const int& bite, const int & breakobj, const int& time);
	~ResultScene();

	void Update(const Input &p);
	void Draw();
};

