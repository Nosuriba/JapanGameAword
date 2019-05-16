#include "SeaCucumber.h"
#include "../Game.h"
#include "../Camera.h"

const Size eSize	 = Size(120, 30);
const int points	 = 10;
const int moveInvCnt = 40;
const int crawlVel   = 1.5f;
const int decSpeeed  = 0.02f;			// å∏ë¨ë¨ìx

SeaCucumber::SeaCucumber(std::shared_ptr<Camera>& camera) : Enemy(camera), _camera(camera)
{
	auto pos = Vector2(800, 500);
	auto size = eSize;
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	enemy._prePos = enemy._pos;
	_vel = Vector2();
	_turnFlag = false;
	cPoint._pos = enemy._pos;
	cPoint._vel = Vector2();
	enemy._dieFlag = false;

	cPoint._pos.x += (_turnFlag ? -enemy._size.width / 2 : enemy._size.width / 2);
	color = 0xa0522d;

	Crawl();
}

SeaCucumber::~SeaCucumber()
{
}

void SeaCucumber::Crawl()
{
	cPoint._vel.x = (_turnFlag ? crawlVel : -crawlVel);
	moveCnt = moveInvCnt;
	_updater = &SeaCucumber::CrawlUpdate;
}

void SeaCucumber::Escape()
{
	auto camera = _camera->CameraCorrection();
	if (cPoint._vel.x == 0.f)
	{
		moveCnt = moveInvCnt / 4;
	}
	_vel.y = 0;
	_updater = &SeaCucumber::EscapeUpdate;
}

void SeaCucumber::Die()
{
	enemy._dieFlag = true;
	enemy._size = Size(0, 0);
	_vel = Vector2(0, 0);

	_updater = &SeaCucumber::DieUpdate;
}

void SeaCucumber::CrawlUpdate()
{
	if (_turnFlag)
	{
		if (_vel.x <= 0.f)
		{
			_vel.x = 0;
			if (moveCnt <= 0)
			{
				cPoint._vel.x = (_vel.x <= 0.f ? crawlVel : 0);
				moveCnt = (cPoint._vel.x == crawlVel ? moveInvCnt : 0);
			}
		}
		else
		{
			_vel.x -=  0.02f;
		}
	}
	else
	{
		if (_vel.x >= 0.f)
		{
			_vel.x = 0;
			if (moveCnt <= 0)
			{
				cPoint._vel.x = (_vel.x >= 0.f ? -crawlVel : 0);
				moveCnt = (cPoint._vel.x == -crawlVel ? moveInvCnt : 0);
			}
		}
		else
		{
			_vel.x += 0.02f;
		}
	}
}

void SeaCucumber::EscapeUpdate()
{
	if (_turnFlag)
	{
		if (_vel.x <= 0.f)
		{
			_vel.x = 0;
			moveCnt--;
			if (moveCnt == 0)
			{
				cPoint._vel.x = crawlVel;
			}
		}
		else
		{
			_vel.x -= 0.04f;
		}
	}
	else
	{
		if (_vel.x >= 0.f)
		{
			_vel.x = 0;
			moveCnt--;
			if (moveCnt == 0)
			{
				cPoint._vel.x = -crawlVel;
			}
		}
		else
		{
			_vel.x += 0.04f;
		}
	}

	if (enemy._pos.x + enemy._size.width / 2 < 0 ||
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
}

void SeaCucumber::DieUpdate()
{
}

void SeaCucumber::MovePoint()
{
	if (_turnFlag)
	{
		if (cPoint._vel.x <= 0.f)
		{
			cPoint._vel.x = 0;
			moveCnt--;
			if (moveCnt == 0)
			{
				_vel.x = crawlVel;
			}
		}
		else
		{
			cPoint._vel.x -= 0.02f;
		}
	}
	else
	{
		if (cPoint._vel.x >= 0.f)
		{
			cPoint._vel.x = 0;
			moveCnt--;
			if (moveCnt == 0)
			{
				_vel.x = -crawlVel;
			}
		}
		else
		{
			cPoint._vel.x += 0.02f;
		}
	}
	cPoint._pos += cPoint._vel;
}

void SeaCucumber::Draw()
{
	auto camera = _camera->CameraCorrection();
	color = (_updater == &SeaCucumber::EscapeUpdate ? 0x80300b : 0xa0522d);


	/// ìGÇÃï`âÊ
	DxLib::DrawOval(enemy._pos.x - camera.x, enemy._pos.y - camera.y, enemy._size.width / 2, enemy._size.height / 2, color, true);

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void SeaCucumber::DebugDraw(const Vector2 & camera)
{ 
	/// ìñÇΩÇËîªíËÇÃï`âÊ
	DrawBox(enemy._rect.Left()  - camera.x, enemy._rect.Top()	 - camera.y,
			enemy._rect.Right() - camera.x, enemy._rect.Bottom() - camera.y, 0xff0000, false);

	/// ìGÇÃíÜêSì_ï`âÊ
	DrawCircle(enemy._pos.x - camera.x, enemy._pos.y - camera.y, 4, 0x00ffff, true);

	/// êßå‰ì_ÇÃï`âÊ
	DrawCircle(cPoint._pos.x - camera.x, cPoint._pos.y - camera.y, 4, 0xffff00, true);
	DrawFormatString(0, 0, 0x000000, "width : %d", enemy._size.width);
}

void SeaCucumber::Update()
{
	MovePoint();
	(this->*_updater)();

	enemy._prePos = enemy._pos;
	enemy._pos += _vel;

	if (_updater == &SeaCucumber::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._size.width = enemy._size.width / 2 + abs(enemy._pos.x - cPoint._pos.x);
		enemy._rect = Rect(enemy._pos, size);
	}
	else
	{
		enemy._size.width = enemy._size.width / 2 + abs(enemy._pos.x - cPoint._pos.x);
		enemy._rect = Rect(enemy._pos, enemy._size);
	}

	
}

EnemyInfo SeaCucumber::GetInfo()
{
	return enemy;
}

shot_vector SeaCucumber::GetShotInfo()
{
	return shot;
}

void SeaCucumber::CalEscapeDir(const Vector2 & vec)
{
	if (_updater != &SeaCucumber::EscapeUpdate && !enemy._dieFlag)
	{
		Escape();
	}
}

void SeaCucumber::ShotDelete(const int & num)
{
	/// ç°ÇÃÇ∆Ç±ÇÎÅAºÆØƒÇë≈Ç¬ó\íËÇÕÇ»Ç¢
}

void SeaCucumber::CalTrackVel(const Vector2 & pos, bool col)
{
	/// ÉvÉåÉCÉÑÅ[Çí«è]Ç∑ÇÈó\íËÇÕÇ»Ç¢
}
