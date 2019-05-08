#include <math.h>
#include "Fish.h"
#include "../Processing/Collision.h"
#include "../Player.h"
#include "../Camera.h"

const int distance = 150;		/// 探知できる距離
const int deg = 30;				/// ﾌﾟﾚｲﾔｰを探知する方向を求めるための角度

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel = Vector2();

	for (int i = 0; i < 2; ++i)
	{
		auto rad = (deg - ((deg * 2) * i)) * DX_PI / 180;	
		auto cosD = cos(rad);
		auto sinD = sin(rad);

		auto pos = Vector2(enemy._rect.Left(), enemy._rect.Top() + (enemy._size.height / 2));
		searchPos.push_back(enemy._pos + Vector2(distance * cosD, -distance * sinD));
	}

	color = 0x88ff88;

	Swim();
}


Fish::~Fish()
{
}

void Fish::Swim()
{
	_vel.x = -2.f;
	updater = &Fish::SwimUpdate;
}

void Fish::Die()
{
	_vel = Vector2(0, 0);
	updater = &Fish::DieUpdate;
}

void Fish::SwimUpdate()
{

}

void Fish::DieUpdate()
{
}

void Fish::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawBox(enemy._rect.Left() - camera.x, enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x, enemy._rect.Bottom() - camera.y, color, true);

	/// 探知できる範囲の描画
	for (int i = 0; i < searchPos.size(); ++i)
	{
		DxLib::DrawLineAA(enemy._rect.Left() - camera.x, enemy._pos.y - camera.y,
					      searchPos[i].x - camera.x, searchPos[i].y - camera.y, 0x0000ff, 3.0);
	}
	DxLib::DrawLineAA(searchPos[0].x - camera.x, searchPos[0].y - camera.y,
					  searchPos[1].x - camera.x, searchPos[1].y - camera.y, 0x0000ff, 3.0);
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

	/// 探知する距離の終点座標を移動している
	for (int i = 0; i < searchPos.size(); ++i)
	{
		auto rad = (deg - ((deg * 2) * i)) * DX_PI / 180;
		auto cosD = cos(rad);
		auto sinD = sin(rad);

		auto pos = Vector2(enemy._rect.Left(), enemy._rect.Top() + (enemy._size.height / 2));
		searchPos[i] = Vector2(pos - Vector2(distance * cosD, distance * sinD));
	}
}

EnemyInfo Fish::GetInfo()
{
	return enemy;
}

void Fish::ChangeColor()
{
	Die();
	color = 0x66dd66;
}

void Fish::ResetColor()
{
	color = 0x88ff88;
}