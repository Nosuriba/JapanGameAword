#include <math.h>
#include "Fish.h"
#include "../Game.h"
#include "../Camera.h"

const Size eSize = Size(90, 50);
const int distance = 150;		// 探知できる距離
const int deg	   = 30;		// ﾌﾟﾚｲﾔｰを探知する方向を求めるための角度
const int points   = 10;		// 制御点の個数
const float maxVel = 2.f;		// 制御点の最大速度

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = eSize;
	auto rect = Rect(pos, size);
	enemy	  = EnemyInfo(pos, size, rect);
	_vel	  = Vector2();

	_turnFlag		= false;
	enemy._dieFlag  = false;
	
	/// 制御点の設定
	CtlInfo cPoint[2];
	cPoint[0]._vel = Vector2(0, 0);
	cPoint[1]._vel = Vector2(0, -0.4);
	cPoint[0]._pos = Vector2(enemy._rect.Left()  + size.width / 3, enemy._pos.y + 20);
	cPoint[1]._pos = Vector2(enemy._rect.Right() - size.width / 3, enemy._pos.y - 20);
	cPoint[0]._flag = false;
	cPoint[1]._flag = true;

	for (auto& itr : cPoint)
	{
		cPoints.push_back(itr);
	}

	/// 敵を察知する範囲の頂点を設定
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
	_vel.x  = (_turnFlag ? maxSpeed : -maxSpeed);

	updater = &Fish::SwimUpdate;
}

void Fish::Escape()
{
	auto camera = _camera->CameraCorrection();
	_vel.x = (enemy._pos.x < Game::GetInstance().GetScreenSize().x / 2 - camera.x ? -maxSpeed : maxSpeed);
	_vel.y = 0;

	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		enemy._searchVert[i] = enemy._pos;
	}
	updater = &Fish::EscapeUpdate;

}

void Fish::Die()
{
	_vel = Vector2(0, 0);
	enemy._dieFlag = true;
	
	updater = &Fish::DieUpdate;
}

void Fish::SwimUpdate()
{
}

void Fish::EscapeUpdate()
{
	/// 画面外に出た時、死亡状態にする
	if (enemy._pos.x + enemy._size.width / 2 < 0 ||
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
}

void Fish::DieUpdate()
{
}

void Fish::SearchMove()
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
			float  rad, cosD, sinD;
			Vector2 pos;

			if (_turnFlag)
			{
				rad = (deg - (deg * (i / 2 * 2))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._pos.x + (enemy._size.width / 2), enemy._pos.y);
			}
			else
			{
				rad = (180 - deg + (deg * (i / 2 * 2))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._pos.x - (enemy._size.width / 2), enemy._pos.y);
			}
			enemy._searchVert[i] = Vector2(pos + Vector2(distance * cosD, -distance * sinD));
		}
	}
}

void Fish::CalBezier()
{
	/// 制御点の移動
	for (int i = 0; i < cPoints.size(); ++i)
	{
		if (cPoints[i]._flag)
		{
			cPoints[i]._flag = (cPoints[i]._vel.y >= 2.0f ? false : true);
		}
		else
		{
			cPoints[i]._flag = (cPoints[i]._vel.y <= -2.0f ? true : false);
		}
		cPoints[i]._vel.y += (cPoints[i]._flag ? 0.1f : -0.1f);
		cPoints[i]._pos += cPoints[i]._vel + _vel;
	}

	/// 中間点の設定
	midPoints.resize(points);
	midPoints[0] = enemy._pos;
	for (int m = 0; m < points; ++m)
	{
		float b = (float)m / points;
		float a = 1.0f - b;

		/// 3次ﾍﾞｼﾞｪの計算
		midPoints[m].x = (a * a * a * (enemy._pos.x - (eSize.width / 2))) + (3 * a * a * b * cPoints[0]._pos.x) + 
						 (3 * a * b * b * cPoints[1]._pos.x) + (b * b * b * (enemy._pos.x + (eSize.width / 2)));

		midPoints[m].y = (a * a * a * enemy._pos.y)			 + (3 * a * a * b * cPoints[0]._pos.y) +
						 (3 * a * b * b * cPoints[1]._pos.y) + (b * b * b * enemy._pos.y);
	}
}

void Fish::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// ﾍﾞｼﾞｪ曲線を用いての描画
	Vector2 p1, p2, p3, p4;
	auto height = enemy._size.height / 4 + 2;			/// 描画する高さの調整
	color = (updater == &Fish::EscapeUpdate ? 0xaa4444 : 0xff4444);
	for (int i = 1; i < midPoints.size(); ++i)
	{
		p1 = Vector2(midPoints[i - 1].x - camera.x, midPoints[i - 1].y - height - camera.y);
		p2 = Vector2(midPoints[i].x - camera.x, midPoints[i].y - height - camera.y);
		p3 = Vector2(midPoints[i].x - camera.x, midPoints[i].y + height - camera.y);
		p4 = Vector2(midPoints[i - 1].x - camera.x, midPoints[i - 1].y + height - camera.y);

		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color, true);
	}

#ifdef _DEBUG
	DebugDraw(camera);
#endif

}

void Fish::DebugDraw(const Vector2& camera)
{
	/// 当たり判定の描画
	DxLib::DrawBox(enemy._rect.Left() - camera.x, enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x, enemy._rect.Bottom() - camera.y, 0x00ff00, false);


	/// 探知できる範囲の描画
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		DxLib::DrawCircle(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y, 5, 0xff2222);

		DxLib::DrawLineAA(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y,
						  enemy._searchVert[(i + 1) % 3].x - camera.x, enemy._searchVert[(i + 1) % 3].y - camera.y, 0x0000ff, 2.0);
	}

	/// 制御点の描画(debug用)
	for (auto& itr : cPoints)
	{
		DxLib::DrawCircle(itr._pos.x - camera.x, itr._pos.y - camera.y, 5, 0x00ffff, true);
	}

}

void Fish::Update()
{
	/// debug用のループ
	auto velX = _vel.x;
	auto screenX = Game::GetInstance().GetScreenSize().x;
	auto DebugRoop = [&velX, &screenX](const Position2& pos)
	{
		if (velX > 0)
		{
			/// 右端を超えた時
			return (pos.x - 60 > screenX ? Position2(0, pos.y) : pos);
		}
		/// 左端を超えた時
		return (pos.x + 60 < 0 ? Position2(screenX, pos.y) : pos);
	};

	(this->*updater)();

	enemy._pos += _vel;
	enemy._pos = DebugRoop(enemy._pos);

	/// 死亡状態かの確認
	if (updater == &Fish::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._rect = Rect(enemy._pos, size);
		cPoints[0]._pos.x = enemy._pos.x - eSize.width / 2 + enemy._size.width / 3;
		cPoints[1]._pos.x = enemy._pos.x + eSize.width / 2 - enemy._size.width / 3;
	}
	else
	{
		enemy._rect = Rect(enemy._pos, enemy._size);
		SearchMove();
	}

	if (enemy._pos.x <= 0 || enemy._pos.x >= screenX)
	{
		cPoints[0]._pos.x = enemy._rect.Left()  + enemy._size.width / 3;
		cPoints[1]._pos.x = enemy._rect.Right() - enemy._size.width / 3;
	}

	CalBezier();
}

EnemyInfo Fish::GetInfo()
{
	return enemy;
}

shot_vector Fish::ShotGetInfo()
{
	return shot;
}

void Fish::CalEscapeDir(const Vector2 & vec)
{
	if (updater != &Fish::EscapeUpdate && !enemy._dieFlag)
	{
		Escape();
	}
}

void Fish::ShotDelete(const int & num)
{
	/// ｼｮｯﾄを打っていないので、何も書かない
}

void Fish::CalTrackVel(const Vector2 & pos, bool col)
{
	if (updater != &Fish::EscapeUpdate && !enemy._dieFlag)
	{
		if (col)
		{
			auto vec = pos - enemy._pos;
			vec.Normalize();
			_vel = Vector2(maxSpeed * vec.x, maxSpeed * vec.y);
		}
		else
		{
			_vel.x = (_turnFlag ? maxSpeed : -maxSpeed);
			_vel.y = 0;
		}
	}
}
