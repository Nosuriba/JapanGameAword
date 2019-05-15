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
	enemy._prePos = enemy._pos;
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
			_vel.x = (moveCnt <= 0 ? maxSpeed : 0.f);
		}
		else
		{
			_vel.x -= (_vel.x <= 0.f ? 0.f :0.1f);
		}
	}
	else
	{
		/// �Ƃ肠�����A�������C��
		if (_vel.x >= 0.f)
		{
			if (moveCnt == 0)
			{
				cPoint._vel.x = -maxSpeed;
			}
			moveCnt--;
		}
		else
		{
			_vel.x += (_vel.x >= 0.f ? 0.f : 0.1f);
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
	/// ����_�̈ړ�
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
		/// �Ƃ肠�����A�������C��
		if (_vel.x >= 0.f && moveCnt == 0)
		{
			cPoint._vel.x += (cPoint._vel.x >= 0.f ? 0.f : 0.1f);
			_vel.x =  (cPoint._vel.x >= 0.f ? -maxSpeed : 0);
			moveCnt = (_vel.x == -maxSpeed ? moveInvCnt : 0);
		}
	}
	cPoint._pos += cPoint._vel;

	/// ���ԓ_�̐ݒ�
	midPoints.resize(points);
	midPoints[0] = enemy._pos;
	for (int m = 0; m < points; ++m)
	{
		float b = (float)m / points;
		float a = 1.0f - b;

		/// 2���޼ު�̌v�Z
		midPoints[m].x = (a * a * (enemy._pos.x - (eSize.width / 2))) + (2 * a * b * cPoint._pos.x) + 
						 (b * b * (enemy._pos.x + (eSize.width / 2)));

		midPoints[m].y = (a * a * enemy._pos.y) + (2 * a * b * cPoint._pos.y) +
						 (b * b * enemy._pos.y);
	}
}

void SeaCucumber::Draw()
{
	auto camera = _camera->CameraCorrection();

	/*for (int i = 1; i < midPoints.size(); ++i)
	{
		DxLib::DrawBox(midPoints[i - 1].x - camera.x, midPoints[i - 1].y - camera.y,
					   midPoints[i].x - camera.x, midPoints[i].y - camera.y, color, true);
	}
*/
	color = (_updater == &SeaCucumber::EscapeUpdate ? 0x80300b : 0xa0522d);
	auto oWidth = abs(enemy._pos.x - midPoints[points - 1].x);
	DrawOval(enemy._pos.x, enemy._pos.y, oWidth, enemy._size.height / 2, color, true);

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void SeaCucumber::DebugDraw(const Vector2 & camera)
{
	/// �����蔻��̕`��
	DrawBox(enemy._rect.Left()  - camera.x, enemy._rect.Top() - camera.y,
			enemy._rect.Right() - camera.y, enemy._rect.Bottom() - camera.y, 0xff0000, false);

	/// �G�̕`��
	DrawCircle(enemy._pos.x, enemy._pos.y, 4, 0x00ffff, true);

	/// ����_�̕`��
	DrawCircle(cPoint._pos.x - camera.x, cPoint._pos.y - camera.y, 4, 0xffff00, true);
}

void SeaCucumber::Update()
{
	(this->*_updater)();

	enemy._prePos = enemy._pos;
	enemy._pos += _vel;

	/// �Ƃ肠�������ݒ�Ȃ̂ŁA��ŏC��������
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
	/// ���̂Ƃ���A���Ă�ł\��͂Ȃ�
}

void SeaCucumber::CalTrackVel(const Vector2 & pos, bool col)
{
	/// �v���C���[��Ǐ]����\��͂Ȃ�
}
