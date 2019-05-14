#include "Player.h"

#include <DxLib.h>
#include <array>
#include <iostream>

#include "Camera.h"
#include "Processing/Input.h"

#define CENTER _star.center
#define LEG(x) _star.legs[x]

constexpr float deceleration = 0.9f;
constexpr float SPEED = 0.5f;

void Player::Normal(const Input & in)
{
	auto input = in;
	GetHitKeyStateAll(Buf);

	_vel *= deceleration;

	if (Buf[KEY_INPUT_UP])		_vel.y -= SPEED;
	if (Buf[KEY_INPUT_DOWN])	_vel.y += SPEED;
	if (Buf[KEY_INPUT_LEFT])	_vel.x -= SPEED;
	if (Buf[KEY_INPUT_RIGHT])	_vel.x += SPEED;
	if (Buf[KEY_INPUT_LSHIFT])	_vel = Vector2();
	if (input.Trigger(BUTTON::A))		LevelUP();
	if (input.Trigger(BUTTON::B))		_updater = &Player::Die;

	float theta_l, theta_r;
	theta_l = theta_r = 0.0f;

	std::array<int, 2> idx = { -1,-1 };

	for (int i = 0; i < _star.legs.size(); i++)
	{
		LEG(i).state = LEG_STATE::NORMAL;

		auto v = LEG(i).tip - CENTER;
		auto lt = Dot(v, input.Stick(STICK::LEFT)) / (v.Magnitude() * input.Stick(STICK::LEFT).Magnitude());
		auto rt = Dot(v, input.Stick(STICK::RIGHT)) / (v.Magnitude() * input.Stick(STICK::RIGHT).Magnitude());

		if (lt > theta_l)
		{
			idx[0] = i;
			theta_l = lt;
		}
		if (rt > theta_r)
		{
			idx[1] = i;
			theta_r = rt;
		}
	}

	for (int i = 0; i < idx.size(); ++i)
	{
		if (idx[i] >= 0)
		{
			LEG(idx[i]).state = LEG_STATE::SELECT;
			if (input.PushTrigger((TRIGGER)i))
			{
				LEG(idx[i]).state = LEG_STATE::SHOT;

				auto v = LEG(idx[i]).pos - LEG(idx[i]).halfway_point[LEG(idx[i]).T / 2];
				_particle[i]->SetPos(LEG(idx[i]).pos.x, LEG(idx[i]).pos.y);
				_particle[i]->SetRota(atan2(v.y, v.x) * 180.0f / DX_PI_F);
				_particle[i]->Create();

			}
			if (input.Push((BUTTON)((int)BUTTON::LB + i)))
				LEG(idx[i]).state = LEG_STATE::HOLD;
		}
	}

	for (auto& l : _star.legs)
	{
		if (l.state == LEG_STATE::SHOT) {
			auto v = l.pos - l.halfway_point[l.T / 2];
			_vel += (-(v).Normalized() * SPEED * (float)_star.level);
		}
	}
}

void Player::Move(const Input & in)
{
}

void Player::Die(const Input & in)
{
	_vel = Vector2();

	_star.r *= 0.99f;
	_star.r = max(_star.r, 10.0f);

	auto radian = 2.0f * DX_PI_F / (float)_star.legs.size();
	for (int i = 0; i < _star.legs.size(); i++)
	{
		LEG(i).tip.x = CENTER.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).tip.y = CENTER.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).pos = LEG(i).tip;
		LEG(i).state = LEG_STATE::NORMAL;
	}
}

Player::Player(const std::shared_ptr<Camera>& c) : _camera(c)
{
	_star.center = Vector2(500, 300);
	_star.level = 1;
	_star.r = 25.0f * (float)_star.level + 25.0f;

	_star.legs.resize(5);
	auto radian = 2.0f * DX_PI_F / (float)_star.legs.size();
	for (int i = 0; i < _star.legs.size(); i++)
	{
		LEG(i).tip.x = CENTER.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).tip.y = CENTER.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).pos = LEG(i).tip;
		LEG(i).state = LEG_STATE::NORMAL;
	}

	_vel = Vector2();

	_particle.resize(2);
	_particle.emplace_back(std::make_shared<Water>(CENTER.x, CENTER.y, 100000));
	_particle.emplace_back(std::make_shared<Water>(CENTER.x, CENTER.y, 100000));

	_updater = &Player::Normal;
}


Player::~Player()
{
}

void Player::Update(const Input& in)
{
	(this->*_updater)(in);

	int hold_count = 0;
	Vector2 asix;
	for (auto& l : _star.legs)
		if (l.state == LEG_STATE::HOLD) 
		{
			++hold_count;
			asix = l.pos;
		}
	MATRIX mat;
	if (hold_count == 1)
	{
		auto v1 = CENTER - asix;
		auto v2 = (CENTER + _vel) - asix;
		mat = MGetTranslate(VScale(asix.V_Cast(), -1));
		mat = MMult(mat, MGetRotVec2(v1.V_Cast(), v2.V_Cast()));
		mat = MMult(mat, MGetTranslate(asix.V_Cast()));
	}
	else
	{
		mat = MGetTranslate(_vel.V_Cast());
	}

	// ヒトデの中心の移動
	_star.center = VTransform(_star.center.V_Cast(), mat);
	// ヒトデの足の先端の移動
	for (auto& l : _star.legs)
	{
		l.tip = VTransform(l.tip.V_Cast(), mat);
	}

	// 足の先端へ足の位置が移動
	for (int i = 0; i < _star.legs.size(); ++i)
	{
		auto v = LEG(i).pos - LEG(i).tip;
		LEG(i).vel = (LEG(i).tip - LEG(i).pos) / 3.0f;
		LEG(i).pos += LEG(i).vel;
	}

	// ヒトデの中心から足の位置までのベジェ曲線
	for (auto& l : _star.legs)
	{
		l.halfway_point.resize(l.T);
		int t = 0;
		l.halfway_point[t] = _star.center;
		for (; t < l.T; ++t)
		{
			float b = (float)t / l.T;
			float a = 1.0f - b;
			l.halfway_point[t].x = a * a * _star.center.x + 2 * a * b * l.tip.x + b * b * l.pos.x;
			l.halfway_point[t].y = a * a * _star.center.y + 2 * a * b * l.tip.y + b * b * l.pos.y;
		}
	}
}

void Player::Draw()
{
	auto c = _camera->CameraCorrection();

	for (auto& leg : _star.legs)
	{
		// 足の先端
		//DrawCircleAA(leg.pos.x, leg.pos.y, 5.0f, 32, 0x00ffff);

		// 足の先端までのライン
		Vector2 pre = leg.halfway_point[0];
		float t = _star.r / 2.5f;
		int color = 0x110000 * _star.level + 0x00ff00 / _star.level;
		for (auto& l : leg.halfway_point)
		{
			DrawLineAA(pre.x - c.x, pre.y - c.y, l.x - c.x, l.y - c.y, color, t);//軌跡描画
			DrawCircleAA(l.x - c.x, l.y - c.y, 1.0f, 32, 0x00ffff);
			pre.x = l.x;//前の位置記憶
			pre.y = l.y;
			t /= 1.3f;
			color += 0x101010;
			color = min(color, 0xffffff);
		}

		if (leg.state == LEG_STATE::SHOT)
		{
			DrawCircleAA(leg.pos.x - c.x, leg.pos.y - c.y, 2.0f * _star.level, 32, 0xff00ff);
			auto v = (leg.pos + (leg.pos - leg.halfway_point[leg.T / 2]) * 100);
			DrawLineAA(leg.pos.x - c.x, leg.pos.y - c.y, v.x - c.x, v.y - c.y, 0x00ffff, 5);
		}
		if (leg.state == LEG_STATE::SELECT)
			DrawCircleAA(leg.pos.x - c.x, leg.pos.y - c.y, 2.0f, 32, 0xffff00);
	}
	for (auto& p : _particle)
	{
		p->Draw();
	}
}

Star Player::GetInfo()
{
	return _star;
}

const std::vector<Vector2> Player::GetShot()
{
	std::vector<Vector2> v;
	v.clear();
	for (auto& l : _star.legs)
	{
		if (l.state == LEG_STATE::SHOT)
		{
			v.push_back(l.tip - _star.center);
		}
		else
		{
			v.push_back(Vector2(0, 0));
		}
	}

	return v;
}

void Player::LevelUP()
{
	_star.level++;

	_star.r = 25.0f * (float)_star.level + 25.0f;

	auto radian = 2.0f * DX_PI_F / (float)_star.legs.size();
	for (int i = 0; i < _star.legs.size(); i++)
	{
		LEG(i).tip.x = CENTER.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).tip.y = CENTER.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).pos = LEG(i).tip;
	}
}
