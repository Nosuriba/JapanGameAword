#include "Player.h"

#include <DxLib.h>
#include <iostream>

#include "Camera.h"
#include "Processing/Input.h"
#include "ResourceManager.h"

#define CENTER _star.center
#define LEG(x) _star.legs[x]

constexpr float deceleration = 0.9f;
constexpr float SPEED = 0.5f;

void Player::Normal(const Input & in)
{
	if (Buf[KEY_INPUT_P]) 
		ToCatch(LEG(3).pos);
	if (Buf[KEY_INPUT_O]) 
		OnDamage();

	// 左STICKの入力
	if (in.PushTrigger(TRIGGER::LEFT) == 0 && !in.Push(BUTTON::LB))
	{
		select_idx[0] = -1;
		float theta = 0;
		for (int i = 0; i < _star.legs.size(); i++)
		{
			auto v = LEG(i).tip - CENTER;
			auto lt = Dot(v, in.Stick(STICK::LEFT)) / (v.Magnitude() * in.Stick(STICK::LEFT).Magnitude());

			if (lt > theta)
			{
				select_idx[0] = i;
				theta = lt;
			}
		}
	}
	// 右STICKの入力
	if (in.PushTrigger(TRIGGER::RIGHT) == 0 && !in.Push(BUTTON::RB))
	{
		select_idx[1] = -1;
		float theta = 0;
		for (int i = 0; i < _star.legs.size(); i++)
		{
			auto v = LEG(i).tip - CENTER;
			auto rt = Dot(v, in.Stick(STICK::RIGHT)) / (v.Magnitude() * in.Stick(STICK::RIGHT).Magnitude());

			if (rt > theta)
			{
				select_idx[1] = i;
				theta = rt;
			}
		}
	}

	for (int i = 0; i < _star.legs.size(); ++i)
	{
		auto v = LEG(i).tip - _star.center;
		LEG(i).vel *= deceleration;

		if (i == select_idx[0])
		{
			if (in.PushTrigger(TRIGGER::LEFT))
			{
				LEG(i).state = LEG_STATE::SHOT;

				auto v = LEG(i).pos - LEG(i).halfway_point[LEG(i).T / 2];
				LEG(i).vel += (-(v).Normalized() * SPEED * (float)_star.level);
				
				//_laser.emplace_back(LEG(i).pos, v.Normalized());

				_particle[0]->SetPos(LEG(i).pos.x, LEG(i).pos.y);
				_particle[0]->SetRota(atan2(v.Normalized().y, v.Normalized().x) * 180.0f / DX_PI_F);
				_particle[0]->Create();

			}
			else if (in.Push(BUTTON::LB))
			{
				LEG(i).state = LEG_STATE::HOLD;

				LEG(i).vel = Vector2();

				if (v.Magnitude() > (_star.r * 0.9f))
					LEG(i).tip = _star.center + v.Normalized() * max((_star.r * 0.9f), v.Magnitude() - 0.1f * _star.level);
			}
			else
			{
				LEG(i).state = LEG_STATE::SELECT;

				if (v.Magnitude() < (_star.r * 1.1f))
					LEG(i).tip = _star.center + v.Normalized() * min((_star.r * 1.1f), v.Magnitude() + 0.1f * _star.level);
			}
		}
		else if (i == select_idx[1])
		{
			if (in.PushTrigger(TRIGGER::RIGHT))
			{
				LEG(i).state = LEG_STATE::SHOT;

				auto v = LEG(i).pos - LEG(i).halfway_point[LEG(i).T / 2];
				LEG(i).vel += (-(v).Normalized() * SPEED * (float)_star.level);

				//_laser.emplace_back(LEG(i).pos, v.Normalized());

				_particle[1]->SetPos(LEG(i).pos.x, LEG(i).pos.y);
				_particle[1]->SetRota(atan2(v.Normalized().y, v.Normalized().x) * 180.0f / DX_PI_F);
				_particle[1]->Create();

			}
			else if (in.Push(BUTTON::RB))
			{
				LEG(i).state = LEG_STATE::HOLD;

				LEG(i).vel = Vector2();

				if (v.Magnitude() > (_star.r * 0.9f))
					LEG(i).tip = _star.center + v.Normalized() * max((_star.r * 0.9f), v.Magnitude() - 0.1f * _star.level);
			}
			else
			{
				LEG(i).state = LEG_STATE::SELECT;

				if (v.Magnitude() < (_star.r * 1.1f))
					LEG(i).tip = _star.center + v.Normalized() * min((_star.r * 1.1f), v.Magnitude() + 0.1f * _star.level);
			}
		}
		else
		{
			LEG(i).state = LEG_STATE::NORMAL;

			if (v.Magnitude() > _star.r)
				LEG(i).tip = _star.center + v.Normalized() * max(_star.r, v.Magnitude() - 0.1f);
			if (v.Magnitude() < _star.r)
				LEG(i).tip = _star.center + v.Normalized() * max(_star.r, v.Magnitude() + 0.1f);
		}
	}

	for (auto& l : _star.legs)
	{
		if (l.vel.Magnitude() != 0)
		{
			auto v = CENTER - l.tip;
			auto lt = Dot(v, l.vel) / (v.Magnitude() * l.vel.Magnitude());

			auto move = v.Normalized() * (l.vel.Magnitude() * lt);

			auto v1 = l.tip - (CENTER + move);
			auto v2 = (l.tip + l.vel) - CENTER;

			MATRIX mat = MGetIdent();
			mat = MGetTranslate(move.V_Cast());

			// ヒトデの中心の移動
			CENTER = VTransform(CENTER.V_Cast(), mat);

			mat = MMult(mat, MGetTranslate(VScale(CENTER.V_Cast(), -1)));
			mat = MMult(mat, MGetRotVec2(v1.V_Cast(), v2.V_Cast()));
			mat = MMult(mat, MGetTranslate(CENTER.V_Cast()));

			// ヒトデの足の先端の移動
			for (auto& l : _star.legs)
				l.tip = VTransform(l.tip.V_Cast(), mat);
		}
	}
}

void Player::Predation(const Input & in)
{
	MATRIX mat = MGetIdent();

	auto move = _target - CENTER;
	if (move.Magnitude() > 5.0f)
		mat = MGetTranslate((move.Normalized() * 6.0f).V_Cast());

	// ヒトデの中心の移動
	CENTER = VTransform(CENTER.V_Cast(), mat);
	// ヒトデの足の先端の移動
	for (auto& l : _star.legs)
	{
		l.tip = VTransform(l.tip.V_Cast(), mat);

		auto v = l.tip - CENTER;

		if (_anim_frame < 30)
			l.tip = CENTER + v.Normalized() * min((_star.r * 1.5f), v.Magnitude() + 2.0f * _star.level);
		else if(_anim_frame < 60)
			l.tip = CENTER + v.Normalized() * max(_star.r, v.Magnitude() - 1.5f * _star.level);
		else if (_anim_frame < 75)
			l.tip = CENTER + v.Normalized() * min((_star.r * 1.5f), v.Magnitude() + 1.5f * _star.level);
		else 
			l.tip = CENTER + v.Normalized() * max(_star.r, v.Magnitude() - 1.5f * _star.level);
	}
	++_anim_frame;
	if(_anim_frame > 90)
		_updater = &Player::Normal;
}

void Player::Die(const Input & in)
{

	MATRIX mat = MGetIdent();

	mat = MGetTranslate(VScale(CENTER.V_Cast(), -1));
	mat = MMult(mat, MGetRotZ(DX_PI_F / 180.0f * 5.0f));
	mat = MMult(mat, MGetTranslate(CENTER.V_Cast()));

	// ヒトデの中心の移動
	CENTER = VTransform(CENTER.V_Cast(), mat);

	_star.r *= 0.94f;
	// ヒトデの足の先端の移動
	for (auto& l : _star.legs)
	{
		l.tip = VTransform(l.tip.V_Cast(), mat);
		auto v = l.tip - CENTER;
		l.tip = CENTER + v.Normalized() * _star.r;
	}

	++_anim_frame;
}

void Player::SetStar(const Vector2 & p, const float & s)
{
	auto move	= p - CENTER;
	_star.r		= s;
	MATRIX mat	= MGetTranslate(move.V_Cast());

	CENTER = VTransform(CENTER.V_Cast(), mat);
	for (auto& l : _star.legs)
	{
		l.tip = VTransform(l.tip.V_Cast(), mat);
		l.pos = VTransform(l.pos.V_Cast(), mat);

		auto v = l.tip - CENTER;
		v.Normalize();
		l.tip = CENTER + v * _star.r;
		l.pos = CENTER + v * _star.r;
	}
}

Player::Player(const std::shared_ptr<Camera>& c) : _camera(c)
{
	_star.center = Vector2(500, 300);
	_star.level = 1;
	_star.r = 50.0f * (float)_star.level + 25.0f;

	_star.legs.resize(5);
	auto radian = 2.0f * DX_PI_F / (float)_star.legs.size();
	for (int i = 0; i < _star.legs.size(); i++)
	{
		LEG(i).tip.x = CENTER.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).tip.y = CENTER.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _star.r;
		LEG(i).pos = LEG(i).tip;
		LEG(i).state = LEG_STATE::NORMAL;
		LEG(i).halfway_point.resize(LEG(i).T);
	}
	select_idx = { -1,-1 };
	_vel = Vector2();

	_particle.emplace_back(std::make_shared<Water>(CENTER.x, CENTER.y, 5000, _camera));
	_particle.emplace_back(std::make_shared<Water>(CENTER.x, CENTER.y, 5000, _camera));

	_img_STICK = ResourceManager::GetInstance().LoadImg("../img/STICK.png");
	_img_TRIGGER = ResourceManager::GetInstance().LoadImg("../img/TRIGGER.png");

	_updater = &Player::Normal;
}


Player::~Player()
{
}

void Player::Update(const Input& in)
{
	GetHitKeyStateAll(Buf);

	for (auto& l : _laser)
	{
		l.pos += l.vel;
		l.vel += l.vel.Normalized();
		++l.count;
	}
	_laser.remove_if([](Laser l) { return l.count > 75; });

	(this->*_updater)(in);

	// 足の先端へ足の位置が移動
	for (int i = 0; i < _star.legs.size(); ++i)
	{
		auto v = LEG(i).pos - LEG(i).tip;
		LEG(i).pos += (LEG(i).tip - LEG(i).pos) / 3.0f;
	}
}

void Player::Draw()
{
	auto c = _camera->CameraCorrection();

	int count = 0;
	for (auto& leg : _star.legs)
	{
		// ヒトデの中心から足の位置までのベジェ曲線
		int idx = 0;
		leg.halfway_point[idx] = _star.center;
		for (; idx < leg.T; ++idx)
		{
			float b = (float)idx / leg.T;
			float a = 1.0f - b;
			leg.halfway_point[idx].x = a * a * _star.center.x + 2 * a * b * leg.tip.x + b * b * leg.pos.x;
			leg.halfway_point[idx].y = a * a * _star.center.y + 2 * a * b * leg.tip.y + b * b * leg.pos.y;
		}

		if (leg.state != LEG_STATE::NORMAL) ++count;
		// 足の先端までのライン
		Vector2 pre = leg.halfway_point[0];
		float t = _star.r / 2.0f;
		int color = leg.state == LEG_STATE::NORMAL ? 0xff0000 : 0xffff00;
		for (auto& l : leg.halfway_point)
		{
			DrawLineAA(pre.x - c.x, pre.y - c.y, l.x - c.x, l.y - c.y, color, t);//軌跡描画
			DrawCircleAA(l.x - c.x, l.y - c.y, t / 10.0f, 32, 0x00ffff);
			pre.x = l.x;//前の位置記憶
			pre.y = l.y;
			t /= 1.3f;
			color -= 0x110000;
		}
		DrawLineAA(pre.x - c.x, pre.y - c.y, leg.pos.x - c.x, leg.pos.y - c.y, color, t);//軌跡描画
	}
	DxLib::DrawCircle(CENTER.x - c.x, CENTER.y - c.y, _star.r / 4.0f, 0xee0000, true);
	//if (count == 0)
	//	DrawRotaGraph(CENTER.x - c.x, CENTER.y - c.y, 0.5f, 0, _img_STICK, true);
	for (int i = 0; i < select_idx.size(); ++i)
		if (select_idx[i] != -1)
			DrawRectRotaGraph
			(LEG(select_idx[i]).tip.x - c.x, LEG(select_idx[i]).tip.y - c.y, 22 * i, 0, 22, 55, 0.5f, 0, _img_TRIGGER, true);
	for (auto& p : _particle)
		p->Draw();
}

void Player::ShadowDraw()
{
	auto c = _camera->CameraCorrection();
	auto s = _camera->GetShadowPos(0.3f);

	for (auto& leg : _star.legs)
	{
		// 足の先端までのライン
		Vector2 pre = leg.halfway_point[0];
		float t = _star.r / 2.5f;
		int color = 0x000000;
		for (auto& l : leg.halfway_point)
		{
			DrawLineAA(
				pre.x - c.x + s.x, pre.y - c.y + s.y, 
				l.x - c.x + s.x, l.y - c.y + s.y, 
				color, t);//軌跡描画
			pre.x = l.x;//前の位置記憶
			pre.y = l.y;
			t /= 1.3f;
		}
	}
}

void Player::SelectDraw(const Vector2 p, const float s)
{
	SetStar(p, s);
	for (auto& leg : _star.legs)
	{
		// ヒトデの中心から足の位置までのベジェ曲線
		int idx = 0;
		leg.halfway_point[idx] = _star.center;
		for (; idx < leg.T; ++idx)
		{
			float b = (float)idx / leg.T;
			float a = 1.0f - b;
			leg.halfway_point[idx].x = a * a * _star.center.x + 2 * a * b * leg.tip.x + b * b * leg.pos.x;
			leg.halfway_point[idx].y = a * a * _star.center.y + 2 * a * b * leg.tip.y + b * b * leg.pos.y;
		}

		// 足の先端までのライン
		Vector2 pre = leg.halfway_point[0];
		float t = _star.r / 2.0f;
		int color = leg.state == LEG_STATE::NORMAL ? 0xff0000 : 0xffff00;
		for (auto& l : leg.halfway_point)
		{
			DrawLineAA(pre.x, pre.y, l.x, l.y, color, t);//軌跡描画
			DrawCircleAA(l.x, l.y, 1.0f, 32, 0x00ffff);
			pre.x = l.x;//前の位置記憶
			pre.y = l.y;
			t /= 1.3f;
			color -= 0x110000;
		}
		DrawLineAA(pre.x, pre.y, leg.pos.x, leg.pos.y, color, t);//軌跡描画
	}
	DxLib::DrawCircle(CENTER.x, CENTER.y, _star.r / 4.0f, 0xee0000, true);
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

const std::list<Laser> Player::GetLaser()
{
	return _laser;
}

void Player::LevelUP()
{
	_star.level++;

	_star.r = 50.0f * (float)_star.level + 25.0f;

	for (auto& l : _star.legs)
	{
		auto v = l.tip - _star.center;
		v.Normalize();
		l.tip = _star.center + v * _star.r;
	}
}

void Player::ToCatch(const Vector2 & t)
{
	_target		= t;
	_anim_frame = 0;
	_updater	= &Player::Predation;
}

void Player::OnDamage()
{
	_anim_frame = 0;
	_updater	= &Player::Die;
}
