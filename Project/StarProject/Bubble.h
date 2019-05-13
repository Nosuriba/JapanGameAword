#pragma once
#include <DxLib.h>
#include <vector>
#include <thread>

/// Debugの時はCreateとDrawを行わない
#ifdef _DEBUG
#define BubbleCreate void nonCreate
#define BubbleDraw void nonDraw
#else
#define BubbleCreate Bubble::GetInstance().Create
#define BubbleDraw Bubble::GetInstance().Draw
#endif

// パーティクルの最大要素数
constexpr int ELEMENT_NUM = 1000;

/// スクリーンサイズ
constexpr int SCREEN_SIZE_X = 1280;
constexpr int SCREEN_SIZE_Y = 740;

struct Element
{
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
	Element p_el[ELEMENT_NUM];
	std::thread thread;

	int cnt,p;
	float x, y;

	void Move();

	Bubble(int _x=0, int _y=0);
	~Bubble();

	struct Bubble_deleter{
		void operator()(Bubble* ptr)const{
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
