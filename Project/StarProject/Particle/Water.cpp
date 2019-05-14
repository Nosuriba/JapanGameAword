#include "Water.h"

constexpr int BubbleMax = 10;
constexpr int Magnification = 100;
constexpr int VelocitySize = 100;
constexpr int VanishSpeed = 1;
constexpr int VanishBright = 10;

Water::Water(int _x, int _y, int _Enum, float _Rota)
{
	x = _x, y = _y;
	ElementNum = _Enum;
	rota = _Rota;
	Init();
}


Water::~Water()
{
	if (p_thread.joinable())
	{
		p_thread.join();
	}
}

void Water::Init()
{
	// ºİ½Ä×¸À°‚Ìˆø”‚Åó‚¯æ‚Á‚½—v‘f”•ª”z—ñ‚É—v‘f‚ğì‚é
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
				auto vSize = (Rand() % (VelocitySize));
				particle[i].vx = cos(Theta)*vSize;
				particle[i].vy = sin(Theta)*vSize;
				particle[i].avx = sin(rota* DX_PI_F / 180.0)*VelocitySize;
				particle[i].avy = cos(rota* DX_PI_F / 180.0)*VelocitySize;

				particle[i].radius = (Rand() % 3) + 2;
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
	concurrency::array_view<Element>p_element(ElementNum, particle);
	auto move = [p_element = p_element](concurrency::index<1> idx)restrict(amp) {
		if (p_element[idx].bright < 10) {
			p_element[idx].bright = 0;
			return;
		}

		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// ‰Á‘¬•”•ª
		p_element[idx].vy += p_element[idx].avx;
		p_element[idx].vx += p_element[idx].avy;
		p_element[idx].bright -= VanishSpeed;
	};

	// GPU‚Å“®‚©‚µ‚Ä‚é
	concurrency::parallel_for_each(p_element.get_extent(), move);
}

void Water::Draw()
{	// ‚±‚±‚Å“®‚­
	Move();

	concurrency::array_view<Element>p_element(ElementNum, particle);

	for (auto p : particle)
	{
		if (p.bright > 0)
		{
			DrawCircle(p.x / 100, p.y / 100, 0xff / p.radius - p.bright / p.radius, GetColor(p.bright/2, p.bright, p.bright), true);
			//DrawCircle(p.x / 100, p.y / 100, 0xff / p.radius - p.bright / p.radius, GetColor(p.bright, p.bright, p.bright), false);
		}
	}
}
