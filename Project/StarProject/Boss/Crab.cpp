#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(3.f, 3.f);	// ãrÇÃìÆÇ≠ë¨ìx
const Vector2 fVel  = Vector2(5.f, 5.f);	// â£ÇÈë¨ìx
const VECTOR rotVec = { 0,0,1.f };
const float rotVel  = DX_PI_F / 540.f;
const int length    = 100;
const int aLength	= length + 60;
const int typeMax   = static_cast<int>(AtkType::MAX);
const int atkCnt	= 60;
const Size eSize    = Size(250, 150);
const Size lSize    = Size(length, 20);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_plPos	  = Vector2();
	_preCtl	  = Vector2();
	atkInvCnt = atkCnt;
	_type	  = AtkType::NORMAL;

	/// äIñ{ëÃÇÃèâä˙âª
	boss._crab._pos = Vector2(center.x, 700);
	boss._crab._size = eSize;
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		auto posX = (i != 0 && i != 3 ? eSize.width / 2 : -eSize.width / 2);
		auto posY = (!(i / (boss._crab._vert.size() / 2)) ? eSize.height / 2 : -eSize.height / 2);

		boss._crab._vert[i] = boss._crab._pos + Vector2(posX, posY);
	}
		
	/// ãrÇÃèâä˙âª
	boss._crab._legs.resize(8);
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// âEë´
			auto pos = Vector2(boss._crab._pos.x + eSize.width  / 2, 
							   boss._crab._pos.y - eSize.height / 3 - lSize.height / 2);
			(*leg)._sPoint	 = pos + Vector2(0, (cnt * (lSize.height * 2)));
			(*leg)._mPoint	 = (*leg)._sPoint + Vector2(length, 0);
			(*leg)._ePoint	 = (*leg)._mPoint + Vector2(length, 0);
			(*leg)._ctlPoint = (*leg)._ePoint - Vector2(length / 2, 0);

			(*leg)._vel = Vector2(lVel.x, lVel.y);
		}
		else
		{
			/// ç∂ë´
			auto pos = Vector2(boss._crab._pos.x - eSize.width  / 2,
							   boss._crab._pos.y - eSize.height / 3 - lSize.height / 2);
			(*leg)._sPoint	 = pos + Vector2(0, (cnt % (boss._crab._legs.size() / 2)) * (lSize.height * 2));
			(*leg)._mPoint	 = (*leg)._sPoint - Vector2(length, 0);
			(*leg)._ePoint	 = (*leg)._mPoint - Vector2(length, 0);
			(*leg)._ctlPoint = (*leg)._ePoint + Vector2(length, 0);

			(*leg)._vel = Vector2(lVel.x, -lVel.y);
		}
	}

	/// òrÇÃèâä˙âª
	boss._crab._arms.resize(2);
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		auto cnt = arm - boss._crab._arms.begin();
		if (!(cnt % 2))
		{
			/// âEòr
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 5,
							   boss._crab._pos.y - eSize.height / 2);
			(*arm)._sPoint   = pos;
			(*arm)._mPoint   = (*arm)._sPoint - Vector2(0, aLength);
			(*arm)._ePoint   = (*arm)._mPoint - Vector2(0, aLength);
			(*arm)._ctlPoint = (*arm)._ePoint + Vector2(0, aLength);
			(*arm)._vel		 = Vector2();
		}
		else
		{
			/// ç∂òr
			auto pos = Vector2(boss._crab._pos.x - eSize.width / 5,
							   boss._crab._pos.y - eSize.height / 2);
			(*arm)._sPoint   = pos;
			(*arm)._mPoint   = (*arm)._sPoint - Vector2(0, aLength);
			(*arm)._ePoint   = (*arm)._mPoint - Vector2(0, aLength);
			(*arm)._ctlPoint = (*arm)._ePoint + Vector2(0, aLength);
			(*arm)._vel		 = Vector2();
		}
	}
	/// ä÷êﬂÇÃèâä˙âª
	MoveJoint();
	CalVert();

	Neutral();
}

Crab::~Crab()
{
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
	if (atkInvCnt <= 0)
	{
		/// ◊›¿ﬁ—Ç≈çUåÇ ﬂ¿∞›ÇåàíË
		// _type = static_cast<AtkType>(GetRand(typeMax - 1));

		/// debugóp
		_type = static_cast<AtkType>(3);	/// â£ÇÈçUåÇ
		 //type = static_cast<AtkType>(2);	/// ã≤ÇﬁçUåÇ
		//_type = static_cast<AtkType>(1);	/// ñAÇÃçUåÇ
	}
	
}

void Crab::FistUpdate()
{
	if (_type == AtkType::NORMAL)
	{
		/// òrÇà¯Ç´ñﬂÇ∑éû
		for (auto arm : boss._crab._arms)
		{
			if (arm._vel.x != 0)
			{
				if (arm._vel.x > 0)
				{
					arm._vel = (arm._ctlPoint.x > _preCtl.x && arm._ctlPoint.y > _preCtl.y ? Vector2() : arm._vel);
				}
				else
				{
					arm._vel = (_preCtl.x < arm.l.x && arm._ctlPoint.y < _preCtl.y ? Vector2() : arm._vel);
				}

				if (arm._vel.x == 0)
				{
					Neutral();
					_preCtl = Vector2();
					atkInvCnt = atkCnt;
					break;
				}
			}	
		}
	}
	else
	{
		/// òrÇêLÇŒÇ∑éû
		for (auto arm : boss._crab._arms)
		{
			if (arm._vel.x != 0)
			{
				if (arm._vel.x > 0)
				{
					arm._vel = (arm._ctlPoint.x > _plPos.x && arm._ctlPoint.y > _plPos.y ? Vector2() : arm._vel);
				}
				else
				{
					arm._vel = (arm._ctlPoint.x < _plPos.x && arm._ctlPoint.y < _plPos.y ? Vector2() : arm._vel);
				}

				/// 
				if (arm._vel.x == 0)
				{
					_plPos = Vector2();
					auto vec = _preCtl - arm._ctlPoint;
					vec.Normalize();
					arm._vel = Vector2(fVel.x * vec.x, fVel.y * vec.y);
					_type = AtkType::NORMAL;
		
				}
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

	/// ãrÇÃãÈå`ê›íË
	for (auto& leg : boss._crab._legs)
	{
		for (int p = 0; p < leg._vert[0].size(); ++p)
		{
			/// énì_Ç©ÇÁíÜä‘ì_Ç‹Ç≈ÇÃãÈå`ÇÃê›íË
			theta = atan2f(leg._mPoint.y - leg._sPoint.y, leg._mPoint.x - leg._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[0][p] = (p == 0 || p == 3 ? leg._sPoint : leg._mPoint) + sizePos;

			/// íÜä‘ì_Ç©ÇÁèIì_Ç‹Ç≈ÇÃãÈå`ÇÃê›íË
			theta = atan2f(leg._ePoint.y - leg._mPoint.y, leg._ePoint.x - leg._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[1][p] = (p == 0 || p == 3 ? leg._mPoint : leg._ePoint) + sizePos;
		}
	}

	/// òrÇÃãÈå`ê›íË
	for (auto& arm : boss._crab._arms)
	{
		for (int p = 0; p < arm._vert[0].size(); ++p)
		{
			/// énì_Ç©ÇÁíÜä‘ì_Ç‹Ç≈ÇÃãÈå`ÇÃê›íË
			theta = atan2f(arm._mPoint.y - arm._sPoint.y, arm._mPoint.x - arm._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			arm._vert[0][p] = (p == 0 || p == 3 ? arm._sPoint : arm._mPoint) + sizePos;

			/// íÜä‘ì_Ç©ÇÁèIì_Ç‹Ç≈ÇÃãÈå`ÇÃê›íË
			theta = atan2f(arm._ePoint.y - arm._mPoint.y, arm._ePoint.x - arm._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			arm._vert[1][p] = (p == 0 || p == 3 ? arm._mPoint : arm._ePoint) + sizePos;
		}
	}
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotVec, rotVel)); 	 // Zé≤ÇÃâÒì]çsóÒÇçÏÇ¡ÇƒÇ¢ÇÈ
	mat = MMult(mat, MGetTranslate(cPos.V_Cast()));
	/// äIñ{ëÃÇÃâÒì]
	boss._crab._pos = VTransform(boss._crab._pos.V_Cast(), mat);

	/// äIÇÃí∏ì_ÇÃâÒì]
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		boss._crab._vert[i] = VTransform(boss._crab._vert[i].V_Cast(), mat);
	}

	/// ãrÇÃâÒì]
	for (auto& leg : boss._crab._legs)
	{
		leg._sPoint = VTransform(leg._sPoint.V_Cast(), mat);
		leg._mPoint = VTransform(leg._mPoint.V_Cast(), mat);
		leg._ePoint = VTransform(leg._ePoint.V_Cast(), mat);
		leg._ctlPoint = VTransform(leg._ctlPoint.V_Cast(), mat);
	}

	/// òrÇÃâÒì]
	for (auto& arm : boss._crab._arms)
	{
		arm._sPoint = VTransform(arm._sPoint.V_Cast(), mat);
		arm._mPoint = VTransform(arm._mPoint.V_Cast(), mat);
		arm._ePoint = VTransform(arm._ePoint.V_Cast(), mat);
		arm._ctlPoint = VTransform(arm._ctlPoint.V_Cast(), mat);
	}
	_preCtl = VTransform(_preCtl.V_Cast(), mat);
}

void Crab::MoveLeg()
{
	/// äIñ{ëÃÇÃç∂í[Ç∆âEí[ÇÃí∏ì_Ç©ÇÁï˚å¸ÉxÉNÉgÉãÇéÊìæÇµÇƒÇ¢ÇÈ
	auto vec = boss._crab._vert[1] - boss._crab._vert[0];
	vec.Normalize();
	
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();

		auto d = Vector2(abs((*leg)._ePoint.x - (*leg)._sPoint.x),
						 abs((*leg)._ePoint.y - (*leg)._sPoint.y));
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// âEãrÇÃìÆÇ´
			if ((*leg)._vel.x > 0)
			{
				(*leg)._liftVel = ((*leg)._liftVel == 0 ? GetRand(3) * 0.2f : (*leg)._liftVel);
				(*leg)._vel.x = (d.x > length + (length / 2) ? -(*leg)._vel.x : (*leg)._vel.x);
			}
			else
			{
				(*leg)._liftVel = 0;
				(*leg)._vel.x = (d.x < length - (length / 2) ? -(*leg)._vel.x : (*leg)._vel.x);
			}

			if ((*leg)._vel.y > 0)
			{
				(*leg)._vel.y = (d.y > length + (length / 2) ? -(*leg)._vel.y : (*leg)._vel.y);
			}
			else
			{
				(*leg)._vel.y = (d.y < length - (length / 2) ? -(*leg)._vel.y : (*leg)._vel.y);
			}
		}
		else
		{
			/// ç∂ãrÇÃìÆÇ´
			if ((*leg)._vel.x > 0)
			{
				(*leg)._liftVel = 0;
				(*leg)._vel.x = (d.x < length - (length / 2) ? -(*leg)._vel.x : (*leg)._vel.x);
			}
			else
			{
				(*leg)._liftVel = ((*leg)._liftVel == 0 ? GetRand(3) * 0.2f : (*leg)._liftVel);
				(*leg)._vel.x = (d.x > length + (length / 2) ? -(*leg)._vel.x : (*leg)._vel.x);
			}

			if ((*leg)._vel.y > 0)
			{
				(*leg)._vel.y = (d.y < length - (length / 2) ? -(*leg)._vel.y : (*leg)._vel.y);
			}
			else
			{
				(*leg)._vel.y = (d.y > length + (length / 2) ? -(*leg)._vel.y : (*leg)._vel.y);
			}
		}
		(*leg)._ctlPoint += Vector2((*leg)._vel.x * vec.x, 
									(*leg)._vel.y * vec.y /*+ (*leg)._liftVel*/);	
	}
}

void Crab::MoveJoint()
{
	Vector2 pLength;
	float sint, cost, rad;

	/// ãrä÷êﬂÇÃà⁄ìÆ
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
				auto cnt = leg - boss._crab._legs.begin();	/// ë´ÇÃî‘ínéwíËóp

				/// Yé≤ÇÃï˚å¸Õﬁ∏ƒŸÇåvéZÇµÇƒÇ¢ÇÈ
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (length * cost);		/// Xï˚å¸ÇÃê¨ï™
				auto sinD = cross2f * (length * sint);					/// Yï˚å¸ÇÃê¨ï™

				/// ç∂ä÷êﬂÇ™ãtÇ…å¸Ç©Ç»Ç¢ÇΩÇﬂÇÃèàóù
				sinD = (!(cnt / (boss._crab._legs.size() / 2)) ? sinD : -sinD);

				(*leg)._mPoint = (*leg)._sPoint + cosD + sinD;
				(*leg)._ePoint = (*leg)._ctlPoint;
			}
			else
			{
				/// í∑Ç≥Ç™àÍíËãóó£ÇÊÇËÇ»Ç™Ç≠Ç»Ç¡ÇΩèÍçá
				(*leg)._mPoint = (*leg)._sPoint + (pLength.Normalized() * length);
				(*leg)._ePoint = (*leg)._mPoint + (pLength.Normalized() * length);
			}
		}
	}

	/// òrä÷êﬂÇÃà⁄ìÆ
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
				auto cnt = arm - boss._crab._arms.begin();	/// ë´ÇÃî‘ínéwíËóp

				/// Yé≤ÇÃï˚å¸Õﬁ∏ƒŸÇåvéZÇµÇƒÇ¢ÇÈ
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (aLength * cost);	/// Xï˚å¸ÇÃê¨ï™
				auto sinD = cross2f * (aLength * sint);					/// Yï˚å¸ÇÃê¨ï™

				/// ç∂ä÷êﬂÇ™ãtÇ…å¸Ç©Ç»Ç¢ÇΩÇﬂÇÃèàóù
				sinD = (cnt / (boss._crab._arms.size() / 2) ? sinD : -sinD);

				(*arm)._mPoint = (*arm)._sPoint + cosD + sinD;
				(*arm)._ePoint = (*arm)._ctlPoint;
			}
			else
			{
				/// í∑Ç≥Ç™àÍíËãóó£ÇÊÇËÇ»Ç™Ç≠Ç»Ç¡ÇΩèÍçá
				(*arm)._mPoint = (*arm)._sPoint + (pLength.Normalized() * aLength);
				(*arm)._ePoint = (*arm)._mPoint + (pLength.Normalized() * aLength);
			}
		}
	}
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
				_preCtl = (*arm)._ctlPoint;			/// à⁄ìÆëOÇÃêßå‰ì_Çï€ë∂
			}
		}
		Fist();
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// ãrÇÃï`âÊ
	Vector2 p1, p2, p3, p4;
	for (auto leg : boss._crab._legs)
	{
		/// énì_Ç©ÇÁíÜä‘Ç‹Ç≈ÇÃãÈå`
		p1 = leg._vert[0][0] - camera;
		p2 = leg._vert[0][1] - camera;
		p3 = leg._vert[0][2] - camera;
		p4 = leg._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// íÜä‘Ç©ÇÁêÊí[Ç‹Ç≈ÇÃãÈå`
		p1 = leg._vert[1][0] - camera;
		p2 = leg._vert[1][1] - camera;
		p3 = leg._vert[1][2] - camera;
		p4 = leg._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}

	/// òrÇÃï`âÊ
	for (auto arm : boss._crab._arms)
	{
		/// énì_Ç©ÇÁíÜä‘Ç‹Ç≈ÇÃãÈå`
		p1 = arm._vert[0][0] - camera;
		p2 = arm._vert[0][1] - camera;
		p3 = arm._vert[0][2] - camera;
		p4 = arm._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// íÜä‘Ç©ÇÁêÊí[Ç‹Ç≈ÇÃãÈå`
		p1 = arm._vert[1][0] - camera;
		p2 = arm._vert[1][1] - camera;
		p3 = arm._vert[1][2] - camera;
		p4 = arm._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}
	/// äIñ{ëÃÇÃï`âÊ
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
	/// êßå‰ì_ÇÃï`âÊ
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
				DxLib::DrawLine(arm._ctlPoint.x, arm._ctlPoint.y, _preCtl.x, _preCtl.y, 0x00ff00, 2);
			}
			else if (_type == AtkType::FIST)
			{
				DxLib::DrawLine(arm._ctlPoint.x, arm._ctlPoint.y, _plPos.x, _plPos.y, 0xff5555, 2);
			}
			else {}
		}
	}

	/// âÒì]Ç∑ÇÈÇ∆Ç´ÇÃíÜêSì_ÇÃï`âÊ
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);
}

void Crab::Update()
{
	(this->*_updater)();

	/// òrÇÃà⁄ìÆ
	for (auto& arm : boss._crab._arms)
	{
		arm._ctlPoint += arm._vel;
	}

	if (_type == AtkType::NORMAL && atkInvCnt >= 0)
	{
		Rotation();
	}
	
	CalVert();			/// ãÈå`ÇÃí∏ì_Çê›íË
	MoveLeg();
	MoveJoint();

	boss._crab._prePos = boss._crab._pos;
}
