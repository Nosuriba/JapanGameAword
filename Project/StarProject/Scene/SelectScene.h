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

	VERTEX2DSHADER bubble_vertex[4];
	int firstscreen;
	int secondscreen;

	int shader_time;
public:
	SelectScene();
	~SelectScene();
	void Draw();
	void Update(const Input& p);
};

