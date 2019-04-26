#include "Fish.h"
#include "Collision.h"
#include "Player.h"

Fish::Fish()
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel = Vector2();

	Swim();
}


Fish::~Fish()
{
}

void Fish::Swim()
{
	updater = &Fish::SwimUpdate;
}

void Fish::Die()
{
	updater = &Fish::DieUpdate;
}

void Fish::SwimUpdate()
{
	_vel.x = -2.f;
}

void Fish::DieUpdate()
{
}

void Fish::Draw()
{
	/// どれを操作しているかの確認で枠を塗りつぶしている
	DxLib::DrawBox(enemy._rect.Left(), enemy._rect.Top(),
				   enemy._rect.Right(), enemy._rect.Bottom(), color, true);

	DxLib::DrawString(0, 0, "魚の表示", 0xffffff);
}

void Fish::Update()
{
	/// ﾃﾞﾊﾞｯｸﾞ用のループ
	auto DebugRoop = [](const Position2& pos)
	{
		return (pos.x < 0 ? Position2(1280, pos.y) : pos);
	};
	(this->*updater)();

	enemy._pos += _vel;
	enemy._rect = Rect(enemy._pos, enemy._size);

	enemy._pos = DebugRoop(enemy._pos);
}

EnemyInfo Fish::GetInfo()
{
	return enemy;
}

void Fish::ChangeColor()
{
	color = 0xaaaaff;
}

void Fish::ResetColor()
{
	color = 0xffffff;
}
