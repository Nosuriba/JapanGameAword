#include "Bubble.h"

constexpr int BubbleMax = 100;
constexpr int Magnification = 100;
constexpr int VelocitySize = 100;
constexpr int VanishSpeed = 1;
constexpr int VanishBright = 10;

Bubble::Bubble(int _x,int _y,int _Enum)
{
	// 引数の代入
	x = _x, y = _y;
	ElementNum = _Enum;

	// 初期化
	Init();
}

Bubble::~Bubble()
{
	if (p_thread.joinable())
	{
		p_thread.join();
	}
}

void Bubble::Init()
{
	// ｺﾝｽﾄﾗｸﾀｰの引数で受け取った要素数分配列に要素を作る
	Element e;
	for (auto i = 0; i < ElementNum;i++)
	{
		particle.push_back(e);
	}
}

void Bubble::Create()
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
			particle[i].vx = cos(Theta)*vSize * 20;
			particle[i].vy = sin(Theta)*vSize * 10;

			particle[i].radius = (Rand() % 3) + 2;
		}
	});
	
}

void Bubble::Move()
{
#ifdef _DEBUG
	for (auto &p : particle)
	{
		// 例外処理
		if (p.bright < VanishBright) {
			p.bright = 0;
			continue;
		}

		// 移動部分
		p.x += p.vx;
		p.y += p.vy;

		// 加速部分
		p.vy += -10;
		p.vx += (int)(p.x / Magnification) % 2 ? 101 : -101; // 左右に揺れる

		p.bright -= VanishSpeed;
	}
#else
	concurrency::array_view<Element>p_element(ElementNum, particle);
	auto move = [p_element = p_element](concurrency::index<1> idx)restrict(amp) {
		// 例外処理
		if (p_element[idx].bright < VanishBright) {
			p_element[idx].bright = 0;
			return;
		}

		// 移動部分
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// 加速部分
		p_element[idx].vy += -10;
		p_element[idx].vx += (int)(p_element[idx].x / Magnification) % 2 ? 101 : -101; // 左右に揺れる

		p_element[idx].bright -= VanishSpeed;
	};

	// GPUで動かしてる
	concurrency::parallel_for_each(p_element.get_extent(), move);
#endif
}

void Bubble::Draw()
{
	// ｽﾚｯﾄﾞが走っていたら合流させる
	if (p_thread.joinable())p_thread.join();

	// ここで動かす
	Move();

	int mode, param;
	GetDrawBlendMode(&mode, &param);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	// 描画部分
	for (auto p : particle)
	{
		if (p.bright> VanishBright)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, p.bright);
			DrawCircle(p.x / Magnification,p.y / Magnification, 0xff / p.radius - p.bright / p.radius, GetColor(p.bright / 3, p.bright * 2 / 3, p.bright), 3);
			DrawCircle(p.x / Magnification, p.y / Magnification, 0xff / p.radius - p.bright / p.radius, 0x9999ff, 0);
			DrawCircle(
				p.x / Magnification + p.x / (Magnification * 10),
				p.y / Magnification + p.y / (Magnification * 10),
				0xff / (p.radius * 2) - p.bright / (p.radius * 2),
				0xffffff,
				false
			);
			// DrawPixel(p.x/100,p.y/100,GetColor(p.bright, p.bright, p.bright));/*粒子*/
		}
	}
	SetDrawBlendMode(mode, param);
}
