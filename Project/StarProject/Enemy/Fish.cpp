#include <math.h>
#include "Fish.h"
#include "../Camera.h"

const int distance = 150;		/// 探知できる距離
const int deg = 30;				/// ﾌﾟﾚｲﾔｰを探知する方向を求めるための角度

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);
	enemy	 = EnemyInfo(pos, size, rect);
	_vel	 = Vector2();

	_turnFlag = false;
	_dieFlag  = false;
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		enemy._searchVert[i] = enemy._pos;
	}

	color = 0x88ff88;

	Swim();
}


Fish::~Fish()
{
}

void Fish::Swim()
{
	_vel.x = (_turnFlag ? 2.f : -2.f);
	updater = &Fish::SwimUpdate;
}

void Fish::Die()
{
	_vel = Vector2(0, 0);
	_dieFlag = true;
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		enemy._searchVert[i] = enemy._pos;
	}
	updater = &Fish::DieUpdate;
}

void Fish::SwimUpdate()
{

}

void Fish::DieUpdate()
{
}

void Fish::searchMove()
{
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		if (i == 0)
		{
			enemy._searchVert[i] = _turnFlag ? Vector2(enemy._rect.Right(), enemy._pos.y) :
											   Vector2(enemy._rect.Left(),  enemy._pos.y);
		}
		else
		{
			double  rad, cosD, sinD;
			Vector2 pos;

			if (_turnFlag)
			{
				rad = (deg - ((deg * 2) * (i - 1))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._rect.Right(), enemy._pos.y);
			}
			else
			{
				rad = (180 - deg + ((deg * 2) * (i - 1))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._rect.Left(), enemy._pos.y);
			}
			enemy._searchVert[i] = Vector2(pos + Vector2(distance * cosD, -distance * sinD));
		}
	}
}

void Fish::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawBox(enemy._rect.Left() - camera.x, enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x, enemy._rect.Bottom() - camera.y, color, !_dieFlag);

	/// 探知できる範囲の描画
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		if (!_dieFlag)
		{
			DxLib::DrawCircle(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y, 5, 0xff2222);

			DxLib::DrawLineAA(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y,
				enemy._searchVert[(i + 1) % 3].x - camera.x, enemy._searchVert[(i + 1) % 3].y - camera.y, 0x0000ff, 3.0);
		}
	}
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
	if (updater != &Fish::DieUpdate)
	{
		searchMove();
	}
	
}

EnemyInfo Fish::GetInfo()
{
	return enemy;
}

shot_vector Fish::ShotGetInfo()
{
	return shot;
}

void Fish::ChangeShotColor(const int & num)
{
	/// ｼｮｯﾄを打っていないので、何も書かない
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

void Fish::CalTrackVel(const Vector2 & pos, bool col)
{
	if (updater != &Fish::DieUpdate)
	{
		if (col)
		{
			auto vec = pos - enemy._pos;
			vec.Normalize();
			_vel = Vector2(2.0f * vec.x, 2.0f * vec.y);
		}
		else
		{
			_vel.x = (_turnFlag ? 2.0 : -2.f);
			_vel.y = 0;
		}
	}
}
