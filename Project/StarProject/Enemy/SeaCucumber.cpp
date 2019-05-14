#include "SeaCucumber.h"
#include "../Game.h"
#include "../Camera.h"

const Size eSize = Size(100, 50);
const int moveInvCnt = 40;

SeaCucumber::SeaCucumber(std::shared_ptr<Camera>& camera) : Enemy(camera), _camera(camera)
{
	auto pos = Vector2(300, 300);
	auto size = eSize;
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel = Vector2();

	_turnFlag = false;
	enemy._dieFlag = false;
	color = 0xa0522d;

	Crawl();
}

SeaCucumber::~SeaCucumber()
{
}

void SeaCucumber::Crawl()
{
	_vel.x = (_turnFlag ? maxSpeed : -maxSpeed);
	moveCnt = moveInvCnt;
	_updater = &SeaCucumber::CrawlUpdate;
}

void SeaCucumber::Escape()
{
	_updater = &SeaCucumber::EscapeUpdate;
}

void SeaCucumber::Die()
{
	_updater = &SeaCucumber::DieUpdate;
}

void SeaCucumber::CrawlUpdate()
{
	if (_turnFlag)
	{
		if (_vel.x <= 0.f)
		{
			moveCnt--;
			_vel.x = (moveCnt < 0 ? maxSpeed : 0.f);
			moveCnt = (_vel.x == maxSpeed ? moveInvCnt : moveCnt);
		}
		else
		{
			_vel.x -= 0.1f;
		}
	}
	else
	{
		if (_vel.x >= 0.f)
		{
			moveCnt--;
			_vel.x = (moveCnt < 0 ? -maxSpeed : 0.f);
			moveCnt = (_vel.x == -maxSpeed ? moveInvCnt : moveCnt);
		}
		else
		{
			_vel.x += 0.1f;
		}
	}

	
}

void SeaCucumber::EscapeUpdate()
{
}

void SeaCucumber::DieUpdate()
{
}

void SeaCucumber::Draw()
{
	auto camera = _camera->CameraCorrection();

	

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void SeaCucumber::DebugDraw(const Vector2 & camera)
{
	/// ìñÇΩÇËîªíËÇÃï`âÊ
	DrawBox(enemy._rect.Left(),  enemy._rect.Top(),
			enemy._rect.Right(), enemy._rect.Bottom(), 0xff0000, false);
}

void SeaCucumber::Update()
{
	(this->*_updater)();

	enemy._pos += _vel;

	/// Ç∆ÇËÇ†Ç¶Ç∏âºê›íËÇ»ÇÃÇ≈ÅAå„Ç≈èCê≥ÇÇ∑ÇÈ
	if (_updater == &SeaCucumber::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._rect = Rect(enemy._pos, size);
	}
	else
	{
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

}

void SeaCucumber::ShotDelete(const int & num)
{
	/// ç°ÇÃÇ∆Ç±ÇÎÅAºÆØƒÇë≈Ç¬ó\íËÇÕÇ»Ç¢
}

void SeaCucumber::CalTrackVel(const Vector2 & pos, bool col)
{
	/// ÉvÉåÉCÉÑÅ[Çí«è]Ç∑ÇÈó\íËÇÕÇ»Ç¢
}
