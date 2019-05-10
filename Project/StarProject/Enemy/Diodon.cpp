#include "Diodon.h"
#include "../Game.h"
#include "../Camera.h"

const float riseSpeed = 0.8f;
const float maxSpeed  = 3.0f;
const Size swellSize  = Size(90, 90);

Diodon::Diodon(std::shared_ptr<Camera>& camera):Enemy(camera),_camera(camera)
{
	auto pos  = Vector2(600, 300);
	auto size = Size(50, 50);
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel  = Vector2();

	riseCnt = blastCnt = 0;
	_turnFlag = true;
	enemy._dieFlag  = false;
	color = 0x77bbff;

	/// ｼｮｯﾄを打つ位置の設定
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

	/// 直接的なｻｲｽﾞ変更で問題が出れば、処理を変える
	_vel = Vector2(0, 0);
	enemy._size = Size(0, 0);
	
	updater = &Diodon::ShotUpdate;
}

void Diodon::Escape()
{
	auto camera = _camera->CameraCorrection();
	/// 逃げていく方向の設定
	_vel.x = (enemy._pos.x < Game::GetInstance().GetScreenSize().x / 2 - camera.x ? -2.0f : 2.0f);
	_vel.y = 0;
	updater = &Diodon::EscapeUpdate;
}

void Diodon::Die()
{
	enemy._dieFlag = true;
	_vel		= Vector2(0, 0);
	enemy._size = Size(0, 0);

	updater  = &Diodon::DieUpdate;
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

		if (enemy._size.width >= swellSize.width &&
			enemy._size.height >= swellSize.height)
		{
			if (blastCnt <= 0)
			{
				Shot();
				_vel = Vector2(0, 0);
			}
			blastCnt--;
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
	auto debug = shot.size();
	for (int i = 0; i < shot.size(); ++i)
	{
		shot[i]._pos += shot[i]._vel;
		shot[i]._rect = Rect(shot[i]._pos, shot[i]._size);
	}

	if (CheckOutScreen())
	{
		Die();
	}
}

void Diodon::EscapeUpdate()
{
	/// 画面外に出た時、死亡状態にする
	if (enemy._pos.x + enemy._size.width / 2 < 0 || 
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
}

void Diodon::DieUpdate()
{

}

bool Diodon::CheckOutScreen()
{
	for (int i = 0; i < shot.size(); ++i)
	{
		/// とりあえず、画面外に行くとｼｮｯﾄを消すようにしている
		if (shot[i]._pos.x < 0 || shot[i]._pos.y < 0 ||
			shot[i]._pos.x > Game::GetInstance().GetScreenSize().x ||
			shot[i]._pos.y > Game::GetInstance().GetScreenSize().y)
		{
			shot.erase(shot.begin() + i);
		}
	}
	if (shot.size() <= 0)
	{
		return true;
	}
	return false;
}

void Diodon::Draw()
{
	auto camera = _camera->CameraCorrection();
	
	auto L = enemy._pos.x - (enemy._size.width / 2);
	auto T = enemy._pos.y - (enemy._size.height / 2);
	auto R = enemy._pos.x + (enemy._size.width / 2);
	auto B = enemy._pos.y + (enemy._size.height / 2);

	DxLib::DrawBox(L - camera.x,  T - camera.y,
				   R - camera.x , B - camera.y, color, (updater != &Diodon::EscapeUpdate));

	for (auto itr : shot)
	{
		DxLib::DrawBox(itr._rect.Left()  - camera.x, itr._rect.Top() - camera.y,
					   itr._rect.Right() - camera.x, itr._rect.Bottom() - camera.y, itr.debugColor, true);
	}

}

void Diodon::Update()
{
	(this->*updater)();

	enemy._pos += _vel;
	if (updater == &Diodon::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._rect = Rect(enemy._pos, size);
	}
	else
	{
		enemy._rect = Rect(enemy._pos, enemy._size);
	}
	

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
	if (!enemy._dieFlag)
	{
		/// 泳いでいる時
		if (updater == &Diodon::SwimUpdate)
		{
			Swell();
			blastCnt = 120;
			color = 0x5599dd;
			return;
		}

		/// 敵が膨らみきった状態の時
		if (updater == &Diodon::SwellUpdate &&
			enemy._size.width  == swellSize.width &&
			enemy._size.height == swellSize.height)
		{
			/// 敵が逃げていく処理の設定を書く
			Escape();
		}
	}
	
}

void Diodon::ResetColor()
{
	color = 0x77bbff;
}

void Diodon::CalTrackVel(const Vector2 & pos, bool col)
{
	/// 追従なし
}
