#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(3.f, 3.f);	// ‹r‚Ì“®‚­‘¬“x
const Vector2 fVel  = Vector2(5.f, 5.f);	// ‰£‚é‘¬“x
const VECTOR rotDir = { 0,0,1.f };								// ‰ñ“]•ûŒü
const VECTOR revRotDir = { -rotDir.x, -rotDir.y, -rotDir.z };	// ‹t‚Ì‰ñ“]•ûŒü
const float rotVel  = DX_PI_F / 540.f;
const int length    = 100;
const int aLength	= length + 60;
const int typeMax   = static_cast<int>(AtkType::MAX);
const int invCntMax	= 60;
const Size eSize    = Size(250, 150);			// ŠI‚Ì‘å‚«‚³
const Size lSize    = Size(length, 20);			// ‹r‚Ì‘å‚«‚³
const Size scisSize = Size(60, 25);		// ‚Í‚³‚İ‚Ì‘å‚«‚³				

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_plPos	   = Vector2();
	_armPrePos = Vector2();
	atkInvCnt  = invCntMax;
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
		/// ŠÖß‚ğ’Ç‰Á‚µ‚Ä‚¢‚é
		(*leg)._points.resize(3);
		/// ˆÚ“®§Œä—p‚ÌÀ•W
		_legPrePos.push_back(Vector2());
		_legMovePos.push_back(Vector2());

		auto cnt = leg - boss._crab._legs.begin();
		auto pos = boss._crab._pos + (!(cnt / (boss._crab._legs.size() / 2))
									  ? Vector2(eSize.width / 2, -eSize.height / 3 - lSize.height / 2)
									  : Vector2(-eSize.width / 2, -eSize.height / 3 - lSize.height / 2));
		(*leg)._points[0] = pos + Vector2(0, (cnt % (boss._crab._legs.size() / 2)) * (lSize.height * 2));
		auto point = (*leg)._points.begin() + 1;
		for (; point != (*leg)._points.end(); ++point)
		{
			auto pCnt = point - (*leg)._points.begin();
			auto leng = (!(cnt / (boss._crab._legs.size() / 2)) ? length : -length);
			(*point) = (*leg)._points[pCnt - 1] + Vector2(leng, 0 );
		}
		(*leg)._ctlPoint = (*leg)._points[2] + (!(cnt / (boss._crab._legs.size() / 2))
												? Vector2(-length / 2, 0)
												: Vector2(length / 2, 0));
		(*leg)._vel = Vector2();
	}
}

void Crab::ArmInit()
{
	/// ˜r‚Ì‰Šú‰»
	boss._crab._arms.resize(2);
	/// ‚Í‚³‚İ‚Ì‰Šú‰»
	_scisCenter.resize(boss._crab._arms.size() * 2);
	_scissors.resize(_scisCenter.size());
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		(*arm)._points.resize(3);
		auto cnt = arm - boss._crab._arms.begin();
		auto pos = boss._crab._pos - (!(cnt % 2) ? Vector2(-eSize.width / 5, eSize.height / 2)
												 : Vector2(eSize.width / 5, eSize.height / 2));
		(*arm)._points[0] = pos;
		auto point = (*arm)._points.begin() + 1;
		for (; point != (*arm)._points.end(); ++point)
		{
			auto pCnt = point - (*arm)._points.begin();
			(*point) = (*arm)._points[pCnt - 1] - Vector2(0, aLength);
		}
		(*arm)._ctlPoint = (*arm)._points[2] + Vector2(0, aLength);
		(*arm)._vel = Vector2();
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
				atkInvCnt  = invCntMax;
				break;
			}
		}
	}
	else
	{
		/// ˜r‚ğL‚Î‚·
		if (_type == AtkType::FIST)
		{
			for (auto& arm : boss._crab._arms)
			{
				auto d = abs((_armPrePos.x - arm._ctlPoint.x) + (_armPrePos.y - arm._ctlPoint.y));
				if ((StopCheck(arm._ctlPoint, _plPos, arm._vel) || (d > aLength))
					&& arm._vel.x != 0)
				{
					auto vec = _armPrePos - arm._ctlPoint;
					vec.Normalize();
					arm._vel   = Vector2(fVel.x * vec.x, fVel.y * vec.y);
					_plPos	   = Vector2();
					_type	   = AtkType::PITCH;
					rotInvCnt = invCntMax;
				}
			}
		}
		else
		{
			scisRotation();
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
			auto point = leg._points.begin() + 1;
			for (; point != leg._points.end(); ++point)
			{
				auto cnt = point - leg._points.begin();
				theta = atan2f((*point).y - leg._points[cnt - 1].y,
							   (*point).x - leg._points[cnt - 1].x);

				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size.x = cost * (lSize.height / 2);
				size.y = sint * (lSize.height / 2);
				sizePos = (p < 2 ? -size : size);

				leg._vert[cnt - 1][p] = (p == 0 || p == 3 ? leg._points[cnt - 1] : (*point)) + sizePos;
			}
		}
	}

	std::vector<float> dirTheta;
	/// ˜r‚Ì‹éŒ`İ’è
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		auto aCnt = arm - boss._crab._arms.begin();
		for (int p = 0; p < (*arm)._vert[0].size(); ++p)
		{
			auto point = (*arm)._points.begin() + 1;
			for (; point != (*arm)._points.end(); ++point)
			{
				auto cnt = point - (*arm)._points.begin();
				theta = atan2f((*point).y - (*arm)._points[cnt - 1].y,
							   (*point).x - (*arm)._points[cnt - 1].x);

				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size.x = cost * (lSize.height / 2);
				size.y = sint * (lSize.height / 2);
				sizePos = (p < 2 ? -size : size);

				(*arm)._vert[cnt - 1][p] = (p == 0 || p == 3 ? (*arm)._points[cnt - 1] : (*point)) + sizePos;
			}
		}
		/// ‹r‚Ì•‚ÌŠp“x‚ğ“o˜^‚µ‚Ä‚¢‚é
		dirTheta.push_back(atan2f((*arm)._vert[1][1].y - (*arm)._vert[1][0].y,
								  (*arm)._vert[1][1].x - (*arm)._vert[1][0].x));

		/// ˜r‚Ìæ’[‚ğ‘¾‚­‚·‚éˆ—
		for (int i = 1; i <= (*arm)._points.size() - 1; ++i)
		{
			theta = atan2f((*arm)._vert[1][i].y - (*arm)._vert[1][(*arm)._points.size() - i].y,
						   (*arm)._vert[1][i].x - (*arm)._vert[1][(*arm)._points.size() - i].x);
			cost = cos(theta);
			sint = sin(theta);
			(*arm)._vert[1][i] = (*arm)._vert[1][i] + Vector2(length / 4 * cost, length / 4 * sint);

		}
	}

	if (_type != AtkType::PITCH)
	{
		/// ‚Í‚³‚İ‚Ì’Ü‚Ì‹éŒ`İ’è
		auto aSize = boss._crab._arms.size();		/// ˜r‚Ì”
		auto scis = _scissors.begin();
		for (; scis != _scissors.end(); ++scis)
		{
			auto sCnt = scis - _scissors.begin();
			auto vert = _scissors[sCnt].begin();
			for (; vert != _scissors[sCnt].end(); ++vert)
			{
				auto vCnt = vert - _scissors[sCnt].begin();
				auto sPos = boss._crab._arms[sCnt / aSize]._vert[1][(sCnt % 2) + 1];

				theta = dirTheta[sCnt / aSize];		/// ˜r‚Ì’†ŠÔ“_‚©‚çæ’[‚ÉŒü‚¯‚Ä‚ÌÍŞ¸ÄÙ
				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size.x = cost * (scisSize.height / 2);
				size.y = sint * (scisSize.height / 2);
				sizePos = (vCnt < 2 ? Vector2() : size * 2);
				sizePos = (sCnt % 2 ? -sizePos : sizePos);

				cost = cos(theta);
				sint = sin(theta);

				auto offset = -Vector2(scisSize.height / 3 * cost, scisSize.height / 3 * sint);
				auto lengPos = (vCnt != 0 && vCnt != 3 ? Vector2(scisSize.width * cost, scisSize.width * sint)
					: Vector2());

				(*vert) = sPos + sizePos + offset + lengPos;
			}
			theta = atan2f(_scissors[sCnt][3].y - _scissors[sCnt][0].y,
						   _scissors[sCnt][3].x - _scissors[sCnt][0].x);
			cost = cos(theta);
			sint = sin(theta);

			_scisCenter[sCnt] = boss._crab._arms[sCnt / 2]._points[2] + Vector2(scisSize.width / 2 * cost, scisSize.width / 2 * sint);
		}
	}
	
}

void Crab::scisRotation()
{
	
	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		auto vert = _scissors[sCnt].begin();
		for (; vert != _scissors[sCnt].end(); ++vert)
		{
			auto vCnt = vert - _scissors[sCnt].begin();
			if (vCnt > 0)
			{
			 	auto center = (sCnt % 2 ? _scissors[sCnt][0] : _scissors[sCnt][3]);
				auto rot = (sCnt % 2 ? revRotDir : rotDir);
				///	‚Í‚³‚ŞŠÔ‚Ì’²®‚ğ‚µ‚Ä‚¢‚é
				if (rotInvCnt <= invCntMax / 2)
				{
					rot = (sCnt % 2 ? rotDir : revRotDir);
				}
				
				auto mat = MGetTranslate((-center).V_Cast());
				mat		 = MMult(mat, MGetRotAxis(rot, rotVel * 5));	
				mat		 = MMult(mat, MGetTranslate(center.V_Cast()));
				(*vert)  = VTransform((*vert).V_Cast(), mat);
			}
		}
	}

	if (rotInvCnt < 0)
	{
		_type = AtkType::NORMAL;
	}
	else
	{
		rotInvCnt--;
	}
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotDir, rotVel)); 	 // Z²‚Ì‰ñ“]s—ñ‚ğì‚Á‚Ä‚¢‚é
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
		for (auto& point : (*leg)._points)
		{
			point = VTransform(point.V_Cast(), mat);
		}
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
		for (auto& point : arm._points)
		{
			point = VTransform(point.V_Cast(), mat);
		}
		arm._ctlPoint = VTransform(arm._ctlPoint.V_Cast(), mat);
	}
	_armPrePos = VTransform(_armPrePos.V_Cast(), mat);

	/// ‚Í‚³‚İ‚Ì‰ñ“]
	auto scissor = _scissors.begin();
	for (; scissor != _scissors.end(); ++scissor)
	{
		auto sCnt = scissor - _scissors.begin();
		for (auto& vert : _scissors[sCnt])
		{
			vert = VTransform(vert.V_Cast(), mat);
		}
	}
}

void Crab::MoveLeg()
{
	Vector2 vec;												// ˆÚ“®‚·‚é•ûŒü‚ÌÍŞ¸ÄÙæ“¾—p
	auto dirVec = boss._crab._vert[1] - boss._crab._vert[0];	// ˆÚ“®•ûŒü‚ÌÍŞ¸ÄÙ	
	dirVec.Normalize();

	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// ‰E‘«‚Ì“®‚«
			/// ‹r‚ğL‚Î‚·æ‚ªŒˆ‚Ü‚Á‚Ä‚¢‚È‚¢AˆÚ“®æ‚ğ“o˜^‚·‚é
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * dirVec.x, (length / 2) * dirVec.y);

				(*leg)._vel = Vector2(lVel.x * dirVec.x, lVel.y * dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ‹r‚ğL‚Î‚·
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._ctlPoint + Vector2(length * -dirVec.x, length * -dirVec.y);
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
				/// ‹r‚ğ–ß‚·
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * dirVec.x, length * dirVec.y);
					(*leg)._vel = Vector2(lVel.x * dirVec.x, lVel.y * dirVec.y);
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
			/// ‹r‚ğL‚Î‚·æ‚ªŒˆ‚Ü‚Á‚Ä‚¢‚È‚¢AˆÚ“®æ‚ğ“o˜^‚·‚é
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * -dirVec.x, (length / 2) * -dirVec.y);
				(*leg)._vel = Vector2(lVel.x * -dirVec.x, lVel.y * -dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ‹r‚ğL‚Î‚·
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._ctlPoint + Vector2(length * dirVec.x, length * dirVec.y);
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
				/// ‹r‚ğ–ß‚·
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * -dirVec.x, length * -dirVec.y);
					(*leg)._vel = Vector2(lVel.x * -dirVec.x, lVel.y * -dirVec.y);
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
		pLength = (*leg)._ctlPoint - (*leg)._points[0];
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

				(*leg)._points[1] = (*leg)._points[0] + cosD + sinD;
				(*leg)._points[2] = (*leg)._ctlPoint;
			}
			else
			{
				/// ’·‚³‚ªˆê’è‹——£‚æ‚è‚È‚ª‚­‚È‚Á‚½ê‡
				(*leg)._points[1] = (*leg)._points[0] + (pLength.Normalized() * length);
				(*leg)._points[2] = (*leg)._points[1] + (pLength.Normalized() * length);
			}
		}
	}

	/// ˜rŠÖß‚ÌˆÚ“®
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		pLength = (*arm)._ctlPoint - (*arm)._points[0];
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

				(*arm)._points[1] = (*arm)._points[0] + cosD + sinD;
				(*arm)._points[2] = (*arm)._ctlPoint;
			}
			else
			{
				/// ’·‚³‚ªˆê’è‹——£‚æ‚è‚È‚ª‚­‚È‚Á‚½ê‡
				(*arm)._points[1] = (*arm)._points[0] + (pLength.Normalized() * aLength);
				(*arm)._points[2] = (*arm)._points[1] + (pLength.Normalized() * aLength);
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

	///	‚±‚±‚É–AUŒ‚‚·‚é‚Ìˆ—‚à’Ç‰Á‚µ‚Ä‚¨‚­
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

	/// ‹r‚Ì•`‰æ(Œã‚ÅC³)
	Vector2 p1, p2, p3, p4;
	for (auto leg : boss._crab._legs)
	{
		for (int i = 0; i < leg._points.size() - 1; ++i)
		{
			p1 = leg._vert[i][0] - camera;
			p2 = leg._vert[i][1] - camera;
			p3 = leg._vert[i][2] - camera;
			p4 = leg._vert[i][3] - camera;
			DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	/// ‚Í‚³‚İ‚Ì•`‰æ
	for (auto scissor : _scissors)
	{
		p1 = scissor[0] - camera;
		p2 = scissor[1] - camera;
		p3 = scissor[2] - camera;
		p4 = scissor[3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdddd0000, true);
	}
	
	/// ˜r‚Ì•`‰æ
	for (auto arm : boss._crab._arms)
	{
		for (int i = 0; i < arm._points.size() - 1; ++i)
		{
			p1 = arm._vert[i][0] - camera;
			p2 = arm._vert[i][1] - camera;
			p3 = arm._vert[i][2] - camera;
			p4 = arm._vert[i][3] - camera;
			DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	/// ŠI–{‘Ì‚Ì•`‰æ
	DxLib::DrawQuadrangle(boss._crab._vert[0].x - camera.x, boss._crab._vert[0].y - camera.y,
						  boss._crab._vert[1].x - camera.x, boss._crab._vert[1].y - camera.y,
						  boss._crab._vert[2].x - camera.x, boss._crab._vert[2].y - camera.y,
						  boss._crab._vert[3].x - camera.x, boss._crab._vert[3].y - camera.y,
						  0xcc3300, true);

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

	// ˜r‚ª“®‚­I“_‚Ü‚Å‚Ì‹——£
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

	for (int i = 0; i < _scisCenter.size(); ++i)
	{
		DxLib::DrawCircle(_scisCenter[i].x, _scisCenter[i].y, 4, 0x00ff00, true);
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
		if (_type != AtkType::PITCH)
		arm._ctlPoint += arm._vel;
	}

	if (_type == AtkType::NORMAL && atkInvCnt >= 0)
	{
		Rotation();
		MoveLeg();
	}
	
	CalVert();			/// ‹éŒ`‚Ì’¸“_‚ğİ’è
	
	MoveJoint();
}
