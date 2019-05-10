#include "Bubble.h"
#include <iostream>
#include "../StarProject/Scene/AMP.h"

int Bubble::PCnt = 0;

using namespace concurrency;
std::unique_ptr<Bubble, Bubble::Bubble_deleter> Bubble::particle(new Bubble(SCREEN_SIZE_X/2, SCREEN_SIZE_Y+100));

constexpr int UseGpuNum = 100;
constexpr int Vec = 200;
constexpr int V_Speed = 1;
constexpr int V_Cnt = 255/ V_Speed;

Bubble::Bubble(int _x, int _y)
{
	p = ++PCnt;
	cnt=0;
	x = _x;
	y = _y;
}


Bubble::~Bubble()
{
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Create()
{
	if (!thread.joinable())
	{
		thread = std::thread([=] {for (int num = 0; num < 5; num++)
		{
			std::random_device rd;
			std::mt19937 Rand(rd());
			int i = 0;
			for (i = 0; i < ElementNum; i++)
			{
				if (p_el[i].light <= 0)
				{
					break;
				}
			}
			p_el[i].x = (x) * 100;
			p_el[i].y = (y) * 100;
			p_el[i].light = 255;
			p_el[i].rad = (Rand()%4)+1;

			int Theta = Rand() % 360;
			p_el[i].vx = cos(Theta)*Vec * 4;
			p_el[i].vy = sin(Theta % 180 - 90)*Vec;
			//------------------------------------------------//
		}});
	}
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Move()
{
	array_view<Element>p_element(ElementNum, p_el);
	auto move = [p_element = p_element](index<1> idx)restrict(amp){
		if (p_element[idx].light < 10){
			p_element[idx].light = 0;
			return;
		}
		if ((-255 * 100 >p_element[idx].x)||(p_element[idx].y < -255 * 100)||(((SCREEN_SIZE_X + 255+200) *100)<p_element[idx].x))
		{
			p_element[idx].light = 0;
			return;
		}
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;
		// 加速部分
		//p_element[idx].vy += -12;
		//p_element[idx].vx += (int)(p_element[idx].x/100) % 2 ? 101: -101;
		p_element[idx].light -= V_Speed;
	};
	// GPUで動かしてる
	parallel_for_each(p_element.get_extent(), move);
}

void Bubble::Draw()
{
	// ここで動く
	Move();

	// ここから描画
	bool flag = false;

	// もとのブレンド値を保持する
	int mode, palam;
	GetDrawBlendMode(&mode, &palam);

	// 描画するためにブレンド値を初期化
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画本体
	for (int i = 0; i < ElementNum; i++)
	{
		if (p_el[i].light > 10)
		{
			DrawCircle(p_el[i].x / 100, p_el[i].y / 100, 255/ p_el[i].rad - p_el[i].light / p_el[i].rad, GetColor(p_el[i].light / 4, p_el[i].light / 2, p_el[i].light), true);
			DrawCircle(p_el[i].x / 100, p_el[i].y / 100, 255 / p_el[i].rad - p_el[i].light / p_el[i].rad, 0xaaaaff, false);
			//DrawPixel(p_el[i].x / 100, p_el[i].y / 100, 0xaaaaff);
			continue;
		}
	}

	// 描画し終わったのでブレンド値を元に戻す
	SetDrawBlendMode(mode, palam);
}
