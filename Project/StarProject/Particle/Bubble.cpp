#include "Bubble.h"
#include "../ResourceManager.h"

constexpr int Magnification = 100;
constexpr int ShakeSize = 31;
constexpr int VanishSpeed = 1;
constexpr int VanishBright = 10;

Bubble::Bubble(int _x, int _y, int _Enum, bool flag, int _BubbleMax):BubbleMax(_BubbleMax)
{
	// ˆø”‚Ì‘ã“ü
	x = _x, y = _y;
	ElementNum = _Enum;
	isSmall = flag;
	vel = 100;
	// ‰Šú‰»
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
	// ºİ½Ä×¸À°‚Ìˆø”‚Åó‚¯æ‚Á‚½—v‘f”•ª”z—ñ‚É—v‘f‚ğì‚é
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

				particle[i].x = (isSmall?x:(Rand() % screen_x))* Magnification;
				particle[i].y = y * Magnification;
				particle[i].bright = 255;

				auto Theta = (Rand() % 360)*DX_PI_F/180.0;
				auto vSize = (Rand() % (vel));
				particle[i].vx = cos(Theta)* (isSmall? vSize/2: vSize * 20);
				particle[i].vy = sin(Theta)*vSize * 10;

				particle[i].avy = -10;

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
	// ½Ú¯ÄŞ‚ª‘–‚Á‚Ä‚¢‚½‚ç‡—¬‚³‚¹‚é
	if (p_thread.joinable())p_thread.join();
#ifdef _DEBUG
	for (auto &p : particle)
	{
		// —áŠOˆ—
		if (p.bright < VanishBright) {
			p.bright = 0;
			continue;
		}
		if ((p.x / 100 < -p.bright) || (p.x / 100 > screen_x + p.bright) || (p.y / 100 > screen_y + p.bright) || (p.y / 100 < -p.bright)) {
			p.bright = 0;
			continue;
		}


		// ˆÚ“®•”•ª
		p.x += p.vx;
		p.y += p.vy;

		// ‰Á‘¬•”•ª
		p.vy += p.avy;
		p.vx += (int)(p.x / Magnification) % 2 ? ShakeSize : -ShakeSize; // ¶‰E‚É—h‚ê‚é

		p.bright -= VanishSpeed;
	}
	
#else
	concurrency::array_view<Element>p_element(ElementNum, particle);
	auto move = [p_element = p_element,sx= screen_x,sy= screen_y](concurrency::index<1> idx)restrict(amp) {
		// —áŠOˆ—
		if (p_element[idx].bright < VanishBright) {
			p_element[idx].bright = 0;
			return;
		}
		if ((p_element[idx].x / 100 < -p_element[idx].bright) || (p_element[idx].x / 100 > sx + p_element[idx].bright) || (p_element[idx].y / 100 > sy + p_element[idx].bright) || (p_element[idx].y / 100 < -p_element[idx].bright)) {
			p_element[idx].bright = 0;
			return;
		}

		// ˆÚ“®•”•ª
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// ‰Á‘¬•”•ª
		p_element[idx].vy += p_element[idx].avy;
		p_element[idx].vx += (int)(p_element[idx].x / Magnification) % 2 ? ShakeSize : -ShakeSize; // ¶‰E‚É—h‚ê‚é

		p_element[idx].bright -= VanishSpeed;
	};

	// GPU‚Å“®‚©‚µ‚Ä‚é
	concurrency::parallel_for_each(p_element.get_extent(), move);
#endif
}

void Bubble::Draw()
{
	// ½Ú¯ÄŞ‚ª‘–‚Á‚Ä‚¢‚½‚ç‡—¬‚³‚¹‚é
	if (p_thread.joinable())p_thread.join();
	// ‚±‚±‚Å“®‚©‚·
	Move();

	int mode, param;
	GetDrawBlendMode(&mode, &param);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	imgBff= ResourceManager::GetInstance().LoadImg("../img/Bubble.png");

	// •`‰æ•”•ª
	for (auto p : particle)
	{
		if (p.bright> VanishBright)
		{
			isSmall ? SetDrawBlendMode(mode, param): SetDrawBlendMode(DX_BLENDMODE_ALPHA, p.bright) ;
			DrawRotaGraphF(p.x / Magnification, p.y / Magnification, (0xff / p.radius - p.bright / p.radius)/ ((Magnification)*(isSmall ? 8 : 1)), 0, imgBff, true);
			continue;
		}
	}
	SetDrawBlendMode(mode, param);
}
