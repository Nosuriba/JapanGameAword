#include "Crab.h"
#include "../ResourceManager.h"
#include "../Particle/Particle.h"
#include "../Particle/Bubble.h"

const VECTOR rotDir = { 0,0,1.f };								// âÒì]ï˚å¸
const VECTOR revRotDir = { -rotDir.x, -rotDir.y, -rotDir.z };	// ãtÇÃâÒì]ï˚å¸
const Vector2 eSize = Vector2(250, 150);
const double magRate = 1.4;						// ägëÂó¶
const float rotVel = DX_PI_F / 540.f;
const float mVel   = 3.f;
const int atkMax   = 120;
const int inviMax  = 180;
const int pitchMax = 50;
const int shotMax  = 240;

Crab::Crab(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos) : Boss(c, p)
{
	_plPos = Vector2();
	_armPrePos = Vector2();
	atkCnt = atkMax;
	_type  = AtkType::NORMAL;
	_lifeCnt = 20;
	_isDie = false;
	inviCnt = 0;

	center = pos;
	/// ª≤Ωﬁê›íË
	_crab._pos  = Vector2(center.x, center.y * 2 - (center.y / 3));
	_crab._size = Size(eSize.x * magRate, eSize.y * magRate);
	lSize		= Size(100 * magRate, 20 * magRate);
	scisSize	= Size(70 * magRate, lSize.height * magRate);
	length		= lSize.width * magRate;
	aLength		= (lSize.width + (60 * magRate)) * magRate;

	SE.damage = ResourceManager::GetInstance().LoadSound("../Sound/damage.mp3");
	SE.die	  = ResourceManager::GetInstance().LoadSound("../Sound/die.mp3");
	SE.pitch  = ResourceManager::GetInstance().LoadSound("../Sound/Crab/pitch.mp3");
	SE.shot   = ResourceManager::GetInstance().LoadSound("../Sound/Crab/shot.mp3");
	SE.swing  = ResourceManager::GetInstance().LoadSound("../Sound/Crab/swing.mp3");
	SE.walk   = ResourceManager::GetInstance().LoadSound("../Sound/Crab/walk.mp3");

	BGM		  = ResourceManager::GetInstance().LoadSound("../Sound/boss.mp3");

	BodyInit();
	LegInit();
	ArmInit();
	/// ä÷êﬂÇÃèâä˙âª
	MoveJoint();
	CalVert();
	Neutral();

}

Crab::~Crab()
{
	if (CheckSoundMem(BGM))
	{
		StopSoundMem(BGM);
	}
}

void Crab::BodyInit()
{
	for (int i = 0; i < _crab._vert.size(); ++i)
	{
		auto posX = (i != 0 && i != 3 ? _crab._size.width / 2 : -_crab._size.width / 2);
		auto posY = (!(i / (_crab._vert.size() / 2)) ? -_crab._size.height / 2 : _crab._size.height / 2);
		_crab._vert[i] = _crab._pos + Vector2(posX, posY);
	}
}

void Crab::LegInit()
{
	if (_crab._legs.size() > 0)
	{
		_crab._legs.clear();
	}
	_crab._legs.resize(8);
	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		(*leg)._points.resize(3);
		(*leg)._center.resize(2);
		/// à⁄ìÆêßå‰ópÇÃç¿ïW
		_legPrePos.emplace_back(Vector2());
		_legMovePos.emplace_back(Vector2());
		_legAccel.emplace_back(Vector2());

		auto cnt = leg - _crab._legs.begin();
		auto pos = _crab._pos + (!(cnt / (_crab._legs.size() / 2))
				 ? Vector2(_crab._size.width / 2, -_crab._size.height / 3 - lSize.height / 2)
				 : Vector2(-_crab._size.width / 2, -_crab._size.height / 3 - lSize.height / 2));

		(*leg)._points[0] = pos + Vector2(0, (cnt % (_crab._legs.size() / 2)) * (lSize.height * 2));
		auto point = (*leg)._points.begin() + 1;
		for (; point != (*leg)._points.end(); ++point)
		{
			auto pCnt = point - (*leg)._points.begin();
			auto leng = (!(cnt / (_crab._legs.size() / 2)) ? length : -length);
			(*point)  = (*leg)._points[pCnt - 1] + Vector2(leng - (leng / 6), 0);
		}
		(*leg)._ctlPoint = (*leg)._points[2] + (!(cnt / (_crab._legs.size() / 2))
						 ? Vector2(-length / 2, 0)
						 : Vector2(length / 2, 0));
		(*leg)._vel = Vector2();
	}
}

void Crab::ArmInit()
{
	if (_crab._arms.size() > 0)
	{
		_crab._arms.clear();
		_scisCenter.clear();
		_scissors.clear();
	}
	_crab._arms.resize(2);
	/// ÇÕÇ≥Ç›ÇÃèâä˙âª
	_scisCenter.resize(_crab._arms.size() * 2);
	_scissors.resize(_scisCenter.size());
	auto arm = _crab._arms.begin();
	for (; arm != _crab._arms.end(); ++arm)
	{
		(*arm)._points.resize(3);
		(*arm)._center.resize(2);
		auto cnt = arm - _crab._arms.begin();
		auto pos = _crab._pos - (!(cnt % 2) ? Vector2(-_crab._size.width / 4, _crab._size.height / 2)
											: Vector2(_crab._size.width / 4, _crab._size.height / 2));
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
	_type = AtkType::NORMAL;
	_updater = &Crab::NeutralUpdate;
}

void Crab::Pitch()
{
	_type = AtkType::MOVE;
	ChangeVolumeSoundMem(255 * 800 / 180, SE.swing);
	PlaySoundMem(SE.swing, DX_PLAYTYPE_BACK);
	_updater = &Crab::PitchUpdate;
}

void Crab::Shot()
{
	_type = AtkType::SHOT;
	_updater = &Crab::ShotUpdate;
}

void Crab::Die()
{
	SetDrawValidGraphCreateFlag(true);
	quake	 =Vector2(10.f, 10.f);
	at.clear();
	da.clear();
	shot.clear();
	_updater = &Crab::DieUpdate;
}

void Crab::NeutralUpdate()
{
	atkCnt--;
}

void Crab::PitchUpdate()
{
	if (_type == AtkType::NORMAL)
	{	/// òrÇñﬂÇµÇƒÇ¢ÇÈéû
		for (auto& arm : _crab._arms)
		{
			if (StopCheck(arm._ctlPoint, _armPrePos, arm._vel) && arm._vel.x != 0)
			{
				Neutral();
				_armPrePos = Vector2();
				arm._vel = Vector2();
				atkCnt = atkMax;
				break;
			}
		}
	}
	else
	{	/// òrÇêLÇŒÇµÇƒÇ¢ÇÈéû
		if (_type == AtkType::MOVE)
		{
			for (auto& arm : _crab._arms)
			{
				auto d = abs((_armPrePos.x - arm._ctlPoint.x) + (_armPrePos.y - arm._ctlPoint.y));
				if ((StopCheck(arm._ctlPoint, _plPos, arm._vel) || (d > aLength))
					&& arm._vel.x != 0)
				{
					auto vec = (_armPrePos - arm._ctlPoint).Normalized();
					arm._vel = Vector2(mVel * vec.x, mVel * vec.y);
					_plPos = Vector2();
					_type = AtkType::PITCH;
					pitchCnt = pitchMax;
				}
			}
		}
		else
		{
			scisRota();
		}
	}
}

void Crab::ShotUpdate()
{
	if (shotCnt > 0)
	{
		if (!(shotCnt % 12))
		{
			auto c = _camera->CameraCorrection();
			auto vec = (_player->GetInfo().center - _crab._pos).Normalized();
			auto lengPos = Vector2(length * vec.x, length * vec.y);
			auto rand = (GetRand(10) - 5);
			auto pos = Vector2(10 * rand, 10 * rand) + lengPos;
			/// ÉvÉåÉCÉÑÅ[ÇÃï˚å¸Ç…å¸Ç©Ç¡Çƒï˙éÀè„Ç…ë≈Ç¬ÇÊÇ§ê›íËÇçsÇ§
			auto theta = atan2f((_crab._pos.y + pos.y) - _crab._pos.y,
								(_crab._pos.x + pos.x) - _crab._pos.x);
			auto cost = cos(theta);
			auto sint = sin(theta);
			auto vel = Vector2(5.0f * cost, 5.0f * sint);
			auto r = 15 * magRate;

			shot.emplace_back(ShotInfo(_crab._pos + lengPos / 2, vel, r));
			auto bPos = _crab._pos + (lengPos / 2);
			_particle = std::make_shared<Bubble>(bPos.x - c.x, bPos.y - c.y, 1000, false, true, 14, 40, 0xddffff);
			_particle->Create();

			PlaySoundMem(SE.shot, DX_PLAYTYPE_BACK);
		}
		shotCnt--;
	}
	else
	{
		Neutral();
		atkCnt = atkMax;
		_plPos = Vector2();
		_type = AtkType::NORMAL;
	}
}

void Crab::DieUpdate()
{
	blendCnt--;

	quake *= 0.9995;
	//quake = (quake.x <= 0.05 ? Vector2() : Vector2(quake.x * 0.9995, quake.y * 0.9995));

	if (blendCnt <= 0)
	{
		_isDie = true;
	}
}

void Crab::CalVert()
{
	at.clear();
	Vector2 size, sizePos;
	float theta, cost, sint;

	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		auto lCnt = leg - _crab._legs.begin();
		for (int p = 0; p < (*leg)._vert[0].size(); ++p)
		{/// ãÈå`ÇÃí∏ì_ê›íË
			auto point = (*leg)._points.begin() + 1;
			for (; point != (*leg)._points.end(); ++point)
			{
				auto cnt = point - (*leg)._points.begin();
				theta = atan2f((*point).y - (*leg)._points[cnt - 1].y, (*point).x - (*leg)._points[cnt - 1].x);
				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size = Vector2((lSize.height / 2) * cost, (lSize.height / 2) * sint);
				sizePos = (p < 2 ? -size : size);

				(*leg)._vert[cnt - 1][p] = (p == 0 || p == 3 ? (*leg)._points[cnt - 1] : (*point)) + sizePos;

				if (p >= 3)
				{
					auto vec = ((*leg)._vert[cnt - 1][2] - (*leg)._vert[cnt - 1][0]).Normalized();
					(*leg)._center[cnt - 1] = (*leg)._vert[cnt - 1][0] +
						Vector2((lSize.width / 2) * vec.x, (lSize.width / 2) * vec.y);
				}
			}
		}
	}

	std::vector<float> dirTheta;		/// ÇÕÇ≥Ç›ÇÃí‹ÇÃï˚å¸óp
	auto arm = _crab._arms.begin();
	for (; arm != _crab._arms.end(); ++arm)
	{
		auto aCnt = arm - _crab._arms.begin();
		for (int p = 0; p < (*arm)._vert[0].size(); ++p)
		{
			auto point = (*arm)._points.begin() + 1;
			for (; point != (*arm)._points.end(); ++point)
			{
				auto cnt = point - (*arm)._points.begin();
				theta = atan2f((*point).y - (*arm)._points[cnt - 1].y, (*point).x - (*arm)._points[cnt - 1].x);
				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size = Vector2((lSize.height / 2) * cost, (lSize.height / 2) * sint);
				sizePos = (p < 2 ? -size : size);
				(*arm)._vert[cnt - 1][p] = (p == 0 || p == 3 ? (*arm)._points[cnt - 1] : (*point)) + sizePos;

				if (p >= 3)
				{
					auto vec = ((*arm)._vert[cnt - 1][2] - (*arm)._vert[cnt - 1][0]).Normalized();
								(*arm)._center[cnt - 1] = (*arm)._vert[cnt - 1][0] + Vector2(aLength / 2 * vec.x, aLength / 2 * vec.y);
				}
			}
		}
		/// ãrÇÃïùÇÃäpìxÇìoò^ÇµÇƒÇ¢ÇÈ
		dirTheta.emplace_back(atan2f((*arm)._vert[1][1].y - (*arm)._vert[1][0].y,
			(*arm)._vert[1][1].x - (*arm)._vert[1][0].x));

		/// òrÇÃêÊí[ÇëæÇ≠Ç∑ÇÈèàóù
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
		auto aSize = _crab._arms.size();		/// òrÇÃêî
		auto scis = _scissors.begin();
		for (; scis != _scissors.end(); ++scis)
		{
			auto sCnt = scis - _scissors.begin();
			auto vert = _scissors[sCnt].begin();
			for (; vert != _scissors[sCnt].end(); ++vert)
			{
				auto vCnt = vert - _scissors[sCnt].begin();
				auto sPos = _crab._arms[sCnt / aSize]._vert[1][(sCnt % 2) + 1];

				theta = dirTheta[sCnt / aSize];		/// òrÇÃíÜä‘ì_Ç©ÇÁêÊí[Ç…å¸ÇØÇƒÇÃÕﬁ∏ƒŸ
				cost = cos(theta + DX_PI / 2);
				sint = sin(theta + DX_PI / 2);

				size = Vector2((scisSize.height / 2) * cost, (scisSize.height / 2) * sint);
				sizePos = (vCnt < 2 ? Vector2() : size * 2);
				sizePos = (sCnt % 2 ? -sizePos : sizePos);

				cost = cos(theta);
				sint = sin(theta);

				auto offset = -Vector2(scisSize.height / 3 * cost, scisSize.height / 3 * sint);
				auto lengPos = (vCnt != 0 && vCnt != 3 ? Vector2(scisSize.width * cost, scisSize.width * sint)
													   : Vector2());
				(*vert) = sPos + sizePos + offset + lengPos;
			}
			theta = atan2f((*scis)[2].y - (*scis)[0].y, (*scis)[2].x - (*scis)[0].x);
			cost = cos(theta);
			sint = sin(theta);
			_scisCenter[sCnt] = (*scis)[0] + Vector2(scisSize.width / 2 * cost, scisSize.width / 2 * sint);
		}
	}
}

void Crab::scisRota()
{
	auto aSize = _crab._arms.size();
	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		auto vert = _scissors[sCnt].begin();
		if (_crab._arms[sCnt / aSize]._vel.x != 0)

		for (; vert != _scissors[sCnt].end(); ++vert)
		{
			auto vCnt = vert - _scissors[sCnt].begin();
			if (vCnt > 0)
			{
				auto center = (sCnt % aSize ? _scissors[sCnt][0] : _scissors[sCnt][3]);
				auto rot = (sCnt % aSize ? revRotDir : rotDir);
				///	ÇÕÇ≥Çﬁéûä‘ÇÃí≤êÆÇÇµÇƒÇ¢ÇÈ
				if (pitchCnt <= pitchMax / 2)
				{
					rot = (sCnt % aSize ? rotDir : revRotDir);
				}
				else
				{
					ChangeVolumeSoundMem(255 * 300 / 180, SE.pitch);
					PlaySoundMem(SE.pitch, DX_PLAYTYPE_BACK);
				}
				auto mat = MGetTranslate((-center).V_Cast());
				mat = MMult(mat, MGetRotAxis(rot, rotVel * 5));
				mat = MMult(mat, MGetTranslate(center.V_Cast()));
				(*vert) = VTransform((*vert).V_Cast(), mat);
				if (vCnt == 3)
				{
					_scisCenter[sCnt] = VTransform(_scisCenter[sCnt].V_Cast(), mat);
				}
			}
		}
	}

	if (pitchCnt < 0)
	{
		_type = AtkType::NORMAL;
		return;
	}
	pitchCnt--;
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotDir, rotVel));
	mat = MMult(mat, MGetTranslate(cPos.V_Cast()));
	/// äIñ{ëÃÇÃâÒì]
	_crab._pos = VTransform(_crab._pos.V_Cast(), mat);

	/// äIÇÃí∏ì_ÇÃâÒì]
	for (int i = 0; i < _crab._vert.size(); ++i)
	{
		_crab._vert[i] = VTransform(_crab._vert[i].V_Cast(), mat);
	}

	/// ãrÇÃâÒì]
	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		auto cnt = leg - _crab._legs.begin();
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

	/// òrÇÃâÒì]
	for (auto& arm : _crab._arms)
	{
		for (auto& point : arm._points)
		{
			point = VTransform(point.V_Cast(), mat);
		}
		arm._ctlPoint = VTransform(arm._ctlPoint.V_Cast(), mat);
	}
	_armPrePos = VTransform(_armPrePos.V_Cast(), mat);

	/// ÇÕÇ≥Ç›ÇÃâÒì]
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
	Vector2 vec;												// à⁄ìÆÇ∑ÇÈï˚å¸ÇÃÕﬁ∏ƒŸéÊìæóp
	auto dirVec = (_crab._vert[1] - _crab._vert[0]).Normalized();	// à⁄ìÆï˚å¸ÇÃÕﬁ∏ƒŸ	
	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		auto rand = GetRand(4);			/// â¡ë¨ìxí≤êÆópÇÃóêêî
		auto cnt = leg - _crab._legs.begin();
		if (!(cnt / (_crab._legs.size() / 2)))
		{	/// âEãr
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * dirVec.x, (length / 2) * dirVec.y);
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * dirVec.x,
									  (mVel + _legAccel[cnt].y) * dirVec.y);
			
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ãrÇÃñﬂÇ∑à íuê›íË
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					PlaySoundMem(SE.walk, DX_PLAYTYPE_BACK);
					auto vec = ((*leg)._points[2] - (*leg)._points[0]).Normalized();
					auto lengPos = Vector2((length / 2) * vec.x, (length / 2) * vec.y);
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._points[0] + lengPos;
					(*leg)._vel = -(*leg)._vel;
					continue;
				}
				vec = (_legMovePos[cnt] - (*leg)._ctlPoint).Normalized();
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * vec.x,
									  (mVel + _legAccel[cnt].y) * vec.y);
			}
			else
			{
				/// ãrÇÃà⁄ìÆÇ∑ÇÈà íuê›íË
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					PlaySoundMem(SE.walk, DX_PLAYTYPE_BACK);
					_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * dirVec.x, length * dirVec.y);
					(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * dirVec.x,
										  (mVel + _legAccel[cnt].y) * dirVec.y);
					continue;
				}
				vec = (_legPrePos[cnt] - (*leg)._ctlPoint).Normalized();
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * vec.x,
									  (mVel + _legAccel[cnt].y) * vec.y);
			}
		}
		else
		{	/// ç∂ãr
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * -dirVec.x, (length / 2) * -dirVec.y);
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * -dirVec.x,
									  (mVel + _legAccel[cnt].y) * -dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// ãrÇÃñﬂÇ∑à íuê›íË
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
					PlaySoundMem(SE.walk, DX_PLAYTYPE_BACK);
					auto vec = ((*leg)._points[2] - (*leg)._points[0]).Normalized();
					auto lengPos = Vector2((length / 2) * vec.x, (length / 2) * vec.y);
					_legMovePos[cnt] = Vector2();
					_legPrePos[cnt] = (*leg)._points[0] + lengPos;
					(*leg)._vel = -(*leg)._vel;
					continue;
				}
				vec = (_legMovePos[cnt] - (*leg)._ctlPoint).Normalized();
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * vec.x,
					(mVel + _legAccel[cnt].y) * vec.y);
			}
			else
			{
				/// ãrÇÃà⁄ìÆÇ∑ÇÈà íuê›íË
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					PlaySoundMem(SE.walk, DX_PLAYTYPE_BACK);
					_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
					_legPrePos[cnt] = Vector2();
					_legMovePos[cnt] = (*leg)._ctlPoint + Vector2(length * -dirVec.x, length * -dirVec.y);
					(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * -dirVec.x,
						(mVel + _legAccel[cnt].y) * -dirVec.y);
					continue;
				}
				vec = (_legPrePos[cnt] - (*leg)._ctlPoint).Normalized();
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * vec.x,
									  (mVel + _legAccel[cnt].y) * vec.y);
			}
		}
		(*leg)._ctlPoint += Vector2((*leg)._vel.x, (*leg)._vel.y);
	}
}

void Crab::MoveJoint()
{
	Vector2 pLength;
	float sint, cost, rad;

	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		pLength = (*leg)._ctlPoint - (*leg)._points[0];
		cost = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
		rad  = acos(cost);
		sint = sin(rad);

		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < length * 2)
			{
				auto cnt = leg - _crab._legs.begin();
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y).Normalized();

				auto cosD = pLength.Normalized() * (length * cost);
				auto sinD = cross2f * (length * sint);

				sinD = (!(cnt / (_crab._legs.size() / 2)) ? sinD : -sinD);

				(*leg)._points[1] = (*leg)._points[0] + cosD + sinD;
				(*leg)._points[2] = (*leg)._ctlPoint;
			}
			else
			{
				(*leg)._points[1] = (*leg)._points[0] + (pLength.Normalized() * length);
				(*leg)._points[2] = (*leg)._points[1] + (pLength.Normalized() * length);
			}
		}
	}

	auto arm = _crab._arms.begin();
	for (; arm != _crab._arms.end(); ++arm)
	{
		pLength = (*arm)._ctlPoint - (*arm)._points[0];
		cost = (pow(aLength, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(aLength, 2.0)) / (2 * aLength * pLength.Magnitude());
		rad = acos(cost);
		sint = sin(rad);
		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < aLength * 2)
			{
				auto cnt = arm - _crab._arms.begin();

				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y).Normalized();

				auto cosD = pLength.Normalized() * (aLength * cost);
				auto sinD = cross2f * (aLength * sint);

				sinD = (cnt / (_crab._arms.size() / 2) ? sinD : -sinD);
				(*arm)._points[1] = (*arm)._points[0] + cosD + sinD;
				(*arm)._points[2] = (*arm)._ctlPoint;
			}
			else
			{
				(*arm)._points[1] = (*arm)._points[0] + (pLength.Normalized() * aLength);
				(*arm)._points[2] = (*arm)._points[1] + (pLength.Normalized() * aLength);
			}
		}
	}
}

void Crab::ShotDelete()
{
	auto s = shot.begin();
	for (; s != shot.end(); ++s)
	{
		if ((*s)._pos.x < 0 || (*s)._pos.y < 0 ||
			(*s)._pos.x > Stage::GetInstance().GetStageSize().x ||
			(*s)._pos.y > Stage::GetInstance().GetStageSize().y)
		{
			shot.erase(s);
			break;
		}
	}
}

void Crab::RegistAtkInfo()
{
	if (at.size() > 0){	at.clear();}

	for (auto leg : _crab._legs)
	{
		for (auto point : leg._points)
		{
			at.emplace_back(AttackInfo(point, lSize.height));
		}
	}
	for (auto arm : _crab._arms)
	{
		auto point = arm._points.begin();
		for (; point != arm._points.end(); ++point)
		{
			auto cnt = point - arm._points.begin();
			auto size = (cnt == 1 ? lSize.width / 4 : lSize.width / 2);
			at.emplace_back(AttackInfo((*point), size));
		}
	}
	
	auto vec = (_crab._vert[1] - _crab._vert[0]).Normalized();
	for (int i = 0; i < 2; ++i)
	{
		vec = (i % 2 ? -vec : vec);
		at.emplace_back(AttackInfo(_crab._pos + Vector2(_crab._size.height / 3 * vec.x, _crab._size.height / 3 * vec.y), _crab._size.height / 2));
	}
}

void Crab::RegistDamageInfo()
{
	if (da.size() > 0)	{ da.clear(); }

	auto vec = (_crab._vert[1] - _crab._vert[0]).Normalized();
	for (int i = 0; i < 2; ++i)
	{
		vec = (i % 2 ? -vec : vec);
		da.emplace_back(DamageInfo(_crab._pos + Vector2(_crab._size.height / 3 * vec.x, _crab._size.height / 3 * vec.y), _crab._size.height / 2));
	}

	for (auto arm : _crab._arms)
	{
		da.emplace_back(DamageInfo(arm._points[2], lSize.width / 2));
	}

}

void Crab::ChangeAtkMode()
{
	if (_updater == &Crab::NeutralUpdate && atkCnt < 0)
	{
		auto mCnt = 0;
		auto moveFlag = false;

		auto arm = _crab._arms.begin();
		for (; arm != _crab._arms.end(); ++arm)
		{
			auto aCnt = arm - _crab._arms.begin();
			auto sPoint = _player->GetInfo().center - (*arm)._ctlPoint;
			auto leng = Vector2(abs((aLength + aLength / 2) * sPoint.Normalized().x),
				abs((aLength + aLength / 2) * sPoint.Normalized().y));
			if (abs(sPoint.x) <= leng.x || abs(sPoint.y) <= leng.y)
			{
				if (_plPos.x == 0 && _plPos.y == 0)
				{
					moveFlag = true;
					_plPos = _player->GetInfo().center;
					mCnt = aCnt;
				}
				else
				{
					auto d = _plPos - (*arm)._ctlPoint;
					mCnt = (abs(d.x) - abs(sPoint.x) < 0 || abs(d.x) - abs(sPoint.y) ? aCnt : mCnt);
				}
			}
		}
		if (moveFlag)
		{
			Pitch();
			auto vec = (_plPos - _crab._arms[mCnt]._ctlPoint).Normalized();
			_crab._arms[mCnt]._vel = Vector2(mVel * vec.x, mVel * vec.y);
			/// à⁄ìÆëOÇÃêßå‰ì_Çï€ë∂
			_armPrePos = _crab._arms[mCnt]._ctlPoint;
			return;
		}
		if (_updater == &Crab::NeutralUpdate)
		{
			Shot();
			_plPos = _player->GetInfo().center;
			shotCnt = shotMax;
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

void Crab::Draw()
{
	auto c = _camera->CameraCorrection();


	for (auto shot : shot)
	{
		DxLib::DrawCircle(shot._pos.x - c.x, shot._pos.y - c.y, shot._r, 0xccffff, true);
	}

	/// ñ≥ìGèÛë‘ÇÃéûÅAìßñæìxÇí≤êÆÇ∑ÇÈ
	if (inviCnt > 0)
	{
		if ((inviCnt / 10) % 2)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		}
	}

	if (_updater == &Crab::DieUpdate)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendCnt);

		quake.x = (GetRand(1) ? quake.x : -quake.x);
		quake.y = (GetRand(1) ? quake.y : -quake.y);
	}

	Vector2 p1, p2, p3, p4;
	for (auto leg : _crab._legs)
	{
		for (int i = 0; i < leg._points.size() - 1; ++i)
		{
			p1 = leg._vert[i][0] - c + quake; p2 = leg._vert[i][1] - c + quake;
			p3 = leg._vert[i][2] - c + quake; p4 = leg._vert[i][3] - c + quake;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		p1 = (*scis)[0] - c + quake; p2 = (*scis)[1] - c + quake;
		p3 = (*scis)[2] - c + quake; p4 = (*scis)[3] - c + quake;

		auto vec = (p1 - p4).Normalized();
		auto vPos = _scisCenter[sCnt] + Vector2((scisSize.width / 3) * vec.x, (scisSize.width / 3) * vec.y) - c + quake;

		DrawTriangleAA(p1.x, p1.y, vPos.x, vPos.y, p2.x, p2.y, 0xdd0000 , true);
		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdd0000, true);
	}

	for (auto arm : _crab._arms)
	{
		for (int i = 0; i < arm._points.size() - 1; ++i)
		{
			p1 = arm._vert[i][0] - c + quake; p2 = arm._vert[i][1] - c + quake;
			p3 = arm._vert[i][2] - c + quake; p4 = arm._vert[i][3] - c + quake;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}
	/// äIñ{ëÃÇÃï`âÊ
	p1 = _crab._vert[0] - c + quake; p2 = _crab._vert[1] - c + quake;
	p3 = _crab._vert[2] - c + quake; p4 = _crab._vert[3] - c + quake;
	DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

	auto vec = (p2 - p1).Normalized();
	auto rEyePos = p1 + Vector2((_crab._size.width / 3) * vec.x, (_crab._size.width / 3) * vec.y);
	auto lEyePos = p2 + Vector2((_crab._size.width / 3) * (-vec.x), (_crab._size.width / 3) * (-vec.y));

	/// ñ⁄ÇÃï`âÊ
	DxLib::DrawCircle(rEyePos.x, rEyePos.y, 5 * magRate, 0x000000, true);
	DxLib::DrawCircle(lEyePos.x, lEyePos.y, 5 * magRate, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (_particle != nullptr)
	{
		_particle->Draw();
	}
	

#ifdef _DEBUG
	DebugDraw(c);
#endif
}

void Crab::ShadowDraw()
{
	auto c = _camera->CameraCorrection();
	auto s = _camera->GetShadowPos(0.3f);

	for (auto shot : shot)
	{
		DxLib::DrawCircle(shot._pos.x - c.x + s.x, shot._pos.y - c.y + s.y, shot._r, 0xccffff, true);
	}

	Vector2 p1, p2, p3, p4;
	for (auto leg : _crab._legs)
	{
		for (int i = 0; i < leg._points.size() - 1; ++i)
		{
			p1 = leg._vert[i][0] - c + s; p2 = leg._vert[i][1] - c + s;
			p3 = leg._vert[i][2] - c + s; p4 = leg._vert[i][3] - c + s;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		p1 = (*scis)[0] - c + s; p2 = (*scis)[1] - c + s;
		p3 = (*scis)[2] - c + s; p4 = (*scis)[3] - c + s;

		auto vec = (p1 - p4).Normalized();
		auto vPos = _scisCenter[sCnt] + Vector2((scisSize.width / 3) * vec.x, (scisSize.width / 3) * vec.y) - c + s;

		DrawTriangleAA(p1.x, p1.y, vPos.x, vPos.y, p2.x, p2.y, 0xdd0000, true);
		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdd0000, true);
	}

	for (auto arm : _crab._arms)
	{
		for (int i = 0; i < arm._points.size() - 1; ++i)
		{
			p1 = arm._vert[i][0] - c + s; p2 = arm._vert[i][1] - c + s;
			p3 = arm._vert[i][2] - c + s; p4 = arm._vert[i][3] - c + s;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}
	/// äIñ{ëÃÇÃï`âÊ
	p1 = _crab._vert[0] - c + s; p2 = _crab._vert[1] - c + s;
	p3 = _crab._vert[2] - c + s; p4 = _crab._vert[3] - c + s;
	DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

	auto vec = (p2 - p1).Normalized();
	auto rEyePos = p1 + Vector2((_crab._size.width / 3) * vec.x, (_crab._size.width / 3) * vec.y);
	auto lEyePos = p2 + Vector2((_crab._size.width / 3) * (-vec.x), (_crab._size.width / 3) * (-vec.y));

}

void Crab::SelectDraw(const Vector2 & pos, const float& scale)
{
	auto camera = _camera->CameraCorrection();

	_crab._pos = pos;
	_crab._size = Size(eSize.x * scale, eSize.y * scale);
	lSize = Size(100 * scale, 20 * scale);
	scisSize = Size(70 * scale, lSize.height);
	length = lSize.width * scale;
	aLength = (lSize.width + (60 * scale)) * scale;

	BodyInit();
	LegInit();
	ArmInit();
	MoveJoint();
	CalVert();

	Vector2 p1, p2, p3, p4;
	for (auto leg : _crab._legs)
	{
		for (int i = 0; i < leg._points.size() - 1; ++i)
		{
			p1 = leg._vert[i][0]; p2 = leg._vert[i][1]; p3 = leg._vert[i][2]; p4 = leg._vert[i][3];
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		p1 = (*scis)[0]; p2 = (*scis)[1]; p3 = (*scis)[2];  p4 = (*scis)[3];

		auto vec = (p1 - p4).Normalized();
		auto vPos = _scisCenter[sCnt] + Vector2((scisSize.width / 3) * vec.x, (scisSize.width / 3) * vec.y) - camera;
		DrawTriangleAA(p1.x, p1.y, vPos.x, vPos.y, p2.x, p2.y, 0xdd0000, true);
		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdd0000, true);
	}

	for (auto arm : _crab._arms)
	{
		for (int i = 0; i < arm._points.size() - 1; ++i)
		{
			p1 = arm._vert[i][0]; p2 = arm._vert[i][1]; p3 = arm._vert[i][2]; p4 = arm._vert[i][3];
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}
	/// äIñ{ëÃÇÃï`âÊ
	p1 = _crab._vert[0]; p2 = _crab._vert[1]; p3 = _crab._vert[2]; p4 = _crab._vert[3];
	DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

	auto vec = (p2 - p1).Normalized();
	auto rEyePos = p1 + Vector2((_crab._size.width / 3) * vec.x, (_crab._size.width / 3) * vec.y);
	auto lEyePos = p2 + Vector2((_crab._size.width / 3) * (-vec.x), (_crab._size.width / 3) * (-vec.y));

	/// ñ⁄ÇÃï`âÊ
	DxLib::DrawCircle(rEyePos.x, rEyePos.y, 5 * scale, 0x000000, true);
	DxLib::DrawCircle(lEyePos.x, lEyePos.y, 5 * scale, 0x000000, true);
}

void Crab::DebugDraw(const Vector2& camera)
{
	/// êßå‰ì_ÇÃï`âÊ
	for (auto leg : _crab._legs)
	{
		DrawCircle(leg._ctlPoint.x - camera.x, leg._ctlPoint.y - camera.y, 4, 0xffff00, true);
	}

	// òrÇ™ìÆÇ≠èIì_Ç‹Ç≈ÇÃãóó£
	for (auto arm : _crab._arms)
	{
		DrawCircle(arm._ctlPoint.x - camera.x, arm._ctlPoint.y - camera.y, 4, 0xffff00, true);
		if (arm._vel.x != 0 && arm._vel.y != 0)
		{
			if (_type == AtkType::NORMAL)
			{
				DxLib::DrawLine(arm._ctlPoint.x - camera.x, arm._ctlPoint.y - camera.y,
					_armPrePos.x - camera.x, _armPrePos.y - camera.y, 0x00ff00, 2);
			}
			else if (_type == AtkType::MOVE)
			{
				DxLib::DrawLine(arm._ctlPoint.x - camera.x, arm._ctlPoint.y - camera.y,
					_plPos.x - camera.x, _plPos.y - camera.y, 0xff5555, 2);
			}
			else {}
		}
	}

	/// à⁄ìÆÇ∑ÇÈë´ÇÃêßå‰ì_ÇÃï`âÊ
	for (int i = 0; i < _crab._legs.size(); ++i)
	{
		DxLib::DrawCircle(_legMovePos[i].x - camera.x, _legMovePos[i].y - camera.y, 4, 0xff0000, true);
		DxLib::DrawCircle(_legPrePos[i].x - camera.x, _legPrePos[i].y - camera.y, 4, 0x00ff00, true);
	}

	/// âÒì]Ç∑ÇÈÇ∆Ç´ÇÃíÜêSì_ÇÃï`âÊ
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);

	for (auto a : at)
	{
		DrawCircle(a._pos.x - camera.x, a._pos.y - camera.y, a._r, 0x55dd00, true);
	}

	for (auto d : da)
	{
		DrawCircle(d._pos.x - camera.x, d._pos.y - camera.y, d._r, 0x0000dd);
	}

	for (auto s : shot)
	{
		DrawCircle(s._pos.x - camera.x, s._pos.y - camera.y, s._r, 0x55dd00);
	}
}

void Crab::OnDamage()
{
	if (inviCnt <= 0)
	{
		_lifeCnt--;
		if (_lifeCnt == 0)
		{
			Die();
			ChangeVolumeSoundMem((255 * 120) / 100, SE.die);
			PlaySoundMem(SE.die, DX_PLAYTYPE_BACK);
			return;
		}
		PlaySoundMem(SE.damage, DX_PLAYTYPE_BACK);
		inviCnt = inviMax;
	}

}

void Crab::HitBlock()
{
	/// âΩÇ‡ÇµÇ»Ç¢
}

void Crab::Update()
{
	///Å@Ç∆ÇËÇ†Ç¶Ç∏âºÇ≈ó¨ÇµÇƒÇ¢ÇÈ
	if (!CheckSoundMem(BGM))
	{
		ChangeVolumeSoundMem(255 * 200 / 180, BGM);
		PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
	}
	(this->*_updater)();
	if (_updater != &Crab::DieUpdate)
	{
		inviCnt = (inviCnt <= 0 ? inviCnt : inviCnt - 1);
		ChangeAtkMode();
		if (!shot.empty()) { ShotDelete(); }
		
		/// òrÇÃà⁄ìÆ
		for (auto& arm : _crab._arms)
		{
			if (_type != AtkType::PITCH)
				arm._ctlPoint += arm._vel;
		}
		/// ºÆØƒÇÃà⁄ìÆ
		for (auto& shot : shot)
		{
			shot._pos += shot._vel;
		}
		if (_type == AtkType::NORMAL && atkCnt >= 0)
		{
			Rotation();
			MoveLeg();
		}
		CalVert();			/// ãÈå`ÇÃí∏ì_Çê›íË
		MoveJoint();
		RegistAtkInfo();
		RegistDamageInfo();
	}
}