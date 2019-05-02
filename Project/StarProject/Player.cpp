#include "Player.h"

#include <DxLib.h>

constexpr float deceleration = 0.9f;

Player::Player()
{
	star.center = Vector2(500, 300);
	star.r = 50;

	auto radian = 2.0f * DX_PI_F / (float)star.vertexs.size();
	for (int i = 0; i < star.vertexs.size(); i++)
	{
		star.vertexs[i].x = star.center.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * star.r;
		star.vertexs[i].y = star.center.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * star.r;
	}
}


Player::~Player()
{
}

void Player::Update()
{
	GetHitKeyStateAll(Buf);

	for (auto& s : star.shot)	s = false;

	vel = vel * deceleration;
	star.axis = -1;



	if (Buf[KEY_INPUT_NUMPAD8])		star.axis = 0;
	if (Buf[KEY_INPUT_NUMPAD6])		star.axis = 1;
	if (Buf[KEY_INPUT_NUMPAD3])		star.axis = 2;
	if (Buf[KEY_INPUT_NUMPAD1])		star.axis = 3;
	if (Buf[KEY_INPUT_NUMPAD4])		star.axis = 4;

	if (Buf[KEY_INPUT_W])
	{
		star.shot[0] = true;
		vel += (star.center - star.vertexs[0]).Normalized();
	}
	if (Buf[KEY_INPUT_D])
	{
		star.shot[1] = true;
		vel += (star.center - star.vertexs[1]).Normalized();
	}
	if (Buf[KEY_INPUT_X])
	{
		star.shot[2] = true;
		vel += (star.center - star.vertexs[2]).Normalized();
	}
	if (Buf[KEY_INPUT_Z])
	{
		star.shot[3] = true;
		vel += (star.center - star.vertexs[3]).Normalized();
	}
	if (Buf[KEY_INPUT_A])
	{
		star.shot[4] = true;
		vel += (star.center - star.vertexs[4]).Normalized();
	}

	MATRIX mat;
	if (star.axis < 0)
	{
		mat = MGetTranslate(vel.V_Cast());
	}
	else
	{
		auto v1 = star.center - star.vertexs[star.axis];
		auto v2 = (star.center + vel) - star.vertexs[star.axis];

		mat = MGetTranslate(VScale(star.vertexs[star.axis].V_Cast(), -1));
		mat = MMult(mat, MGetRotVec2(v1.V_Cast(), v2.V_Cast()));
		mat = MMult(mat, MGetTranslate(star.vertexs[star.axis].V_Cast()));
	}

	star.center = VTransform(star.center.V_Cast(), mat);
	for (auto& v : star.vertexs)
	{
		v = VTransform(v.V_Cast(), mat);
	}
}

void Player::Draw()
{
	DrawCircleAA(star.center.x, star.center.y, 2.0f, 32, 0x00ff00);
	for (int i = 0; i < star.vertexs.size(); i++)
	{
		DrawCircleAA(star.vertexs[i].x, star.vertexs[i].y, 2.0f, 32, star.axis == i ? 0xff00ff : 0x00ff00);
	}

	for (int i = 0; i < star.vertexs.size(); i++)
	{
		DrawLineAA(
			star.vertexs[i].x, star.vertexs[i].y,
			star.vertexs[(i + 2) % star.vertexs.size()].x, star.vertexs[(i + 2) % star.vertexs.size()].y,
			0x00ff00);
	}

	for (auto i = 0; i < star.shot.size(); i++)
	{
		if (star.shot[i])
		{
			auto v = star.vertexs[i] - star.center;
			DrawLineAA(star.vertexs[i].x, star.vertexs[i].y,
				star.vertexs[i].x + v.x * 10, star.vertexs[i].y + v.y * 10, 0xff0000);
		}
	}
}

Star Player::GetInfo()
{
	return star;
}