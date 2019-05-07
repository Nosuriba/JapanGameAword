#include "Fish.h"
#include "../Processing/Collision.h"
#include "../Player.h"
#include "../Camera.h"

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel = Vector2();

	color = 0x88ff88;

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
	auto camera = _camera->CameraCorrection();

	/// どれを操作しているかの確認で枠を塗りつぶしている
	DxLib::DrawBox(enemy._rect.Left() - camera.x, enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x, enemy._rect.Bottom() - camera.y, color, true);

	DxLib::DrawString(0, 0, "魚の表示", 0x000000);
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
	color = 0x66dd66;
}

void Fish::ResetColor()
{
	color = 0x88ff88;
}