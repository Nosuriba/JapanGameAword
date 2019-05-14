#include <math.h>
#include "Fish.h"
#include "../Game.h"
#include "../Camera.h"

const int distance = 150;		/// 探知できる距離
const int deg = 30;				/// ﾌﾟﾚｲﾔｰを探知する方向を求めるための角度
const int mPoint = 10;

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
	
	/// 制御点の設定
	CtlInfo sPoint, ePoint;
	sPoint.vel = Vector2(0, 0);
	ePoint.vel = Vector2(0, 0);
	sPoint.pos = Vector2(enemy._pos.x - enemy._size.width / 2, enemy._pos.y);
	ePoint.pos = Vector2(enemy._pos.x + enemy._size.width / 2, enemy._pos.y);
	sPoint.flag = false;
	ePoint.flag = true;

	ctlPoint.push_back(sPoint);
	ctlPoint.push_back(ePoint);
	sPoints.push_back(Vector2(sPoint.pos.x - enemy._size.width, sPoint.pos.y));
	sPoints.push_back(Vector2(ePoint.pos.x + enemy._size.width, ePoint.pos.y));

	dCtlPos.resize(ctlPoint.size());
	
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
	for (int i = 0; i < ctlPoint.size(); ++i)
	{
		ctlPoint[i].vel.y += (ctlPoint[i].flag ? 0.2f : -0.2f);

		if (ctlPoint[i].flag)
		{
			ctlPoint[i].flag = (ctlPoint[i].vel.y > 2.0f ? false : true);
		}
		else
		{
			ctlPoint[i].flag = (ctlPoint[i].vel.y < -2.0f ? true : false);
		}
		ctlPoint[i].pos += ctlPoint[i].vel;
		dCtlPos[i].resize(mPoint);
		dCtlPos[i][0] = ctlPoint[i].pos;
		for (int m = 0; m < mPoint; ++m)
		{
			float b = (float)m / mPoint;
			float a = 1.0f - b;

			dCtlPos[i][m].x = (a * a * sPoints[i].x) + (2 * a * b * ctlPoint[i].pos.x) + (b * b * enemy._pos.x);
			dCtlPos[i][m].y = (a * a * sPoints[i].y) + (2 * a * b * ctlPoint[i].pos.y) + (b * b * enemy._pos.y);
		}
	}

}

void Fish::CalNormalVec()
{
	ctlVel.y += (ctlFlag ? 0.1f : -0.1f);
	if (ctlFlag)
	{
		ctlFlag = (ctlVel.y >= 1.0f ? false : true);
	}
	else
	{
		ctlFlag = (ctlVel.y <= -1.0f ? true : false);
	}

	divEnemy[0]._pos += ctlVel;
	for (int i = 1; i < divEnemy.size(); ++i)
	{
		auto sin = (float)(60 * DX_PI / 180.f);
		auto A = Vector2(divEnemy[i - 1]._pos.x + divEnemy[i - 1]._size.width / 2,
						 divEnemy[i - 1]._pos.y + divEnemy[i - 1]._size.height / 2);
		auto B = Vector2(divEnemy[i]._pos.x - divEnemy[i]._size.width / 2,
						 divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);
		auto C = Vector2(divEnemy[i]._pos.x + divEnemy[i]._size.width / 2,
						 divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);

		auto AB = B - A;
		auto BC = C - B;
		AB.Normalize();
		BC.Normalize();
		
		auto cross = Cross(AB, BC);
		divEnemy[i]._pos = Vector2(divEnemy[i]._pos.x/* * cross*//*+ (divEnemy[i]._size.width * cross)*/,
								   divEnemy[i]._pos.y * cross);

	}
}

void Fish::Draw()
{

	auto camera = _camera->CameraCorrection();

	auto sPos = Vector2(enemy._pos.x - (enemy._size.width / 2), enemy._pos.y - (enemy._size.height / 2));
	auto ePos = Vector2(enemy._pos.x + (enemy._size.width / 2), enemy._pos.y + (enemy._size.height / 2));
	/// 通常の描画
	DxLib::DrawBox(sPos.x - camera.x, sPos.y - camera.y,
					ePos.x - camera.x, ePos.y - camera.y, color, (updater != &Fish::EscapeUpdate));

	/// 画像の読み込み
	/*int sizeX = 0;
	int sizeY = 0;
	DxLib::GetImageSize_File("resource/fish.png", &sizeX, &sizeY);
	DxLib::LoadDivGraph("resource/fish.png", divEnemy.size(), divEnemy.size(), 1, 
						 sizeX / divEnemy.size(), sizeY, &divImage[0]);*/

	/*auto image = DxLib::LoadGraph("resource/abi.png", true);
	DrawModiGraph(100, 100, 300, 100, 200, 200, 100, 200, image, true);*/

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

	Vector2 pre = dCtlPos[1][0];
	float t = 40.f;
	for (auto& m : dCtlPos[1])
	{
		DxLib::DrawLineAA(pre.x - camera.x, pre.y - camera.y,
			m.x - camera.x, m.y - camera.y, 0xff0000, 20);
		pre.x = m.x;
		pre.y = m.y;
		t /= 1.3;
	}

	for (int i = 0; i < dCtlPos.size(); ++i)
	{
		for (int m = 0; m < dCtlPos[i].size(); ++m)
		{
			DxLib::DrawCircle(dCtlPos[i][m].x, dCtlPos[i][m].y, 5, (i == 0 ? 0xff0000 : 0xffff00), true);
		}
	}

	

	//for (int i = 1; i < divEnemy.size(); ++i)
	//{
	//	///// 画像の描画がおかしいので、後で必ず直す
	//	//auto p1 = Vector2(divEnemy[i]._pos.x - divEnemy[i]._size.width / 2 + (i * divEnemy[i]._size.width),
	//	//				  divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);		// 左上
	//	//auto p2 = Vector2(divEnemy[i]._pos.x + divEnemy[i]._size.width / 2 + (i * divEnemy[i]._size.width),
	//	//				  divEnemy[i]._pos.y - divEnemy[i]._size.height / 2);		// 右上
	//	//auto p3 = Vector2(divEnemy[i]._pos.x + divEnemy[i]._size.width / 2 + (i * divEnemy[i]._size.width),
	//	//				  divEnemy[i]._pos.y + divEnemy[i]._size.height / 2);		// 右下
	//	//auto p4 = Vector2(divEnemy[i]._pos.x - divEnemy[i]._size.width / 2 + (i * divEnemy[i]._size.width),
	//	//				  divEnemy[i]._pos.y + divEnemy[i]._size.height / 2);		// 左下


	//	DrawLine(divEnemy[i - 1]._pos.x, divEnemy[i - 1]._pos.y, divEnemy[i]._pos.x, divEnemy[i]._pos.y, 0xff2222, 2.0f);
	//	//DxLib::DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, divImage[i], true);

	//}

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

	CalCurve();
	//CalNormalVec();
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
