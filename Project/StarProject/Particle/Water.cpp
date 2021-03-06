#include "Water.h"
#include "../Camera.h"
#include "../ResourceManager.h"

constexpr int BubbleMax = 10;
constexpr int Magnification = 100;
constexpr int VanishSpeed = 1;
constexpr int VanishBright = 10;

Water::Water(int _x, int _y, int _Enum,const std::shared_ptr<Camera>& c)
{
	x = _x, y = _y;
	ElementNum = _Enum;
	camera = c;
	vel = 100;
	Init();
}


Water::~Water()
{
	if (p_thread.joinable())
		p_thread.join();
}

void Water::Init()
{
	// ｺﾝｽﾄﾗｸﾀｰの引数で受け取った要素数分配列に要素を作る
	Element e;
	for (auto i = 0; i < ElementNum; i++)
	{
		particle.push_back(e);
	}
}

void Water::Create()
{
	if (!p_thread.joinable())
	{
		p_thread = std::thread([&] {
			std::random_device rd;
			std::mt19937 Rand(rd());
			for (int num = 0; num < BubbleMax; num++)
			{
				int i = 0;
				for (i = 0; i < ElementNum; i++)
				{
					if (particle[i].bright <= VanishBright)break;
				}

				if (i == ElementNum)return;

				particle[i].x = (x)* Magnification;
				particle[i].y = (y)* Magnification;
				particle[i].bright = 255;

				auto Theta = (Rand() % 360)* DX_PI_F / 180.0;
				auto vSize = (Rand() % (vel/10))+ (vel / 10)*9;

				particle[i].vx = cos(Theta)*vel /2;
				particle[i].vy = sin(Theta)*vel /2;

				particle[i].avx = cos(rota* DX_PI_F / 180.0)*vSize;
				particle[i].avy = sin(rota* DX_PI_F / 180.0)*vSize;

				particle[i].radius = (Rand() % 3) + 7;
			}
		});
	}
}

void Water::Move()
{
	if (p_thread.joinable())
	{
		p_thread.join();
	}
	auto c = camera->CameraCorrection();
#ifdef _DEBUG
	for (auto &p: particle)
	{
		if (p.bright < 10) {
			p.bright = 0;
			continue;
		}
		if ((p.x / 100 -c.x< -p.bright) || (p.x / 100 - c.x > screen_x + p.bright) ||
			(p.y / 100 - c.y > screen_y + p.bright) || (p.y / 100 - c.y < -p.bright)) {
			p.bright = 0;
			continue;
		}

		p.x += p.vx;
		p.y += p.vy;

		// 加速部分
		p.vy += p.avy;
		p.vx += p.avx;
		p.bright -= VanishSpeed;
	}
#else
	concurrency::array_view<Element>p_element(ElementNum, particle);
	auto move = [p_element = p_element, sx = screen_x, sy = screen_y,c=c](concurrency::index<1> idx)restrict(amp) {
		if (p_element[idx].bright < 10) {
			p_element[idx].bright = 0;
			return;
		}

		if ((p_element[idx].x / 100 - c.x < -p_element[idx].bright / p_element[idx].radius) || (p_element[idx].x / 100 - c.x > sx + p_element[idx].bright / p_element[idx].radius) ||
			(p_element[idx].y / 100 - c.y > sy + p_element[idx].bright / p_element[idx].radius) || (p_element[idx].y / 100 - c.y < -p_element[idx].bright / p_element[idx].radius)) {
			p_element[idx].bright = 0;
			return;
		}
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// 加速部分
		p_element[idx].vx += p_element[idx].avx;
		p_element[idx].vy += p_element[idx].avy;
		p_element[idx].bright -= VanishSpeed;
	};

	// GPUで動かしてる
	concurrency::parallel_for_each(p_element.get_extent(), move);
#endif
}

void Water::Draw(int color)
{	// ここで動く
	Move();

	auto c = camera->CameraCorrection();
	for (auto p : particle)
	{
		if (p.bright > 0)
		{
			DrawCircle(p.x / 100-c.x, p.y / 100 - c.y, 0xff / p.radius - p.bright / p.radius, (color==-1)?GetColor(p.bright*2/3, p.bright, p.bright): color, true);
		}
	}
}
