#include "Diodon.h"
#include "../Game.h"
#include "../Camera.h"
#include "../Player.h"

constexpr	float	Speed = 2.0f;
constexpr	float	Angle = 1.0f;
constexpr	int		points = 10;			// 中間点の個数

constexpr float MAX_W = 120;
constexpr float MAX_H = 100;
constexpr float MIN_W = 70;
constexpr float MIN_H = 50;
constexpr float Boiling_Speed = 5;		//膨らむスピード

Diodon::Diodon(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos) :Enemy(c, p)
{
	auto size	= Size(MIN_W, MIN_H);
	_enemy		= EnemyInfo(pos, size);

	_isTurn		= false;
	_vel		= _isTurn ? Vector2(1, 0) : Vector2(-1, 0);

	_escapeTime = 0;

	_updater = &Diodon::SwimUpdate;
}

Diodon::~Diodon()
{
}

void Diodon::SwimUpdate()
{
	Move();
	Search();

	++_anim_frame;
	if (_anim_frame % 8 == 0)
	{
		SIZE.width	= max(MIN_W, SIZE.width		- Boiling_Speed);
		SIZE.height	= max(MIN_H, SIZE.height	- Boiling_Speed);
	}
}

void Diodon::SwellUpdate()
{
	Move();
	Search();

	++_anim_frame;
	if (_anim_frame % 8 == 0)
	{
		SIZE.width	= min(MAX_W, SIZE.width		+ Boiling_Speed);
		SIZE.height	= min(MAX_H, SIZE.height	+ Boiling_Speed);
	}
}

void Diodon::EscapeUpdate()
{
	++_escapeTime;
	++_anim_frame;

	if (_anim_frame % 5 == 0)
	{
		SIZE.width	= max(10, SIZE.width	- Boiling_Speed);
		SIZE.height = max(10, SIZE.height	- Boiling_Speed);
	}

	if (_escapeTime > 255)
		_updater = &Diodon::DieUpdate;
}

void Diodon::DieUpdate()
{
	ALIVE = false;
}

void Diodon::Move()
{
	Vector2 axis;
	axis = VCross(_vel.V_Cast(), VGet(0, 0, 1));
	axis = POS + axis.Normalized() * 150;

	MATRIX rot;
	rot = MGetRotZ(DX_PI_F / 180.0f * (_isTurn ? Angle : -Angle));

	auto v = _vel.Normalized();
	_vel = VTransform(v.V_Cast(), rot);

	MATRIX mat;

	mat = MGetTranslate(VScale(axis.V_Cast(), -1));
	mat = MMult(mat, rot);
	mat = MMult(mat, MGetTranslate(axis.V_Cast()));

	POS = VTransform(POS.V_Cast(), mat);
}

void Diodon::Draw()
{
	auto start	= POS + (_vel.Normalized()		* SIZE.width / 2);
	auto end	= POS + ((-_vel.Normalized())	* SIZE.width / 2);

	//// 中間点の設定
	std::vector<Vector2> midPoints;
	midPoints.resize(points);
	for (int m = 0; m < midPoints.size(); ++m)
		midPoints[m] = start + (-_vel.Normalized()) * (SIZE.width / points) * (m + 0.5f);

	// 描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - _escapeTime);

	DrawCircle(POS.x - CC.x, POS.y - CC.y, SIZE.width / 2, 0xCC9933, true);

	Vector2 p1, p2, p3, p4;
	for (int i = 1; i < midPoints.size(); ++i)
	{
		auto t = max(2, SIZE.height / 2 - abs((i % 10) - 5) * 6);

		Vector2 v;
		v = VCross(_vel.V_Cast(), VGet(0, 0, -1));
		p1 = midPoints[i - 1] + v.Normalized() * t;
		p2 = midPoints[i] + v.Normalized() * t;

		v = VCross(VGet(0, 0, -1), _vel.V_Cast());
		p3 = midPoints[i] + v.Normalized() * t;
		p4 = midPoints[i - 1] + v.Normalized() * t;

		if (i % 2)
			DxLib::DrawQuadrangleAA(p1.x - CC.x, p1.y - CC.y, p2.x - CC.x, p2.y - CC.y, p3.x - CC.x, p3.y - CC.y, p4.x - CC.x, p4.y - CC.y, 0xCC9933, true);
		else
			DxLib::DrawQuadrangleAA(p1.x - CC.x, p1.y - CC.y, p2.x - CC.x, p2.y - CC.y, p3.x - CC.x, p3.y - CC.y, p4.x - CC.x, p4.y - CC.y, 0x550000, true);

		DrawNeedle(midPoints[i - 1], midPoints[i - 1] - POS, SIZE.height / 4);
		DrawNeedle(p1, p1 - POS, SIZE.height / 4);
		DrawNeedle(p4, p4 - POS, SIZE.height / 4);

		v = VCross(_vel.V_Cast(), VGet(0, 0, -1));
		DrawNeedle(midPoints[i - 1] + v.Normalized() * (t / 2), (midPoints[i - 1] + v.Normalized() * (t / 2)) - POS, SIZE.height / 4);
		v = VCross(VGet(0, 0, -1), _vel.V_Cast());
		DrawNeedle(midPoints[i - 1] + v.Normalized() * (t / 2), (midPoints[i - 1] + v.Normalized() * (t / 2)) - POS, SIZE.height / 4);
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	//DebugDraw();
#endif
}

void Diodon::DebugDraw()
{
	// 中心
	DxLib::DrawCircle(POS.x - CC.x, POS.y - CC.y, 2, 0x000000);
}

void Diodon::DrawNeedle(const Vector2& p, const Vector2& v, const float r)
{
	Vector2 p1, p2, p3, vc;

	p1 = p + v.Normalized() * r;

	vc = VCross(v.V_Cast(), VGet(0, 0, 1));
	p2 = p + vc.Normalized() * (r / 10);

	vc = VCross(v.V_Cast(), VGet(0, 0, -1));
	p3 = p + vc.Normalized() * (r / 10);

	DrawTriangle(p1.x - CC.x, p1.y - CC.y, p2.x - CC.x, p2.y - CC.y, p3.x - CC.x, p3.y - CC.y, 0xffffff, true);
}

void Diodon::Update()
{
	(this->*_updater)();

}

void Diodon::Search()
{
	auto p = _player->GetInfo().center;
	if ((p - POS).Magnitude() < 300)
	{
		if (_updater != &Diodon::SwellUpdate)
			_anim_frame = 0;
		_updater	= &Diodon::SwellUpdate;
	}
	else
	{
		if (_updater != &Diodon::SwimUpdate)
			_anim_frame = 0;
		_updater	= &Diodon::SwimUpdate;
	}
}

void Diodon::OnDamage()
{
	if (!DAMAGE)
	{
		DAMAGE		= true;
		_escapeTime = 0;
		_anim_frame = 0;
		_updater	= &Diodon::EscapeUpdate;
	}
}
