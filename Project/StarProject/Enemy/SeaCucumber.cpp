#include "SeaCucumber.h"
#include "../Game.h"
#include "../Camera.h"

const int points	 = 10;
const int moveInvCnt = 40;
const int crawlVel   = 1.5f;
const int decSpeeed  = 0.02f;			// Œ¸‘¬‘¬“x

SeaCucumber::SeaCucumber(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos) : Enemy(c, p)
{
	auto size	= Size(120, 30);

	_enemy		= EnemyInfo(pos, size);

	_pL = POS + Vector2(-1, 0) * (SIZE.width / 2.0f);
	_pR = POS + Vector2(1, 0) * (SIZE.width / 2.0f);

	auto cpos = c->CameraCorrection();

	_particle.emplace_back( std::make_shared<Bubble>(_pL.x - cpos.x, _pL.y - cpos.y, 1000, false, true, 5, 3, 0x660099,c));
	_particle.emplace_back( std::make_shared<Bubble>(_pR.x - cpos.x, _pR.y - cpos.y, 1000, false, true, 5, 3, 0x660099,c));

	_updater = &SeaCucumber::WaitUpdate;
}

SeaCucumber::~SeaCucumber()
{
}

void SeaCucumber::WaitUpdate()
{
	++_anim_frame;
	if (_anim_frame % 300 == 0)
	{
		_anim_frame = 0;
		_isTurn		= (GetRand(25252) % 2 == 0);
		_updater	= &SeaCucumber::MoveUpdate;
	}
}

void SeaCucumber::MoveUpdate()
{
	if (_anim_frame < 20)
	{
		if (_isTurn)
		{
			auto v = _pR - _pL;
			_pL = _pL + v.Normalized();

			SIZE.width = (_pR - _pL).Magnitude();
			POS = _pL + v.Normalized() * (SIZE.width / 2.0f);
		}
		else
		{
			auto v = _pL - _pR;
			_pR = _pR + v.Normalized();

			SIZE.width = (_pL - _pR).Magnitude();
			POS = _pR + v.Normalized() * (SIZE.width / 2.0f);
		}
	}
	else if (_anim_frame < 40)
	{
		if (_isTurn)
		{
			auto v = _pR - _pL;
			_pR = _pR + v.Normalized();

			SIZE.width = (_pR - _pL).Magnitude();
			POS = _pL + v.Normalized() * (SIZE.width / 2.0f);
		}
		else
		{
			auto v = _pL - _pR;
			_pL = _pL + v.Normalized();

			SIZE.width = (_pL - _pR).Magnitude();
			POS = _pR + v.Normalized() * (SIZE.width / 2.0f);
		}
	}

	_particle[0]->SetPos(_pL.x,_pL.y);
	_particle[1]->SetPos(_pR.x, _pR.y);

	++_anim_frame;
	if (_anim_frame > 40)
	{
		_anim_frame = 0;
		_updater	= &SeaCucumber::WaitUpdate;
	}
}

void SeaCucumber::CounterUpdate()
{
	auto cpos=_camera->CameraCorrection();

	for (auto& p : _particle)
	{
		_particle[0]->SetRota(180);
		p->Create();
	}

	++_anim_frame;
	if (_anim_frame > 300)
	{
		DAMAGE		= false;
		_anim_frame = 0;
		_updater	= &SeaCucumber::WaitUpdate;
	}
}

void SeaCucumber::Draw()
{
	/// “G‚Ì•`‰æ
	DxLib::DrawOval(POS.x - CC.x, POS.y - CC.y, SIZE.width / 2, SIZE.height / 2, 0, true);

#ifdef _DEBUG
	DebugDraw();
#endif

	for (auto& p : _particle)
	{
		p->Draw();
	}
}

void SeaCucumber::Shadow()
{
	/// “G‚Ì•`‰æ
	DxLib::DrawOval(POS.x - CC.x, POS.y - CC.y, SIZE.width / 2, SIZE.height / 2, 0, true);
}

void SeaCucumber::DebugDraw()
{ 
	// “G‚Ì’†S“_•`‰æ
	DrawCircle(POS.x - CC.x, POS.y - CC.y, 4, 0x000000, true);
	DrawCircle(_pL.x - CC.x, _pL.y - CC.y, 4, 0xff0000, true);
	DrawCircle(_pR.x - CC.x, _pR.y - CC.y, 4, 0x0000ff, true);

	for (auto& d : _damage)
		DrawCircle(d.pos.x - CC.x, d.pos.y - CC.y, d.r, 0xff00ff, true);
	for (auto& a : _attack)
		DrawCircle(a.pos.x - CC.x, a.pos.y - CC.y, a.r, 0xff00ff, true);
}

void SeaCucumber::Update()
{
	(this->*_updater)();

	CreateDamagePoints();
	CreateAttackPoints();
}

void SeaCucumber::OnDamage()
{
	if (!DAMAGE)
	{
		DAMAGE		= true;
		_anim_frame = 0;
		_updater	= &SeaCucumber::CounterUpdate;
	}
}

void SeaCucumber::CreateDamagePoints()
{
	_damage.clear();

	_damage.emplace_back(POS, SIZE.height / 2);
	_damage.emplace_back((POS + _pL) / 2, SIZE.height / 3);
	_damage.emplace_back((POS + _pR) / 2, SIZE.height / 3);
	_damage.emplace_back(Vector2(_pL.x + SIZE.width / 10, _pL.y), SIZE.height / 4);
	_damage.emplace_back(Vector2(_pR.x - SIZE.width / 10, _pL.y), SIZE.height / 4);
}

void SeaCucumber::CreateAttackPoints()
{
	_attack.clear();

	_attack.emplace_back(POS, SIZE.height / 2);
	_attack.emplace_back((POS + _pL) / 2, SIZE.height / 3);
	_attack.emplace_back((POS + _pR) / 2, SIZE.height / 3);
	_attack.emplace_back(Vector2(_pL.x + SIZE.width / 10, _pL.y), SIZE.height / 4);
	_attack.emplace_back(Vector2(_pR.x - SIZE.width / 10, _pR.y), SIZE.height / 4);
	if (_updater == &SeaCucumber::CounterUpdate)
	{
		_attack.emplace_back(Vector2(_pL.x - SIZE.width / 2, _pL.y), SIZE.height);
		_attack.emplace_back(Vector2(_pR.x + SIZE.width / 2, _pR.y), SIZE.height);
		_attack.emplace_back(Vector2(_pL.x - SIZE.width / 5, _pL.y), SIZE.height / 2);
		_attack.emplace_back(Vector2(_pR.x + SIZE.width / 5, _pR.y), SIZE.height / 2);
	}
}
