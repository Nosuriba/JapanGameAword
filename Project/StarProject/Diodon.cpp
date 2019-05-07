#include "Diodon.h"
#include "Camera.h"

const float maxSpeed = 3.0f;

Diodon::Diodon(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(300, 300);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel  = Vector2();

	color = 0x77bbff;

	_turnFlag = true;

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
	updater = &Diodon::SwellUpdate;
}

void Diodon::Die()
{
	updater = &Diodon::DieUpdate;
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
	/// 一定時間浮くと、上昇が止まるようにする
	_vel.y = -0.5f;
}

void Diodon::DieUpdate()
{
	/// たぶん死なない設定にすると思う
}

void Diodon::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawBox(enemy._rect.Left() - camera.x,  enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x , enemy._rect.Bottom() - camera.y, color, true);

	DxLib::DrawString(0, 40, "ﾊﾘｾﾝﾎﾞﾝの表示", 0x000000);
}

void Diodon::Update()
{
	(this->*updater)();

	enemy._pos += _vel;
	enemy._rect = Rect(enemy._pos, enemy._size);
}

EnemyInfo Diodon::GetInfo()
{
	return enemy;
}

void Diodon::ChangeColor()
{
	Swell();
	color = 0x5599dd;
}

void Diodon::ResetColor()
{
	/// debug用
	_vel.y = 0;
	Swim();
	color = 0x77bbff;
}