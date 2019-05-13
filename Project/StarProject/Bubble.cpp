#include "Bubble.h"
#include <iostream>
#include <math.h>
#include <random>
#include "../StarProject/Scene/AMP.h"

constexpr int offset = 250;					// Y�̒l���ǂ̂��炢�����邩�̵̾��
std::unique_ptr<Bubble, Bubble::Bubble_deleter> Bubble::particle(new Bubble(SCREEN_SIZE_X/2, SCREEN_SIZE_Y+ offset));

constexpr int Magnification = 100;			// �{��
constexpr int Vec = 2* Magnification;		// ��߰��
constexpr int BubbleMax = 5;				// ���̐����ŏo���A�̗�

constexpr int V_Speed = 1;					// �����鑬�x
constexpr int V_Light = 10;					// �����閾�邳
constexpr int V_Cnt = 0xff/ V_Speed;		// ������܂ł��ڰѐ�

Bubble::Bubble(int _x, int _y)
{
	cnt=0;
	x = _x;
	y = _y;
}

Bubble::~Bubble()
{
	// �X���b�h�������Ă�̂ɏI�����悤�Ƃ����獇��������
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Create()
{
	// �گ�ނ𑖂点�Ă�������������
	if (!thread.joinable())
	{
		// �ʽگ�ނɓ����Ă鏈��
		thread = std::thread([=] {

			// ����ъ֐��̐���
			std::random_device rd;
			std::mt19937 Rand(rd());

			// ���s��
			for (int num = 0; num < BubbleMax; num++)
			{
				int i = 0;
				for (i = 0; i < ELEMENT_NUM; i++)
				{
					// ���邳���K��l�ȉ��������甲����
					if (p_el[i].light <= V_Light)
					{
						break;
					}
				}

				// �����l�̐ݒ�
				p_el[i].x = (x) * Magnification;
				p_el[i].y = (y) * Magnification;
				p_el[i].light = 0xff;
				p_el[i].rad = (Rand()%4)+1;

				// ������p����x��y�̑��x�����߂�(������萔���ɂł����GPU�ɏ�����n����)
				int Theta = Rand() % 360;
				p_el[i].vx = cos(Theta)*Vec * 4;
				p_el[i].vy = sin(Theta % 180 - 90)*Vec;
			}
		});
	}

	// �گ�ނ̍���
	if (thread.joinable())
	{
		thread.join();
	}
}

void Bubble::Move()
{
	// GPU�Ŏg�����߂̔z���p�ӂ���
	concurrency::array_view<Element>p_element(ELEMENT_NUM, p_el);

	// GPU�ɓ������Ă��炤�֐�
	auto move = [p_element = p_element](concurrency::index<1> idx)restrict(amp){
		// �Â��Ȃ��Ă�����A��ʊO�ɖA���o�Ă��܂����玩���ŏ����鏈��
		if (p_element[idx].light < V_Light){
			p_element[idx].light = 0;
			return;
		}

		if ((-0xff * Magnification >p_element[idx].x)||(p_element[idx].y < -0xff * Magnification)||(((SCREEN_SIZE_X + 0xff+200) *Magnification)<p_element[idx].x))
		{
			p_element[idx].light = 0;
			return;
		}

		// ����������s��
		p_element[idx].x += p_element[idx].vx;
		p_element[idx].y += p_element[idx].vy;

		// ��������
		p_element[idx].vy += -15; // ��ɏオ���Ă���
		p_element[idx].vx += (int)(p_element[idx].x/ Magnification) % 2 ? 101: -101; // ���E�ɗh���
		p_element[idx].light -= V_Speed; // ���X�ɏ�����
	};

	// GPU�œ������Ă�
	concurrency::parallel_for_each(p_element.get_extent(), move);
}

void Bubble::Draw()
{
	// �����œ�����
	Move();

	/*��������`��*/
	// ���Ƃ������ޒl����U�ێ�����
	int mode, palam;
	GetDrawBlendMode(&mode, &palam);

	// �`�悷�邽�߂������ޒl��������
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �`��{��
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

	// �`�悵�I������̂������ޒl�����ɖ߂�
	SetDrawBlendMode(mode, palam);
}
