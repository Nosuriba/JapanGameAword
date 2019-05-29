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

	std::vector<std::unique_ptr<Particle>> CoralBubble;

	int flame,Cnt,Select;
	int bubble;
	int background;

	std::vector < std::string >selectStr;

	VERTEX2DSHADER bubble_vertex[4];
	int firstscreen;
	int secondscreen;

	int scrollCnt, scrollWait;
	int shader_time;

	int BGM;
	int SE;
public:
	SelectScene();
	~SelectScene();
	void Draw();
	void Update(const Input& p);
};

