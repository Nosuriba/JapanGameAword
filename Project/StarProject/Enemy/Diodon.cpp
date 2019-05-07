#include "Diodon.h"
#include "../Camera.h"

const float riseSpeed = 0.8f;
const float maxSpeed  = 3.0f;
const Size swellSize  = Size(90, 90);

Diodon::Diodon(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(300, 300);
	auto size = Size(80, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel  = Vector2();

	color = 0x77bbff;

	riseCnt = 0;
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
	_turnFlag = true;
	updater = &Diodon::SwellUpdate;
}

void Diodon::SwimUpdate()
{
	/// Ç∆ÇËÇ†Ç¶Ç∏ÅAç∂âEÇ…ìÆÇ©ÇµÇƒÇ¢ÇÈÅB
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

		/// ïùÇÃägèk
		if (size.width != swell.width)
		{
			size.width = (size.width > swell.width ? size.width - 1 : size.width + 1);
		}

		/// çÇÇ≥ÇÃägèk
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

void Diodon::Draw()
{
	auto camera = _camera->CameraCorrection();

	DxLib::DrawBox(enemy._rect.Left() - camera.x,  enemy._rect.Top() - camera.y,
				   enemy._rect.Right() - camera.x , enemy._rect.Bottom() - camera.y, color, true);

	DxLib::DrawString(0, 40, " ÿæ›Œﬁ›ÇÃï\é¶", 0x000000);
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
	color = 0x77bbff;
}