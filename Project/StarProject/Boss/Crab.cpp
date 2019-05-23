#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(3.f, 3.f);	// ‹r‚Ì“®‚­‘¬“x
const Vector2 fVel  = Vector2(5.f, 5.f);	// ‰£‚é‘¬“x
const VECTOR rotVec = { 0,0,1.f };
const float rotVel  = DX_PI_F / 540.f;
const int length    = 100;
const int aLength	= length + 60;
const int typeMax   = static_cast<int>(AtkType::MAX);
const int atkCnt	= 60;
const Size eSize    = Size(250, 150);			// ŠI‚Ì‘å‚«‚³
const Size lSize    = Size(length, 20);			// ‹r‚Ì‘å‚«‚³
const Size scisSize = Size(length / 2, 10);		// ‚Í‚³‚İ‚Ì‘å‚«‚³				

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_plPos	   = Vector2();
	_armPrePos = Vector2();
	atkInvCnt  = atkCnt;
	_type	   = AtkType::NORMAL;

	/// ŠI‚Ì‰Šú‰»
	BodyInit();
	LegInit();
	ArmInit();
	
	/// ŠÖß‚Ì‰Šú‰»
	MoveJoint();
	CalVert();
	Neutral();
}

Crab::~Crab()
{
}

void Crab::BodyInit()
{
	/// ŠI–{‘Ì‚Ì‰Šú‰»
	boss._crab._pos = Vector2(center.x, 700);
	boss._crab._size = eSize;
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		auto posX = (i != 0 && i != 3 ? eSize.width / 2 : -eSize.width / 2);
		auto posY = (!(i / (boss._crab._vert.size() / 2)) ? eSize.height / 2 : -eSize.height / 2);

		boss._crab._vert[i] = boss._crab._pos + Vector2(posX, posY);
	}
}

void Crab::LegInit()
{
	/// ‹r‚Ì‰Šú‰»
	boss._crab._legs.resize(8);
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		_legPrePos.push_back(Vector2());
		_legMovePos.push_back(Vector2());

		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// ‰E‘«
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 2,
				boss._crab._pos.y - eSize.height / 3 - lSize.height / 2);
			(*leg)._sPoint = pos + Vector2(0, (cnt * (lSize.height * 2)));
			(*leg)._mPoint = (*leg)._sPoint + Vector2(length, 0);
			(*leg)._ePoint = (*leg)._mPoint + Vector2(length, 0);
			(*leg)._ctlPoint = (*leg)._ePoint - Vector2(length / 2, 0);

			(*leg)._vel = Vector2();
		}
		else
		{
			/// ¶‘«
			auto pos = Vector2(boss._crab._pos.x - eSize.width / 2,
				boss._crab._pos.y - eSize.height / 3 - lSize.height / 2);
			(*leg)._sPoint = pos + Vector2(0, (cnt % (boss._crab._legs.size() / 2)) * (lSize.height * 2));
			(*leg)._mPoint = (*leg)._sPoint - Vector2(length, 0);
			(*leg)._ePoint = (*leg)._mPoint - Vector2(length, 0);
			(*leg)._ctlPoint = (*leg)._ePoint + Vector2(length / 2, 0);

			(*leg)._vel = Vector2();
		}
	}
}

void Crab::ArmInit()
{
	/// ˜r‚Ì‰Šú‰»
	boss._crab._arms.resize(2);
	/// ‚Í‚³‚İ‚Ì‰Šú‰»
	_scisCenter.resize(boss._crab._arms.size());
	_scissors.resize(boss._crab._arms.size() * 2);
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		auto cnt = arm - boss._crab._arms.begin();
		if (!(cnt % 2))
		{
			/// ‰E˜r
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 5,
				boss._crab._pos.y - eSize.height / 2);
			(*arm)._sPoint = pos;
			(*arm)._mPoint = (*arm)._sPoint - Vector2(0, aLength);
			(*arm)._ePoint = (*arm)._mPoint - Vector2(0, aLength);
			(*arm)._ctlPoint = (*arm)._ePoint + Vector2(0, aLength);
			(*arm)._vel = Vector2();
		}
		else
		{
			/// ¶˜r
			auto pos = Vector2(boss._crab._pos.x - eSize.width / 5,
				boss._crab._pos.y - eSize.height / 2);
			(*arm)._sPoint = pos;
			(*arm)._mPoint = (*arm)._sPoint - Vector2(0, aLength);
			(*arm)._ePoint = (*arm)._mPoint - Vector2(0, aLength);
			(*arm)._ctlPoint = (*arm)._ePoint + Vector2(0, aLength);
			(*arm)._vel = Vector2();
		}
	}
}

void Crab::Neutral()
{
	_updater = &Crab::NeutralUpdate;
}

void Crab::Fist()
{
	_updater = &Crab::FistUpdate;
}

void Crab::Shot()
{
	_updater = &Crab::ShotUpdate;
}

void Crab::Die()
{
	_updater = &Crab::DieUpdate;
}

void Crab::NeutralUpdate()
{
	atkInvCnt--;
	if (atkInvCnt < 0)
	{
		/// ×İÀŞÑ‚ÅUŒ‚ÊßÀ°İ‚ğŒˆ’è
		// _type = static_cast<AtkType>(GetRand(typeMax - 1));

		/// debug—p
		_type = static_cast<AtkType>(3);	/// ‰£‚éUŒ‚
		 //type = static_cast<AtkType>(2);	/// ‹²‚ŞUŒ‚
		//_type = static_cast<AtkType>(1);	/// –A‚ÌUŒ‚
	}
	
}

void Crab::FistUpdate()
{
	if (_type == AtkType::NORMAL)
	{
		/// ˜r‚ğˆø‚«–ß‚·
		for (auto& arm : boss._crab._arms)
		{
			if (StopCheck(arm._ctlPoint, _armPrePos, arm._vel) && arm._vel.x != 0)
			{
				Neutral();
				_armPrePos = Vector2();
				arm._vel   = Vector2();
				atkInvCnt  = atkCnt;
				break;
			}
		}
	}
	else
	{
		/// ˜r‚ğL‚Î‚·
		for (auto& arm : boss._crab._arms)
		{
       			auto d = abs((_armPrePos.x - arm._ctlPoint.x) + (_armPrePos.y - arm._ctlPoint.y));
			if ((StopCheck(arm._ctlPoint, _plPos, arm._vel) || (d > aLength)) 
				&& arm._vel.x != 0)
			{
				auto vec = _armPrePos - arm._ctlPoint;
				vec.Normalize();
				arm._vel = Vector2(fVel.x * vec.x, fVel.y * vec.y);
				_plPos	 = Vector2();
				_type	 = AtkType::NORMAL;
		
			}
		}
	}
}

void Crab::ShotUpdate()
{
}

void Crab::DieUpdate()
{
}

void Crab::CalVert()
{
	Vector2 size, sizePos;
	float theta, cost, sint;

	/// ‹r‚Ì‹éŒ`İ’è
	for (auto& leg : boss._crab._legs)
	{
		for (int p = 0; p < leg._vert[0].size(); ++p)
		{
			/// n“_‚©‚ç’†ŠÔ“_‚Ü‚Å‚Ì‹éŒ`‚Ìİ’è
			theta = atan2f(leg._mPoint.y - leg._sPoint.y, leg._mPoint.x - leg._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[0][p] = (p == 0 || p == 3 ? leg._sPoint : leg._mPoint) + sizePos;

			/// ’†ŠÔ“_‚©‚çI“_‚Ü‚Å‚Ì‹éŒ`‚Ìİ’è
			theta = atan2f(leg._ePoint.y - leg._mPoint.y, leg._ePoint.x - leg._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[1][p] = (p == 0 || p == 3 ? leg._mPoint : leg._ePoint) + sizePos;
		}
	}

	/// ˜r‚Ì‹éŒ`İ’è
	for (auto& arm : boss._crab._arms)
	{
		for (int p = 0; p < arm._vert[0].size(); ++p)
		{
			/// n“_‚©‚ç’†ŠÔ“_‚Ü‚Å‚Ì‹éŒ`‚Ìİ’è
			theta = atan2f(arm._mPoint.y - arm._sPoint.y, arm._mPoint.x - arm._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			arm._vert[0][p] = (p == 0 || p == 3 ? arm._sPoint : arm._mPoint) + sizePos;

			/// ’†ŠÔ“_‚©‚çI“_‚Ü‚Å‚Ì‹éŒ`‚Ìİ’è
			theta = atan2f(arm._ePoint.y - arm._mPoint.y, arm._ePoint.x - arm._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			arm._vert[1][p] = (p == 0 || p == 3 ? arm._mPoint : arm._ePoint) + sizePos;
		}
	}

	/// ‚Í‚³‚İ‚Ì‹éŒ`İ’è
	auto scissor = _scissors.begin();
	auto armCnt = boss._crab._arms.size();
	for (; scissor != _scissors.end(); ++scissor)
	{
		auto sCnt = scissor - _scissors.begin();		// ’Ü‚Ì”
		auto vert = _scissors[0].begin();
		for (; vert != _scissors[0].end(); ++vert)
		{
			auto vCnt = vert - _scissors[0].begin();	// ’¸“_‚Ì”


			/// n“_İ’è
			auto sPos = (vCnt / 2 ? boss._crab._arms[sCnt / armCnt]._vert[1][3]
								  : boss._crab._arms[sCnt / armCnt]._vert[1][3]);

			theta = atan2f(boss._crab._arms[sCnt / armCnt]._ePoint.y - boss._crab._arms[sCnt / armCnt]._mPoint.y,
						   boss._crab._arms[sCnt / armCnt]._ePoint.x - boss._crab._arms[sCnt / armCnt]._mPoint.x);

			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			/// ‹éŒ`İ’è
			size.x = cost * (scisSize.height / 2);
			size.y = sint * (scisSize.height / 2);
			sizePos = ((vCnt % 2) ? size : -size);

			/// æ’[‚Ì‹——£İ’è
			auto lengPos = (vCnt == 1 || vCnt == 2 ? Vector2(scisSize.width * cost, scisSize.height * sint)
												   : Vector2());

			(*vert) = sPos + sizePos + lengPos;
		}
		
	}
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotVec, rotVel)); 	 // Z²‚Ì‰ñ“]s—ñ‚ğì‚Á‚Ä‚¢‚é
	mat = MMult(mat, MGetTranslate(cPos.V_Cast()));
	/// ŠI–{‘Ì‚Ì‰ñ“]
	boss._crab._pos = VTransform(boss._crab._pos.V_Cast(), mat);

	/// ŠI‚Ì’¸“_‚Ì‰ñ“]
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		boss._crab._vert[i] = VTransform(boss._crab._vert[i].V_Cast(), mat);
	}

	/// ‹r‚Ì‰ñ“]
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();
		(*leg)._sPoint = VTransform((*leg)._sPoint.V_Cast(), mat);
		(*leg)._mPoint = VTransform((*leg)._mPoint.V_Cast(), mat);
		(*leg)._ePoint = VTransform((*leg)._ePoint.V_Cast(), mat);
		(*leg)._ctlPoint = VTransform((*leg)._ctlPoint.V_Cast(), mat);

		if (_legMovePos[cnt].x != 0)
		{
			_legMovePos[cnt] = VTransform(_legMovePos[cnt].V_Cast(), mat);
		}
		
		if (_legPrePos[cnt].x != 0)
		{
			_legPrePos[cnt] = VTransform(_legPrePos[cnt].V_Cast(), mat);
		}
	}

	/// ˜r‚Ì‰ñ“]
	for (auto& arm : boss._crab._arms)
	{
		arm._sPoint = VTransform(arm._sPoint.V_Cast(), mat);
		arm._mPoint = VTransform(arm._mPoint.V_Cast(), mat);
		arm._ePoint = VTransform(arm._ePoint.V_Cast(), mat);
		arm._ctlPoint = VTransform(arm._ctlPoint.V_Cast(), mat);
	}
	_armPrePos = VTransform(_armPrePos.V_Cast(), mat);
}

void Crab::MoveLeg()
{
	Vector2 vec;		// ˆÚ“®‚·‚é•ûŒü‚ÌÍŞ¸ÄÙæ“¾—p

	/// ŠI–{‘Ì‚Ì¶’¸“_‚©‚ç‰E’¸“_‚Ì•ûŒüÍŞ¸ÄÙ(‰E‹r—p)
	auto rightVec = boss._crab._vert[1] - boss._crab._vert[0];
	/// ŠI–{‘Ì‚Ì‰E’¸“_‚©‚ç¶’¸“_‚Ì•ûŒüÍŞ¸ÄÙ(¶‹r—p)
	auto leftVec = boss._crab._vert[0] - boss._crab._vert[1];
	rightVec.Normalize();
	leftVec.Normalize();
	
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// ‰E‘«‚Ì“®‚«(New)
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._ePoint + Vector2((length / 2) * rightVec.x, (length / 2) * rightVec.y);
				_legPrePos[cnt] = (*leg)._ctlPoint;

				(*leg)._vel = Vector2(lVel.x * rightVec.x, lVel.y * rightVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ‘«‚ğL‚Î‚·
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._ctlPoint + Vector2(length * -rightVec.x, length * -rightVec.y);
					(*leg)._vel = -(*leg)._vel;
				}
				else
				{
					vec = _legMovePos[cnt] - (*leg)._ctlPoint;
					vec.Normalize();
					(*leg)._vel = Vector2(lVel.x * vec.x, lVel.y * vec.y);
				}
			}
			else
			{
				/// ‘«‚ğ–ß‚·
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * rightVec.x, length * rightVec.y);
					(*leg)._vel = Vector2(lVel.x * rightVec.x, lVel.y * rightVec.y);
				}
				else
				{
					vec = _legPrePos[cnt] - (*leg)._ctlPoint;
					vec.Normalize();
					(*leg)._vel = Vector2(lVel.x * vec.x, lVel.y * vec.y);
				}
			}
		}
		else
		{
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._ePoint + Vector2((length / 2) * leftVec.x, (length / 2) * leftVec.y);
				_legPrePos[cnt] = (*leg)._ctlPoint;

				(*leg)._vel = Vector2(lVel.x * leftVec.x, lVel.y * leftVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ‘«‚ğL‚Î‚·
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._ctlPoint + Vector2(length * -leftVec.x, length * -leftVec.y);
					(*leg)._vel = -(*leg)._vel;
				}
				else
				{
					vec = _legMovePos[cnt] - (*leg)._ctlPoint;
					vec.Normalize();
					(*leg)._vel = Vector2(lVel.x * vec.x, lVel.y * vec.y);
				}
			}
			else
			{
				/// ‘«‚ğ–ß‚·
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * leftVec.x, length * leftVec.y);
					(*leg)._vel = Vector2(lVel.x * leftVec.x, lVel.y * leftVec.y);
				}
				else
				{
					vec = _legPrePos[cnt] - (*leg)._ctlPoint;
					vec.Normalize();
					(*leg)._vel = Vector2(lVel.x * vec.x, lVel.y * vec.y);
				}
			}
		}
		(*leg)._ctlPoint += Vector2((*leg)._vel.x, (*leg)._vel.y);
	}
}

void Crab::MoveJoint()
{
	Vector2 pLength;
	float sint, cost, rad;

	/// ‹rŠÖß‚ÌˆÚ“®
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		pLength = (*leg)._ctlPoint - (*leg)._sPoint;
		cost = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
		rad  = acos(cost);
		sint = sin(rad);

		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < length * 2)
			{
				auto cnt = leg - boss._crab._legs.begin();	/// ‘«‚Ì”Ô’nw’è—p

				/// Y²‚Ì•ûŒüÍŞ¸ÄÙ‚ğŒvZ‚µ‚Ä‚¢‚é
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (length * cost);		/// X•ûŒü‚Ì¬•ª
				auto sinD = cross2f * (length * sint);					/// Y•ûŒü‚Ì¬•ª

				/// ¶ŠÖß‚ª‹t‚ÉŒü‚©‚È‚¢‚½‚ß‚Ìˆ—
				sinD = (!(cnt / (boss._crab._legs.size() / 2)) ? sinD : -sinD);

				(*leg)._mPoint = (*leg)._sPoint + cosD + sinD;
				(*leg)._ePoint = (*leg)._ctlPoint;
			}
			else
			{
				/// ’·‚³‚ªˆê’è‹——£‚æ‚è‚È‚ª‚­‚È‚Á‚½ê‡
				(*leg)._mPoint = (*leg)._sPoint + (pLength.Normalized() * length);
				(*leg)._ePoint = (*leg)._mPoint + (pLength.Normalized() * length);
			}
		}
	}

	/// ˜rŠÖß‚ÌˆÚ“®
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		pLength = (*arm)._ctlPoint - (*arm)._sPoint;
		cost = (pow(aLength, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(aLength, 2.0)) / (2 * aLength * pLength.Magnitude());
		rad  = acos(cost);
		sint = sin(rad);

		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < aLength * 2)
			{
				auto cnt = arm - boss._crab._arms.begin();	/// ‘«‚Ì”Ô’nw’è—p

				/// Y²‚Ì•ûŒüÍŞ¸ÄÙ‚ğŒvZ‚µ‚Ä‚¢‚é
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (aLength * cost);	/// X•ûŒü‚Ì¬•ª
				auto sinD = cross2f * (aLength * sint);					/// Y•ûŒü‚Ì¬•ª

				/// ¶ŠÖß‚ª‹t‚ÉŒü‚©‚È‚¢‚½‚ß‚Ìˆ—
				sinD = (cnt / (boss._crab._arms.size() / 2) ? sinD : -sinD);

				(*arm)._mPoint = (*arm)._sPoint + cosD + sinD;
				(*arm)._ePoint = (*arm)._ctlPoint;
			}
			else
			{
				/// ’·‚³‚ªˆê’è‹——£‚æ‚è‚È‚ª‚­‚È‚Á‚½ê‡
				(*arm)._mPoint = (*arm)._sPoint + (pLength.Normalized() * aLength);
				(*arm)._ePoint = (*arm)._mPoint + (pLength.Normalized() * aLength);
			}
		}
	}
}

bool Crab::StopCheck(const Vector2 & sPos, const Vector2 & ePos, const Vector2 & vel)
{
	auto rtnFlag = false;

	if (vel.x <= 0)
	{
		rtnFlag = (sPos.x <= ePos.x);
	}
	else
	{
		rtnFlag = (sPos.x >= ePos.x);
	}

	if (rtnFlag)
	{
		if (vel.y <= 0)
		{
			rtnFlag = (sPos.y <= ePos.y);
		}
		else
		{
			rtnFlag = (sPos.y >= ePos.y);
		}
	}

	return rtnFlag;
}

BossInfo Crab::GetInfo()
{
	return boss;
}

void Crab::CalTrackVel(const Vector2 & pos)
{
	if (_type == AtkType::FIST && _updater == &Crab::NeutralUpdate)
	{
		_plPos = pos;
		auto l = GetRand(boss._crab._arms.size() - 1);
		auto arm = boss._crab._arms.begin();
		for (; arm != boss._crab._arms.end(); ++arm)
		{
			auto cnt = arm - boss._crab._arms.begin();
			if (cnt == l)
			{
				_plPos = pos;
				auto vec = _plPos - (*arm)._ctlPoint;
				vec.Normalize();
				(*arm)._vel = Vector2(fVel.x * vec.x, fVel.y * vec.y);
				_armPrePos = (*arm)._ctlPoint;			/// ˆÚ“®‘O‚Ì§Œä“_‚ğ•Û‘¶
			}
		}
		Fist();
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// ‹r‚Ì•`‰æ
	Vector2 p1, p2, p3, p4;
	/*for (auto leg : boss._crab._legs)
	{
		/// n“_‚©‚ç’†ŠÔ‚Ü‚Å‚Ì‹éŒ`
		p1 = leg._vert[0][0] - camera;
		p2 = leg._vert[0][1] - camera;
		p3 = leg._vert[0][2] - camera;
		p4 = leg._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// ’†ŠÔ‚©‚çæ’[‚Ü‚Å‚Ì‹éŒ`
		p1 = leg._vert[1][0] - camera;
		p2 = leg._vert[1][1] - camera;
		p3 = leg._vert[1][2] - camera;
		p4 = leg._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}

	/// ˜r‚Ì•`‰æ
	for (auto arm : boss._crab._arms)
	{
		/// n“_‚©‚ç’†ŠÔ‚Ü‚Å‚Ì‹éŒ`
		p1 = arm._vert[0][0] - camera;
		p2 = arm._vert[0][1] - camera;
		p3 = arm._vert[0][2] - camera;
		p4 = arm._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// ’†ŠÔ‚©‚çæ’[‚Ü‚Å‚Ì‹éŒ`
		p1 = arm._vert[1][0] - camera;
		p2 = arm._vert[1][1] - camera;
		p3 = arm._vert[1][2] - camera;
		p4 = arm._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}*/

	/// ‚Í‚³‚İ‚Ì•`‰æ
	for (auto scissor : _scissors)
	{
		p1 = scissor[0] - camera;
		p2 = scissor[1] - camera;
		p3 = scissor[2] - camera;
		p4 = scissor[3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}

	

	/*/// ŠI–{‘Ì‚Ì•`‰æ
	DxLib::DrawQuadrangle(boss._crab._vert[0].x - camera.x, boss._crab._vert[0].y - camera.y,
						  boss._crab._vert[1].x - camera.x, boss._crab._vert[1].y - camera.y,
						  boss._crab._vert[2].x - camera.x, boss._crab._vert[2].y - camera.y,
						  boss._crab._vert[3].x - camera.x, boss._crab._vert[3].y - camera.y,
						  0xcc3300, true);*/

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void Crab::DebugDraw(const Vector2& camera)
{
	/// §Œä“_‚Ì•`‰æ
	for (auto leg : boss._crab._legs)
	{
		DrawCircle(leg._ctlPoint.x - camera.x, leg._ctlPoint.y - camera.y, 4, 0xffff00, true);
	}
	for (auto arm : boss._crab._arms)
	{
		DrawCircle(arm._ctlPoint.x - camera.x, arm._ctlPoint.y - camera.y, 4, 0xffff00, true);

		if (arm._vel.x != 0 && arm._vel.y != 0)
		{
			if (_type == AtkType::NORMAL)
			{
				DxLib::DrawLine(arm._ctlPoint.x, arm._ctlPoint.y, _armPrePos.x, _armPrePos.y, 0x00ff00, 2);
			}
			else if (_type == AtkType::FIST)
			{
				DxLib::DrawLine(arm._ctlPoint.x, arm._ctlPoint.y, _plPos.x, _plPos.y, 0xff5555, 2);
			}
			else {}
		}
	}

	/// ˆÚ“®‚·‚é‘«‚Ì§Œä“_‚Ì•`‰æ
	for (int i = 0; i < boss._crab._legs.size(); ++i)
	{
		DxLib::DrawCircle(_legMovePos[i].x, _legMovePos[i].y, 4, 0xff0000, true);
		DxLib::DrawCircle(_legPrePos[i].x, _legPrePos[i].y, 4, 0x00ff00, true);
	}

	/// ‰ñ“]‚·‚é‚Æ‚«‚Ì’†S“_‚Ì•`‰æ
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);
}

void Crab::Update()
{
	(this->*_updater)();

	/// ˜r‚ÌˆÚ“®
	for (auto& arm : boss._crab._arms)
	{
		//arm._ctlPoint += arm._vel;
	}

	//if (_type == AtkType::NORMAL && atkInvCnt >= 0)
	//{
	//	Rotation();
	//	MoveLeg();
	//}
	//CalVert();			/// ‹éŒ`‚Ì’¸“_‚ğİ’è
	//
	//MoveJoint();

	boss._crab._prePos = boss._crab._pos;
}
