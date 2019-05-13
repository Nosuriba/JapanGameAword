#include "Bubble.h"
#include <iostream>
#include <math.h>
#include <random>
#include "../StarProject/Scene/AMP.h"

constexpr int offset = 250;					// Yの値をどのくらい下げるかのｵﾌｾｯﾄ
std::unique_ptr<Bubble, Bubble::Bubble_deleter> Bubble::particle(new Bubble(SCREEN_SIZE_X/2, SCREEN_SIZE_Y+ offset));

constexpr int Magnification = 100;			// 倍率
constexpr int Vec = 2* Magnification;		// ｽﾋﾟｰﾄﾞ
constexpr int BubbleMax = 5;				// 一回の生成で出す泡の量

constexpr int V_Speed = 1;					// 消える速度
constexpr int V_Light = 10;					// 消える明るさ
constexpr int V_Cnt = 0xff/ V_Speed;		// 消えるまでのﾌﾚｰﾑ数

Bubble::Bubble(int _x, int _y)
{
	cnt=0;
	x = _x;
	y = _y;
}

Bubble::~Bubble()
{
	// スレッドが動いてるのに終了しようとしたら合流させる
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Create()
{
	// ｽﾚｯﾄﾞを走らせてもいいかのﾁｪｯｸ
	if (!thread.joinable())
	{
		// 別ｽﾚｯﾄﾞに投げてる処理
		thread = std::thread([=] {

			// ﾗﾝﾀﾞﾑ関数の生成
			std::random_device rd;
			std::mt19937 Rand(rd());

			// 実行部
			for (int num = 0; num < BubbleMax; num++)
			{
				int i = 0;
				for (i = 0; i < ELEMENT_NUM; i++)
				{
					// 明るさが規定値以下だったら抜ける
					if (p_el[i].light <= V_Light)
					{
						break;
					}
				}

				// 初期値の設定
				p_el[i].x = (x) * Magnification;
				p_el[i].y = (y) * Magnification;
				p_el[i].light = 0xff;
				p_el[i].rad = (Rand()%4)+1;

				// 乱数を用いてxとyの速度を決める(ここを定数等にできればGPUに処理を渡せる)
				int Theta = Rand() % 360;
				p_el[i].vx = cos(Theta)*Vec * 4;
				p_el[i].vy = sin(Theta % 180 - 90)*Vec;
			}
		});
	}

	// ｽﾚｯﾄﾞの合流
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Move()
{
	// GPUで使うための配列を用意する
	concurrency::array_view<Element>p_element(ELEMENT_NUM, p_el);

	// GPUに動かしてもらう関数
	auto move = [p_element = p_element](concurrency::index<1> idx)restrict(amp){
		// 暗くなってきたり、画面外に泡が出てしまったら自動で消える処理
		if (p_element[idx].light < V_Light){
			p_element[idx].light = 0;
			return;
		}

		if ((-0xff * Magnification >p_element[idx].x)||(p_element[idx].y < -0xff * Magnification)||(((SCREEN_SIZE_X + 0xff+200) *Magnification)<p_element[idx].x))
		{
			p_element[idx].light = 0;
			return;
		}

		// ここから実行部
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// 加速部分
		p_element[idx].vy += -15; // 上に上がっていく
		p_element[idx].vx += (int)(p_element[idx].x/ Magnification) % 2 ? 101: -101; // 左右に揺れる
		p_element[idx].light -= V_Speed; // 徐々に消える
	};

	// GPUで動かしてる
	concurrency::parallel_for_each(p_element.get_extent(), move);
}

void Bubble::Draw()
{
	// ここで動かす
	Move();

	/*ここから描画*/
	// もとのﾌﾞﾚﾝﾄﾞ値を一旦保持する
	int mode, palam;
	GetDrawBlendMode(&mode, &palam);

	// 描画するためにﾌﾞﾚﾝﾄﾞ値を初期化
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画本体
	for (int i = 0; i < ELEMENT_NUM; i++)
	{
		if (p_el[i].light > V_Light)
		{
			DrawCircle(p_el[i].x / Magnification, p_el[i].y / Magnification, 0xff / p_el[i].rad - p_el[i].light / p_el[i].rad, GetColor(p_el[i].light / 3, p_el[i].light * 2 / 3, p_el[i].light), 3);
			DrawCircle(p_el[i].x / Magnification, p_el[i].y / Magnification, 0xff / p_el[i].rad - p_el[i].light / p_el[i].rad, 0x9999ff, 0);
			DrawCircle(
				p_el[i].x / Magnification + p_el[i].x / (Magnification * 10),
				p_el[i].y / Magnification + p_el[i].y / (Magnification * 10),
				0xff / (p_el[i].rad * 2) - p_el[i].light / (p_el[i].rad * 2),
				0xffffff,
				false
			);
			continue;
		}
	}

	// 描画し終わったのでﾌﾞﾚﾝﾄﾞ値を元に戻す
	SetDrawBlendMode(mode, palam);
}
