#include <math.h>
#include "Fish.h"
#include "../Game.h"
#include "../Camera.h"
#include "../Player.h"

constexpr int Search_range	= 300;		// 探知できる距離
constexpr int Viewing_angle = 30;		// 視野角
constexpr int points		= 10;		// 中間点の個数
constexpr float Speed		= 3.0f;		// 移動速度

Fish::Fish(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos) :Enemy(c, p)
{
	auto size	= Size(150, 50);
	_enemy		= EnemyInfo(pos, size);

	_isTurn		= false;
	_vel		= _isTurn ? Vector2(1, 0) : Vector2(-1, 0);

	_anim_frame = 0;
	_escapeTime	= 0;

	_axis		= POS;
	
	/// 制御点の設定
	cPoints.emplace_back(POS + _vel.Normalized() * (SIZE.width / 4.0f), false);
	cPoints.emplace_back(POS - _vel.Normalized() * (SIZE.width / 4.0f), true);

	midPoints.resize(points);
	CalBezier();

	_updater = &Fish::SwimUpdate;
}

Fish::~Fish()
{
}


void Fish::SwimUpdate()
{
	auto tmp = _isTurn ? Vector2(1, 0) : Vector2(-1, 0);
	LookAt(tmp);

	POS += _vel.Normalized() * Speed;
	for (auto& cp : cPoints)
		cp._pos += (_vel.Normalized() * Speed);

	++_anim_frame;
	if (_anim_frame > 150)
	{
		_anim_frame = 0;
		_isTurn		= !_isTurn;
		_angle		= 2;
		_axis		= VCross(_vel.V_Cast(), VGet(0, 0, -1));
		_axis		= POS + _axis.Normalized() * 80;
		_updater	= &Fish::TurnUpdate;
	}

	Search();
}

void Fish::TurnUpdate()
{
	auto v = _vel.Normalized();
	_vel = VTransform(v.V_Cast(), MGetRotZ(DX_PI_F / 180.0f * _angle));

	MATRIX mat;

	mat = MGetTranslate(VScale(_axis.V_Cast(), -1));
	mat = MMult(mat, MGetRotZ(DX_PI_F / 180.0f * _angle));
	mat = MMult(mat, MGetTranslate(_axis.V_Cast()));

	POS = VTransform(POS.V_Cast(), mat);

	for (int i = 0; i < cPoints.size(); ++i)
	{
		cPoints[i]._pos = VTransform(cPoints[i]._pos.V_Cast(), mat);
	}

	++_anim_frame;
	if (_anim_frame >= (180 / _angle))
	{
		_anim_frame = 0;
		_updater = &Fish::SwimUpdate;
	}

	Search();
}

void Fish::TrackUpdate()
{
	POS += _vel.Normalized() * Speed;
	for (auto& cp : cPoints)
		cp._pos += (_vel.Normalized() * Speed);

	auto v = (_target + _vel.Normalized() * 50);
	if ((v - POS).Magnitude() < 5)
	{
		_updater = &Fish::SwimUpdate;
	}
}

void Fish::EscapeUpdate()
{
	POS += _vel.Normalized() * Speed * 2;
	for (auto& cp : cPoints)
		cp._pos += (_vel.Normalized() * Speed * 2);

	++_escapeTime;
	if (_escapeTime > 255)
		_updater = &Fish::DieUpdate;
}

void Fish::DieUpdate()
{
	ALIVE = false;
}

void Fish::CalBezier()
{
	/// 中間点の設定
	auto start	= POS + (_vel.Normalized()		* SIZE.width / 2);
	auto end	= POS + ((-_vel.Normalized())	* SIZE.width / 2);

	for (int m = 0; m < points; ++m)
	{
		float b = (float)m / points;
		float a = 1.0f - b;

		/// 3次ﾍﾞｼﾞｪの計算
		midPoints[m].x = (a * a * a * start.x) + (3 * a * a * b * cPoints[0]._pos.x) + 
						 (3 * a * b * b * cPoints[1]._pos.x) + (b * b * b * end.x);

		midPoints[m].y = (a * a * a * start.y) + (3 * a * a * b * cPoints[0]._pos.y) +
						 (3 * a * b * b * cPoints[1]._pos.y) + (b * b * b * end.y);
	}

	CreateDamagePoints(); 
	CreateAttackPoints();
}

void Fish::LookAt(const Vector2& v)
{
	MATRIX rot;
	rot		= MGetRotVec2(_vel.V_Cast(), v.V_Cast());
	_vel	= VTransform(_vel.V_Cast(), rot);

	MATRIX mat;

	mat = MGetTranslate(VScale(POS.V_Cast(), -1));
	mat = MMult(mat, rot);
	mat = MMult(mat, MGetTranslate(POS.V_Cast()));

	POS = VTransform(POS.V_Cast(), mat);

	for (int i = 0; i < cPoints.size(); ++i)
	{
		cPoints[i]._pos = VTransform(cPoints[i]._pos.V_Cast(), mat);
	}
}

void Fish::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - _escapeTime);

	/// ﾍﾞｼﾞｪ曲線を用いての描画
	Vector2 p1, p2, p3, p4;
	for (int i = 1; i < midPoints.size(); ++i)
	{
		auto t = abs(((i + 6) % 10) - 5) * 4 + 2;

		Vector2 v; 
		v	= VCross(_vel.V_Cast(), VGet(0, 0, -1));
		p1	= midPoints[i - 1]	+ v.Normalized() * t - CC;
		p2	= midPoints[i]		+ v.Normalized() * t - CC;

		v	= VCross(VGet(0, 0, -1), _vel.V_Cast());
		p3	= midPoints[i]		+ v.Normalized() * t - CC;
		p4	= midPoints[i - 1]	+ v.Normalized() * t - CC;

		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0x808080, true);

		if (i > 1 && i < 8)
			DrawLine(midPoints[i - 1].x - CC.x, midPoints[i - 1].y - CC.y,
				midPoints[i].x - CC.x, midPoints[i].y - CC.y, 0xC0C0C0, 2);
		if (i == 2)
		{
			DrawLine(p1.x, p1.y, p2.x, p2.y, 0, 3);
			DrawLine(p3.x, p3.y, p4.x, p4.y, 0, 3);
		}
	}
	auto start	= POS + (_vel.Normalized() * SIZE.width / 2);
	auto end	= POS + ((-_vel.Normalized()) * SIZE.width / 2);

	DrawLine(midPoints[9].x - CC.x, midPoints[9].y - CC.y,
		end.x - CC.x, end.y - CC.y, 0x808080, 5);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DebugDraw();
#endif

}

void Fish::DebugDraw()
{
	auto start	= POS + (_vel.Normalized() * SIZE.width / 2);
	auto end	= POS + ((-_vel.Normalized()) * SIZE.width / 2);

	DrawCircle(start.x - CC.x, start.y - CC.y, 5, 0x00ff00);
	DrawCircle(end.x - CC.x, end.y - CC.y, 5, 0xffff00);

	auto vp = start + _vel.Normalized() * 100;
	DrawLine(start.x - CC.x, start.y - CC.y, vp.x - CC.x, vp.y - CC.y, 0, 5);

	DrawCircle(POS.x - CC.x,	POS.y - CC.y,	5, 0x000000);
	DrawCircle(_axis.x - CC.x,	_axis.y - CC.y, 5, 0x000000);
	for (auto& p : cPoints)
	{
		DrawCircle(p._pos.x - CC.x, p._pos.y - CC.y, 5, 0x0000ff);

		auto vp = start + (-_vel.Normalized()) * Dot((-_vel.Normalized()), (p._pos - start));
		DrawCircle(vp.x - CC.x, vp.y - CC.y, 5, 0x0000ff);
	}

	for (auto& d : _damage)
		DrawCircle(d.pos.x, d.pos.y, d.r, 0xff00ff, true);
}

void Fish::Update()
{
	(this->*_updater)();

	auto start	= POS + (_vel.Normalized() * SIZE.width / 2);
	auto end	= POS + ((-_vel.Normalized()) * SIZE.width / 2);

	/// 制御点の移動
	for (auto& cp : cPoints)
	{
		Vector2 v;
		v = VCross((_isTurn ? VGet(0, 0, 1) : VGet(0, 0, -1)), _vel.V_Cast());
		v = v.Normalized() * (cp._isTurn ? 2 : -2);
		cp._pos += v;

		auto p = start + (-_vel.Normalized()) * Dot((-_vel.Normalized()), (cp._pos - start));
		if ((cp._pos - p).Magnitude() > 30)
		{
			cp._isTurn = !cp._isTurn;
			cp._pos = p + (cp._pos - p).Normalized() * 30.0f;
		}
	}

	CalBezier();
}

void Fish::Search()
{
	_target = _player->GetInfo().center;
	
	auto v1 = _vel;
	auto v2 = _target - POS;

	if (v2.Magnitude() < Search_range)
	{
		auto theta = Dot(v1, v2) / (v1.Magnitude() * v2.Magnitude());

		if (acos(theta) < (DX_PI_F / 180.0f * Viewing_angle))
		{
			LookAt(v2);

			_updater = &Fish::TrackUpdate;
		}
	}
}

void Fish::OnDamage()
{
	_target = _player->GetInfo().center;

	if (!DAMAGE)
	{
		auto v1 = _vel;
		auto v2 = POS - _target;
		v2 += v1;

		LookAt(v2);

		DAMAGE = true;

		_updater = &Fish::EscapeUpdate;
	}
}

void Fish::CreateDamagePoints()
{
	_damage.clear();

	_damage.emplace_back(POS, SIZE.height / 3);
	_damage.emplace_back(midPoints[midPoints.size() / 4], SIZE.height / 3);
	_damage.emplace_back(midPoints[midPoints.size() - midPoints.size() / 4], SIZE.height / 4);
}

void Fish::CreateAttackPoints()
{
	_attack.clear();

	_attack.emplace_back(POS, SIZE.height / 3);
	_attack.emplace_back(midPoints[midPoints.size() / 4], SIZE.height / 3);
	_attack.emplace_back(midPoints[midPoints.size() - midPoints.size() / 4], SIZE.height / 4);
}
