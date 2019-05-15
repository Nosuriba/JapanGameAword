#include "SeaCucumber.h"
#include "../Game.h"
#include "../Camera.h"

const Size eSize = Size(120, 30);
const int points = 10;
const int moveInvCnt = 40;

SeaCucumber::SeaCucumber(std::shared_ptr<Camera>& camera) : Enemy(camera), _camera(camera)
{
	auto pos = Vector2(800, 300);
	auto size = eSize;
	auto rect = Rect(pos, size);

	enemy = EnemyInfo(pos, size, rect);
	_vel = Vector2();
	cPoint._flag = _turnFlag = false;
	cPoint._pos = enemy._pos;
	cPoint._vel = Vector2();
	enemy._dieFlag = false;

	cPoint._pos.x += (_turnFlag ? -eSize.width / 2 : eSize.width / 2);
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
	auto camera = _camera->CameraCorrection();
	_vel.x = (enemy._pos.x < Game::GetInstance().GetScreenSize().x / 2 - camera.x ? -maxSpeed : maxSpeed);
	_vel.y = 0;

	for (auto& pos : enemy._searchVert)
	{
		pos = enemy._pos;
	}
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
			cPoint._vel.x = (moveCnt >= moveInvCnt ? maxSpeed : cPoint._vel.x);
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
			cPoint._vel.x = (moveCnt >= moveInvCnt ? -maxSpeed : cPoint._vel.x);
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
	if (enemy._pos.x + enemy._size.width / 2 < 0 ||
		enemy._pos.x - enemy._size.width / 2 > Game::GetInstance().GetScreenSize().x)
	{
		Die();
	}
}

void SeaCucumber::DieUpdate()
{
}

void SeaCucumber::CalBezier()
{
	/// §Œä“_‚ÌˆÚ“®
	if (cPoint._flag)
	{
		if (_vel.x <= 0.f)
		{
			cPoint._vel.x = (cPoint._vel.x <= 0.f ? 0.f : cPoint._vel.x - 0.1f);
		}
		else
		{
			cPoint._vel.x = 0;
		}
	}
	else
	{
		if (_vel.x >= 0.f)
		{
			cPoint._vel.x = (cPoint._vel.x >= 0.f ? 0.f : cPoint._vel.x + 0.1f);
		}
		else
		{
			cPoint._vel.x = 0;
		}
	}
	cPoint._pos += cPoint._vel;

	/// ’†ŠÔ“_‚Ìİ’è
	midPoints.resize(points);
	midPoints[0] = enemy._pos;
	for (int m = 0; m < points; ++m)
	{
		float b = (float)m / points;
		float a = 1.0f - b;

		/// 2ŸÍŞ¼Şª‚ÌŒvZ
		midPoints[m].x = (a * a * (enemy._pos.x - (eSize.width / 2))) + (2 * a * b * cPoint._pos.x) + 
						 (b * b * (enemy._pos.x + (eSize.width / 2)));

		midPoints[m].y = (a * a * enemy._pos.y) + (2 * a * b * cPoint._pos.y) +
						 (b * b * enemy._pos.y);
	}
}

void SeaCucumber::Draw()
{
	auto camera = _camera->CameraCorrection();

	for (int i = 1; i < midPoints.size(); ++i)
	{
		DxLib::DrawBox(midPoints[i - 1].x - camera.x, midPoints[i - 1].y - camera.y,
					   midPoints[i].x - camera.x, midPoints[i].y - camera.y, color, true);
	}

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void SeaCucumber::DebugDraw(const Vector2 & camera)
{
	/// “–‚½‚è”»’è‚Ì•`‰æ
	DrawBox(enemy._rect.Left()  - camera.x, enemy._rect.Top() - camera.y,
			enemy._rect.Right() - camera.y, enemy._rect.Bottom() - camera.y, 0xff0000, false);

	Vector2 p1, p2, p3, p4;
	auto height = enemy._size.height / 2;			/// •`‰æ‚·‚é‚‚³‚Ì’²®
	color = (_updater == &SeaCucumber::EscapeUpdate ? 0x80300b : 0xa0522d);
	for (int i = 1; i < midPoints.size(); ++i)
	{
		p1 = Vector2(midPoints[i - 1].x - camera.x, midPoints[i - 1].y - height - camera.y);
		p2 = Vector2(midPoints[i].x - camera.x, midPoints[i].y - height - camera.y);
		p3 = Vector2(midPoints[i].x - camera.x, midPoints[i].y + height - camera.y);
		p4 = Vector2(midPoints[i - 1].x - camera.x, midPoints[i - 1].y + height - camera.y);

		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color, true);
	}

	DrawCircle(cPoint._pos.x - camera.x, cPoint._pos.y - camera.y, 4, 0xffff00, false);
}

void SeaCucumber::Update()
{
	(this->*_updater)();

	enemy._pos += _vel;

	/// ‚Æ‚è‚ ‚¦‚¸‰¼İ’è‚È‚Ì‚ÅAŒã‚ÅC³‚ğ‚·‚é
	if (_updater == &SeaCucumber::EscapeUpdate || enemy._dieFlag)
	{
		auto size = Size(0, 0);
		enemy._rect = Rect(enemy._pos, size);
	}
	else
	{
		enemy._rect = Rect(enemy._pos, enemy._size);
	}

	CalBezier();
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
	/// ¡‚Ì‚Æ‚±‚ëA¼®¯Ä‚ğ‘Å‚Â—\’è‚Í‚È‚¢
}

void SeaCucumber::CalTrackVel(const Vector2 & pos, bool col)
{
	/// ƒvƒŒƒCƒ„[‚ğ’Ç]‚·‚é—\’è‚Í‚È‚¢
}
