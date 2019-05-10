#include <math.h>
#include "Fish.h"
#include "../Game.h"
#include "../Camera.h"

const int distance = 150;		/// �T�m�ł��鋗��
const int deg = 30;				/// ��ڲ԰��T�m������������߂邽�߂̊p�x

Fish::Fish(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(200, 200);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);
	enemy	 = EnemyInfo(pos, size, rect);
	_vel	 = Vector2();

	_turnFlag = false;
	enemy._dieFlag  = false;
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
	/// ��ʊO�ɏo�����A���S��Ԃɂ���
	if (enemy._pos.x + enemy._size.width / 2 < 0 ||
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
}

void Fish::DieUpdate()
{
	/// ��ʊO�ɏo�����A���S��Ԃɂ���
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

	auto sPos = Vector2(enemy._pos.x - (enemy._size.width / 2), enemy._pos.y - (enemy._size.height / 2));
	auto ePos = Vector2(enemy._pos.x + (enemy._size.width / 2), enemy._pos.y + (enemy._size.height / 2));

	DxLib::DrawBox(sPos.x - camera.x, sPos.y - camera.y,
				   ePos.x - camera.x, ePos.y - camera.y, color, (updater != &Fish::EscapeUpdate));

	/// �T�m�ł���͈͂̕`��
	for (int i = 0; i < enemy._searchVert.size(); ++i)
	{
		DxLib::DrawCircle(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y, 5, 0xff2222);

		DxLib::DrawLineAA(enemy._searchVert[i].x - camera.x, enemy._searchVert[i].y - camera.y,
						  enemy._searchVert[(i + 1) % 3].x - camera.x, enemy._searchVert[(i + 1) % 3].y - camera.y, 0x0000ff, 3.0);
	}
}

void Fish::Update()
{
	auto velX = _vel.x;
	auto screenX = Game::GetInstance().GetScreenSize().x;
	/// ���ޯ�ޗp�̃��[�v
	auto DebugRoop = [&velX, &screenX](const Position2& pos)
	{
		if (velX > 0)
		{
			(pos.x - 60 > screenX ? Position2(0, pos.y) : pos);
		}
		return (pos.x + 60 < 0 ? Position2(screenX, pos.y) : pos);
	};
	(this->*updater)();

	enemy._pos += _vel;
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
	/// ���Ă�ł��Ă��Ȃ��̂ŁA���������Ȃ�
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
