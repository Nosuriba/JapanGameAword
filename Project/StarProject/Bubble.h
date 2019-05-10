#pragma once
#include <DxLib.h>
#include <math.h>
#include <random>
#include <vector>
#include <thread>

#ifdef _DEBUG
#define BubbleCreate void tmp
#define BubbleDraw void tmp
#else
#define BubbleCreate Bubble::GetInstance().Create
#define BubbleDraw Bubble::GetInstance().Draw
#endif

constexpr int ElementNum = 10000;

constexpr int SCREEN_SIZE_X = 1280;
constexpr int SCREEN_SIZE_Y = 740;

struct Element
{
public:
	int x = SCREEN_SIZE_X / 2;
	int y = SCREEN_SIZE_Y / 2;
	int vx;
	int vy;
	int light = 0;
	int rad = 1;
};


class Bubble
{
private:
	Element p_el[ElementNum];
	std::thread thread;

	int cnt,p;
	float x, y;

	int timer = 0;

	static int PCnt;

	void Move();


	Bubble(int _x=0, int _y=0);
	~Bubble();
	struct Bubble_deleter	// custom_deleter
	{
		void operator()(Bubble* ptr)const {
			delete ptr;
		}
	};
	static std::unique_ptr<Bubble, Bubble_deleter> particle;
public:
	static Bubble & GetInstance() {
		return *particle;
	}

	void Create();
	void Draw();
};

