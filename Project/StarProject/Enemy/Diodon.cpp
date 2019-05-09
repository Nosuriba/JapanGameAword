#include "Diodon.h"
#include "../Camera.h"

const float riseSpeed = 0.8f;
const float maxSpeed  = 3.0f;
const Size swellSize  = Size(90, 90);

Diodon::Diodon(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(300, 300);
	auto size = Size(50, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel  = Vector2();

	riseCnt = 0;
	_turnFlag = true;
	color = 0x77bbff;

	for (int i = 0; i < _dirPos.size(); ++i)
	{
		if ((i % 4) == 0)
		{
			_dirPos[i] = (i == 0 ? Vector2(enemy._pos.x, enemy._rect.Top()) :
								   Vector2(enemy._pos.x, enemy._rect.Bottom()));
		}
		else
		{
			auto posX = enemy._pos.x + (enemy._rect.Width()  / 2 - (enemy._rect.Width() * (i / 4)));
			auto posY = enemy._pos.y + (enemy._rect.Height() / 2 * ((i - 1 + (i / 4 * 2)) % 3)) - enemy._rect.Height() / 2;

			_dirPos[i] = Vector2(posX, posY);
		}
	}

	Swim();
}

Diodon::~Diodon()
{
}

void Diodon::Swim()
{
	updater = &Diodon::SwimUpdate;
}

void Diodon::Swell()
{
	_vel.x = 0;
	_turnFlag = true;
	updater = &Diodon::SwellUpdate;
}

void Diodon::Shot()
{
	Vector2 vec, vel;
	Size size;
	Rect rect;

	for (int i = 0; i < _dirPos.size(); ++i)
	{
		/// ｻｲｽﾞと矩形の設定
		size = Size(10, 10);
		rect = Rect(_dirPos[i], size);

		/// 速度の設定
		vec = _dirPos[i] - enemy._pos;
		vec.Normalize();
		vel = Vector2(2.f * vec.x, 2.f * vec.y);

		shot.push_back(ShotInfo(_dirPos[i], vel, size, rect));
		shot[i].debugColor = 0xffffff;
	}

	updater = &Diodon::ShotUpdate;
}

void Diodon::Die()
{
}

void Diodon::SwimUpdate()
{
	/// とりあえず、左右に動かしている。
	if (_turnFlag)
	{
		_vel.x += 0.05f;
		_turnFlag = (_vel.x < maxSpeed ? true : false);
	}
	else
	{
		_vel.x -= 0.05f;
		_turnFlag = (_vel.x < -maxSpeed ? true : false);
	}
}

void Diodon::SwellUpdate()
{
	auto SizeScaling = [](const Size& enemy, const Size& swell)
	{
		auto size = enemy;

		/// 幅の拡縮
		if (size.width != swell.width)
		{
			size.width = (size.width > swell.width ? size.width - 1 : size.width + 1);
		}

		/// 高さの拡縮
		if (size.height != swell.height)
		{
			size.height = (size.height > swell.height ? size.height - 1 : size.height + 1);
		}
		return size;
	};

	riseCnt++;
	if (riseCnt > 180)
	{
		if (_turnFlag)
		{
			_vel.y += 0.02f;
			_turnFlag = (_vel.y <  riseSpeed? true : false);
		}
		else
		{
			_vel.y -= 0.02f;
			_turnFlag = (_vel.y < -riseSpeed ? true : false);
		}
	}
	else
	{
		_vel.y = -0.5f;
	}
	
	enemy._size = SizeScaling(enemy._size, swellSize);
}

void Diodon::ShotUpdate()
{
	for (int i = 0; i < shot.size(); ++i)
	{
		shot[i]._pos += shot[i]._vel;
		shot[i]._rect = Rect(shot[i]._pos, shot[i]._size);
	}
}

void Diodon::DieUpdate()
{
}


void Diodon::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawBox(enemy._rect.Left() - camera.x,  enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x , enemy._rect.Bottom() - camera.y, color, true);

	for (auto itr : shot)
	{
		DxLib::DrawBox(itr._rect.Left() - camera.x, itr._rect.Top() - camera.y,
					   itr._rect.Right() - camera.x, itr._rect.Bottom() - camera.y, itr.debugColor, true);
	}

}

void Diodon::Update()
{
	(this->*updater)();

	enemy._pos += _vel;
	enemy._rect = Rect(enemy._pos, enemy._size);

	/// ｼｮｯﾄを出す座標の更新
	for (int i = 0; i < _dirPos.size(); ++i)
	{
		_dirPos[i] += _vel;
	}
}

EnemyInfo Diodon::GetInfo()
{
	return enemy;
}

shot_vector Diodon::GetShotInfo()
{
	return shot;
}

void Diodon::ChangeShotColor(const int& num)
{
	shot[num].debugColor = 0xeeee00;
}

void Diodon::ChangeColor()
{
	if (updater != &Diodon::ShotUpdate)
	{
		_vel = Vector2(0, 0);
		Shot();
	}
	color = 0x5599dd;
}

void Diodon::ResetColor()
{
	color = 0x77bbff;
}

void Diodon::CalTrackVel(const Vector2 & pos, bool col)
{
	/// 特に必要ないので何も書かない
}
