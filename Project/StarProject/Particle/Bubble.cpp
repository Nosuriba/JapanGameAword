#include "Bubble.h"
#include "../ResourceManager.h"
#include "../Camera.h"

constexpr int Magnification = 100;
constexpr int ShakeSize = 31;
constexpr int VanishSpeed = 1;
constexpr int VanishBright = 10;

Bubble::Bubble(int _x, int _y, int _Enum, bool _isSmall, int _BubbleMax,int color):BubbleMax(_BubbleMax), color(color),v_Speed(VanishSpeed)
{
	// 引数の代入
	flag = false;
	x = _x, y = _y;
	ElementNum = _Enum;
	isSmall = _isSmall;
	vel = 100;
	rota = 0;
	// 初期化
	Init();
}

Bubble::Bubble(int _x, int _y, int _Enum, bool _isSmall, bool _flag,int _vs,int _BubbleMax, int _color,const std::shared_ptr<Camera>& c):BubbleMax(_BubbleMax), color(_color),c(c), v_Speed(_vs)
{
	// 引数の代入
	flag = _flag;
	x = _x, y = _y;
	ElementNum = _Enum;
	isSmall = _isSmall;
	vel = 100;
	rota = 0;
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

				particle[i].x = (isSmall?x: flag?x:(Rand() % screen_x))* Magnification;
				particle[i].y = y * Magnification;
				particle[i].bright = 255;

				auto Theta = (Rand() % 360)*DX_PI_F/180.0;
				auto vSize = (Rand() % (vel));

				particle[i].vx = cos(Theta)* (isSmall ? vSize / 2 : flag ? vSize: vSize * 20);
				particle[i].vy = sin(Theta)* (flag ? vSize: vSize * 10);

				if (flag)
				{
					particle[i].avx = cos(rota* DX_PI_F / 180.0)*10;
					particle[i].avy = sin(rota* DX_PI_F / 180.0)*10;
				}
				else
				{
					particle[i].avy = -10;
				}

				particle[i].radius = (Rand() % 3) + 2;
			}
		});
	}
}

void Bubble::Create(int _x, int _y)
{
	x = _x,y=_y;
	Create();
}

void Bubble::Move()
{
	// ｽﾚｯﾄﾞが走っていたら合流させる
	if (p_thread.joinable())p_thread.join();
	auto cpos = c == nullptr ? Vector2() : c->CameraCorrection();
#ifdef _DEBUG
	for (auto &p : particle)
	{
		// 例外処理
		if (p.bright < VanishBright) {
			p.bright = 0;
			continue;
		}
		if ((p.x / 100 < -p.bright) || (p.x / 100 > screen_x+ cpos.x + p.bright) || (p.y / 100 > screen_y+cpos.y + p.bright) || (p.y / 100 < -p.bright)) {
			p.bright = 0;
			continue;
		}


		// 移動部分
		p.x += p.vx;
		p.y += p.vy;

		p.bright -= v_Speed;
		// 加速部分
		p.vy += p.avy;
		p.vx += flag? p.avx :(int)(p.x / Magnification) % 2 ? ShakeSize : -ShakeSize; // 左右に揺れる

	}
	
#else
	concurrency::array_view<Element>p_element(ElementNum, particle);
	auto move = [p_element = p_element,sx= screen_x,sy= screen_y, v_Speed= v_Speed, flag= flag](concurrency::index<1> idx)restrict(amp) {
		// 例外処理
		if (p_element[idx].bright < VanishBright) {
			p_element[idx].bright = 0;
			return;
		}
		if ((p_element[idx].x / 100 < -p_element[idx].bright) || (p_element[idx].x / 100 > sx + p_element[idx].bright) || (p_element[idx].y / 100 > sy + p_element[idx].bright) || (p_element[idx].y / 100 < -p_element[idx].bright)) {
			p_element[idx].bright = 0;
			return;
		}

		// 移動部分
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		p_element[idx].bright -= v_Speed;

		// 加速部分
		p_element[idx].vy += p_element[idx].avy;
		p_element[idx].vx += flag? p_element[idx].avx:(int)(p_element[idx].x / Magnification) % 2 ? ShakeSize : -ShakeSize; // 左右に揺れる

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
	auto cpos = c==nullptr?Vector2():c->CameraCorrection();
	int mode, param,x,y;
	GetDrawBlendMode(&mode, &param);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	imgBff= ResourceManager::GetInstance().LoadImg("../img/Bubble.png");
	GetGraphSize(imgBff,&x,&y);
	// 描画部分
	for (auto &p : particle)
	{
		if (p.bright> VanishBright)
		{
			isSmall ? SetDrawBlendMode(mode, param): SetDrawBlendMode(DX_BLENDMODE_ALPHA, p.bright) ;
			if (color != -1) 
			{
 				DrawCircle(p.x / Magnification- cpos.x, p.y / Magnification - cpos.y, ((y - 90) / 2)*(0xff / p.radius - p.bright / p.radius) / ((Magnification)*(isSmall ? 8 : flag ? 0xff / v_Speed / 2 : 1)), color);
			}
			flag ? SetDrawBlendMode(DX_BLENDMODE_ALPHA, p.bright) : SetDrawBlendMode(mode, param);
			DrawRotaGraphF(p.x / Magnification - cpos.x, p.y / Magnification - cpos.y, (0xff / p.radius - p.bright / p.radius) / ((Magnification)*(isSmall ? 8 : flag ? 0xff / v_Speed / 2 : 1)), 0, imgBff, true);
			continue;
		}
	}
	SetDrawBlendMode(mode, param);
}
