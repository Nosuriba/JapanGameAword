#include <math.h>
#include "Fish.h"
#include "../Game.h"
#include "../Camera.h"

const int distance = 150;		/// 探知できる距離
const int deg = 30;				/// ﾌﾟﾚｲﾔｰを探知する方向を求めるための角度

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = Size(90, 50);
	auto rect = Rect(pos, size);
	enemy	 = EnemyInfo(pos, size, rect);
	_vel	 = Vector2();

	ctlPos = Vector2(enemy._rect.Left(), enemy._pos.y);
	ctlVel = Vector2();
	ctlFlag = false;
	_turnFlag = false;
	enemy._dieFlag  = false;

	/// 分割した座標の指定
	for (int i = 0; i < divEnemy.size(); ++i)
	{
		divEnemy[i]._size = Size(enemy._size.width / divEnemy.size(), enemy._size.height);
		divEnemy[i]._pos = enemy._pos + Vector2(i * (enemy._size.width / divEnemy.size()), enemy._size.height);
		divEnemy[i]._rect = Rect(divEnemy[i]._pos, divEnemy[i]._size);
	}

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
	/// 画面外に出た時、死亡状態にする
	if (enemy._pos.x + enemy._size.width / 2 < 0 ||
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
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
			float  rad, cosD, sinD;
			Vector2 pos;

			if (_turnFlag)
			{
				rad = (deg - ((deg * 2) * (i))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._rect.Right(), enemy._pos.y);
			}
			else
			{
				rad = (180 - deg + ((deg * 2) * (i))) * DX_PI / 180;
				cosD = cos(rad);
				sinD = sin(rad);

				pos = Vector2(enemy._rect.Left(), enemy._pos.y);
			}
			enemy._searchVert[i] = Vector2(pos + Vector2(distance * cosD, -distance * sinD));
		}
	}
}

void Fish::CalCurve()
{
	//// 敵の速度をｺﾒﾝﾄｱｳﾄしているので、後で直す
	ctlVel.y += (ctlFlag ? 0.2f : -0.2f);
	if (ctlFlag)
	{
		ctlFlag = (ctlVel.y > 2.0f ? false : true);
	}
	else
	{
		ctlFlag = (ctlVel.y < -2.0f ? true : false);
	}
	ctlPos += ctlVel;
	

	/// ﾍﾞｼﾞｪ曲線の計算
	midPos.resize(mPoint);
	midPos[0] = Vector2(enemy._rect.Left(), enemy._rect.Top());
	for (int i = 0; i < mPoint; ++i)
	{
		float b = (float)i / mPoint;
		float a = 1.0f - b;

		midPos[i].x = (a * a * enemy._rect.Left()) + (2 * a * b * ctlPos.x) + (b * b * enemy._rect.Right());
		midPos[i].y = (a * a * enemy._rect.Top())  + (2 * a * b * ctlPos.y) + (b * b * enemy._rect.Bottom());
	}
}

void Fish::CalNormalVec()
{
	ctlVel.y += (ctlFlag ? 0.2f : -0.2f);
	if (ctlFlag)
	{
		ctlFlag = (ctlVel.y > 2.0f ? false : true);
	}
	else
	{
		ctlFlag = (ctlVel.y < -2.0f ? true : false);
	}

	divEnemy[0]._pos += ctlVel;
	for (int i = 1; i < divEnemy.size(); ++i)
	{
		auto A = Vector2(divEnemy[i]._pos.x + divEnemy[i]._rect.Right(),
						 divEnemy[i]._pos.y + divEnemy[i]._rect.Bottom());
		auto B = Vector2(divEnemy[i]._pos.x + divEnemy[i]._rect.Left(), divEnemy[i]._pos.y + divEnemy[i]._rect.Top());
		auto C = Vector2(divEnemy[i]._pos.x + divEnemy[i]._rect.Right(), divEnemy[i]._pos.y + divEnemy[i]._rect.Top());

		auto AB = B - A;
		auto BC = C - B;
		AB.Normalize();
		BC.Normalize();
		
		auto cross = Cross(AB, BC);
		divEnemy[i]._pos = Vector2(divEnemy[i]._pos.x + (enemy._size.width * cross),
								   divEnemy[i]._pos.y + (enemy._size.height * cross));

	}
}

void Fish::Draw()
{

	auto camera = _camera->CameraCorrection();

	auto sPos = Vector2(enemy._pos.x - (enemy._size.width / 2), enemy._pos.y - (enemy._size.height / 2));
	auto ePos = Vector2(enemy._pos.x + (enemy._size.width / 2), enemy._pos.y + (enemy._size.height / 2));
	/// 通常の描画
	/*DxLib::DrawBox(sPos.x - camera.x, sPos.y - camera.y,
					ePos.x - camera.x, ePos.y - camera.y, color, (updater != &Fish::EscapeUpdate));*/

	/// 画像の読み込み
	int sizeX = 0;
	int sizeY = 0;
	DxLib::GetImageSize_File("resource/fish.png", &sizeX, &sizeY);
	DxLib::LoadDivGraph("resource/fish.png", divEnemy.size(), divEnemy.size(), 1, 
						 sizeX / divEnemy.size(), sizeY, &divImage[0]);

	/// ﾍﾞｼﾞｪ曲線を用いての描画
	/*Vector2 pre = midPos[0];		/// 前の座標
	float t = 40.0f;
	for (auto& m : midPos)
	{
		DxLib::DrawLineAA(pre.x - camera.x, pre.y - camera.y,
					     m.x - camera.x, m.y - camera.y, color, 20);
		pre.x = m.x;
		pre.y = m.y;
		t /= 1.3;
	}*/



	for (int i = 0; i < divEnemy.size(); ++i)
	{
		auto p1 = Vector2(divEnemy[i]._pos.x - divEnemy[i]._size.width / 2, divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);		// 左上
		auto p2 = Vector2(divEnemy[i]._pos.x + divEnemy[i]._size.width / 2, divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);		// 右上
		auto p3 = Vector2(divEnemy[i]._pos.x + divEnemy[i]._size.width / 2, divEnemy[i]._pos.y + divEnemy[i]._size.height / 2);		// 右下
		auto p4 = Vector2(divEnemy[i]._pos.x - divEnemy[i]._size.width / 2, divEnemy[i]._pos.y + divEnemy[i]._size.height / 2);		// 左下
		DxLib::DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, divImage[i], true);

	}

	/// 探知できる範囲の描画
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		DxLib::DrawCircle(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y, 5, 0xff2222);

		DxLib::DrawLineAA(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y,
						  enemy._searchVert[(i + 1) % 3].x - camera.x, enemy._searchVert[(i + 1) % 3].y - camera.y, 0x0000ff, 3.0);
	}
}

void Fish::Update()
{
	/// ﾃﾞﾊﾞｯｸﾞ用のループ
	auto velX = _vel.x;
	auto screenX = Game::GetInstance().GetScreenSize().x;
	auto DebugRoop = [&velX, &screenX](const Position2& pos)
	{
		if (velX > 0)
		{
			(pos.x - 60 > screenX ? Position2(0, pos.y) : pos);
		}
		return (pos.x + 60 < 0 ? Position2(screenX, pos.y) : pos);
	};
	(this->*updater)();

	//enemy._pos += _vel;
	if (updater == &Fish::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._rect = Rect(enemy._pos, size);
	}
	else
	{
		enemy._rect = Rect(enemy._pos, enemy._size);
		searchMove();
	}
	enemy._pos = DebugRoop(enemy._pos);

	//CalCurve();
	CalNormalVec();
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

void Fish::ChangeShotColor(const int & num)
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
