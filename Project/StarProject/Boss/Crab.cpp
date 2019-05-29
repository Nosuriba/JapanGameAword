#include "Crab.h"
#include "../Game.h"


const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const VECTOR rotDir = { 0,0,1.f };								// 回転方向
const VECTOR revRotDir = { -rotDir.x, -rotDir.y, -rotDir.z };	// 逆の回転方向
const Vector2 eSize = Vector2(250, 150);
const float rotVel  = DX_PI_F / 540.f;
const float mVel	= 3.f;
const int atkMax	= 120;
const int pitchMax	= 50;
const int shotMax	= 240;

Crab::Crab(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p) : Boss(c, p)
{
	_plPos	   = Vector2();
	_armPrePos = Vector2();
	atkCnt	   = atkMax;
	_type	   = AtkType::NORMAL;
	
	/// ｻｲｽﾞ設定
	_crab._pos  = Vector2(600, 750);
	_crab._size = Size(eSize.x, eSize.y);
	lSize	 = Size(100, 20);
	scisSize = Size(70, lSize.height);
	length   = lSize.width;
	aLength  = (lSize.width + 60);

	BodyInit();
	LegInit();
	ArmInit();
	/// 関節の初期化
	MoveJoint();
	CalVert();
	Neutral();
}

Crab::~Crab()
{
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
		/// 移動制御用の座標
		_legPrePos.push_back(Vector2());
		_legMovePos.push_back(Vector2());
		_legAccel.push_back(Vector2());

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
			(*point)  = (*leg)._points[pCnt - 1] + Vector2(leng - (leng / 6), 0 );
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
	/// はさみの初期化
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
	_updater = &Crab::PitchUpdate;
}

void Crab::Shot()
{
	_type = AtkType::SHOT;
	_updater = &Crab::ShotUpdate;
}

void Crab::Die()
{
	_updater = &Crab::DieUpdate;
}

void Crab::NeutralUpdate()
{
	atkCnt--;
}

void Crab::PitchUpdate()
{
	if (_type == AtkType::NORMAL)
	{
		/// 腕を戻している時
		for (auto& arm : _crab._arms)
		{
			if (StopCheck(arm._ctlPoint, _armPrePos, arm._vel) && arm._vel.x != 0)
			{
				Neutral();
				_armPrePos = Vector2();
				arm._vel   = Vector2();
				atkCnt	   = atkMax;
				break;
			}
		}
	}
	else
	{
		/// 腕を伸ばしている時
		if (_type == AtkType::MOVE)
		{
			for (auto& arm : _crab._arms)
			{
				auto d = abs((_armPrePos.x - arm._ctlPoint.x) + (_armPrePos.y - arm._ctlPoint.y));
				if ((StopCheck(arm._ctlPoint, _plPos, arm._vel) || (d > aLength))
					&& arm._vel.x != 0)
				{
					auto vec = (_armPrePos - arm._ctlPoint).Normalized();
					arm._vel   = Vector2(mVel * vec.x, mVel * vec.y);
					_plPos	   = Vector2();
					_type	   = AtkType::PITCH;
					pitchCnt   = pitchMax;
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
		if (!(shotCnt % 10))
		{
			auto vec = (_crab._vert[0] - _crab._vert[3]).Normalized();		/// かにの向いてる方向に向かって放射状
			auto lengPos = Vector2(length * vec.x, length * vec.y);
			auto rand = (GetRand(10) - 5);
			auto pos = Vector2(10 * rand, 10 * rand) + lengPos;
			/// プレイヤーの方向に向かって放射上に打つよう設定を行う
			auto theta = atan2f((_crab._pos.y + pos.y) - _crab._pos.y,
							    (_crab._pos.x + pos.x) - _crab._pos.x);
			auto cost = cos(theta);
			auto sint = sin(theta);
			auto vel = Vector2(5.0f * cost, 5.0f * sint);
			auto size = Size(10, 10);

			_shot.push_back(ShotInfo(_crab._pos, vel, size));
		}
		shotCnt--;
	}
	else
	{
		Neutral();
		atkCnt = atkMax;
		_plPos = Vector2();
		_type  = AtkType::NORMAL;
	}
}

void Crab::DieUpdate()
{
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
		{/// 矩形の頂点設定
			auto point = (*leg)._points.begin() + 1;
			for (; point != (*leg)._points.end(); ++point)
			{
				auto cnt = point - (*leg)._points.begin();
				theta = atan2f((*point).y - (*leg)._points[cnt - 1].y, (*point).x - (*leg)._points[cnt - 1].x);
				cost  = cos(theta + DX_PI / 2);
				sint  = sin(theta + DX_PI / 2);

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

	std::vector<float> dirTheta;		/// はさみの爪の方向用
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
				cost  = cos(theta + DX_PI / 2);
				sint  = sin(theta + DX_PI / 2);

				size = Vector2((lSize.height / 2) * cost, (lSize.height / 2) * sint);
				sizePos = (p < 2 ? -size : size);
				(*arm)._vert[cnt - 1][p] = (p == 0 || p == 3 ? (*arm)._points[cnt - 1] : (*point)) + sizePos;

				if (p >= 3)
				{
					auto vec = ((*arm)._vert[cnt - 1][2] - (*arm)._vert[cnt - 1][0]).Normalized();
					(*arm)._center[cnt - 1] = (*arm)._vert[cnt - 1][0] + Vector2(lSize.width * vec.x, lSize.width * vec.y);
				}
			}
		}
		/// 脚の幅の角度を登録している
		dirTheta.push_back(atan2f((*arm)._vert[1][1].y - (*arm)._vert[1][0].y,
								  (*arm)._vert[1][1].x - (*arm)._vert[1][0].x));

		/// 腕の先端を太くする処理
		for (int i = 1; i <= (*arm)._points.size() - 1; ++i)
		{
			theta = atan2f((*arm)._vert[1][i].y - (*arm)._vert[1][(*arm)._points.size() - i].y,
						   (*arm)._vert[1][i].x - (*arm)._vert[1][(*arm)._points.size() - i].x);
			cost  = cos(theta);
			sint  = sin(theta);
			(*arm)._vert[1][i] = (*arm)._vert[1][i] + Vector2(length / 4 * cost, length / 4 * sint);
		}
	}

	if (_type != AtkType::PITCH)
	{
		auto aSize = _crab._arms.size();		/// 腕の数
		auto scis = _scissors.begin();
		for (; scis != _scissors.end(); ++scis)
		{
			auto sCnt = scis - _scissors.begin();
			auto vert = _scissors[sCnt].begin();
			for (; vert != _scissors[sCnt].end(); ++vert)
			{
				auto vCnt = vert - _scissors[sCnt].begin();
				auto sPos = _crab._arms[sCnt / aSize]._vert[1][(sCnt % 2) + 1];

				theta = dirTheta[sCnt / aSize];		/// 腕の中間点から先端に向けてのﾍﾞｸﾄﾙ
				cost  = cos(theta + DX_PI / 2);
				sint  = sin(theta + DX_PI / 2);

				size	= Vector2((scisSize.height / 2) * cost, (scisSize.height / 2) * sint);
				sizePos = (vCnt < 2 ? Vector2() : size * 2);
				sizePos = (sCnt % 2 ? -sizePos : sizePos);

				cost = cos(theta);
				sint = sin(theta);

				auto offset  = -Vector2(scisSize.height / 3 * cost, scisSize.height / 3 * sint);
				auto lengPos = (vCnt != 0 && vCnt != 3 ? Vector2(scisSize.width * cost, scisSize.width * sint)
													   : Vector2());
				(*vert) = sPos + sizePos + offset + lengPos;
			}
			theta = atan2f((*scis)[2].y - (*scis)[0].y, (*scis)[2].x - (*scis)[0].x);
			cost  = cos(theta);
			sint  = sin(theta);
		}
	}
}

void Crab::scisRota()
{
	auto aSize = _crab._arms.size();
	auto scis  = _scissors.begin();
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
				auto rot	= (sCnt % aSize ? revRotDir : rotDir);
				///	はさむ時間の調整をしている
				if (pitchCnt <= pitchMax / 2)
				{
					rot = (sCnt % aSize ? rotDir : revRotDir);
				}
				auto mat = MGetTranslate((-center).V_Cast());		
				mat		 = MMult(mat, MGetRotAxis(rot, rotVel * 5));	
				mat		 = MMult(mat, MGetTranslate(center.V_Cast()));
				(*vert)  = VTransform((*vert).V_Cast(), mat);
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
	/// 蟹本体の回転
	_crab._pos = VTransform(_crab._pos.V_Cast(), mat);

	/// 蟹の頂点の回転
	for (int i = 0; i < _crab._vert.size(); ++i)
	{
		_crab._vert[i] = VTransform(_crab._vert[i].V_Cast(), mat);
	}

	/// 脚の回転
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

	/// 腕の回転
	for (auto& arm : _crab._arms)
	{
		for (auto& point : arm._points)
		{
			point = VTransform(point.V_Cast(), mat);
		}
		arm._ctlPoint = VTransform(arm._ctlPoint.V_Cast(), mat);
	}
	_armPrePos = VTransform(_armPrePos.V_Cast(), mat);

	/// はさみの回転
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
	Vector2 vec;												// 移動する方向のﾍﾞｸﾄﾙ取得用
	auto dirVec = (_crab._vert[1] - _crab._vert[0]).Normalized();	// 移動方向のﾍﾞｸﾄﾙ	
	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		auto rand = GetRand(4);			/// 加速度調整用の乱数
		auto cnt = leg - _crab._legs.begin();
		if (!(cnt / (_crab._legs.size() / 2)))
		{	/// 右脚
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * dirVec.x, (length / 2) * dirVec.y);
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * dirVec.x,
									  (mVel + _legAccel[cnt].y) * dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚の戻す位置設定
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
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
				/// 脚の移動する位置設定
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legAccel[cnt]	 = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
					_legPrePos[cnt]	 = Vector2();
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
		{	/// 左脚
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legAccel[cnt] = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * -dirVec.x, (length / 2) * -dirVec.y);
				(*leg)._vel = Vector2((mVel + _legAccel[cnt].x) * -dirVec.x,
									  (mVel + _legAccel[cnt].y) * -dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚の戻す位置設定
				if (StopCheck((*leg)._ctlPoint, _legMovePos[cnt], (*leg)._vel))
				{
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
				/// 脚の移動する位置設定
				if (StopCheck((*leg)._ctlPoint, _legPrePos[cnt], (*leg)._vel))
				{
					_legAccel[cnt]   = Vector2((mVel / 10) * rand, (mVel / 10) * rand);
					_legPrePos[cnt]  = Vector2();
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

	/// 脚関節の移動
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
				auto cnt = leg - _crab._legs.begin();	/// 足の番地指定用
				/// Y軸方向のﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y).Normalized();

				auto cosD = pLength.Normalized() * (length * cost);		
				auto sinD = cross2f * (length * sint);				

				/// 左関節が逆に向かないための処理
				sinD = (!(cnt / (_crab._legs.size() / 2)) ? sinD : -sinD);

				(*leg)._points[1] = (*leg)._points[0] + cosD + sinD;
				(*leg)._points[2] = (*leg)._ctlPoint;
			}
			else
			{
				/// 長さが一定距離よりながくなった場合
				(*leg)._points[1] = (*leg)._points[0] + (pLength.Normalized() * length);
				(*leg)._points[2] = (*leg)._points[1] + (pLength.Normalized() * length);
			}
		}
	}

	/// 腕関節の移動
	auto arm = _crab._arms.begin();
	for (; arm != _crab._arms.end(); ++arm)
	{
		pLength = (*arm)._ctlPoint - (*arm)._points[0];
		cost = (pow(aLength, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(aLength, 2.0)) / (2 * aLength * pLength.Magnitude());
		rad  = acos(cost);
		sint = sin(rad);

		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < aLength * 2)
			{
				auto cnt = arm - _crab._arms.begin();	/// 足の番地指定用

				/// Y軸方向のﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y).Normalized();

				auto cosD = pLength.Normalized() * (aLength * cost);	
				auto sinD = cross2f * (aLength * sint);					

				/// 左関節が逆に向かないための処理
				sinD = (cnt / (_crab._arms.size() / 2) ? sinD : -sinD);

				(*arm)._points[1] = (*arm)._points[0] + cosD + sinD;
				(*arm)._points[2] = (*arm)._ctlPoint;
			}
			else
			{
				/// 長さが一定距離よりながくなった場合
				(*arm)._points[1] = (*arm)._points[0] + (pLength.Normalized() * aLength);
				(*arm)._points[2] = (*arm)._points[1] + (pLength.Normalized() * aLength);
			}
		}
	}
}

void Crab::ShotDelete()
{
	for (int i = 0; i < _shot.size(); ++i)
	{
		if (_shot[i]._pos.x < 0 || _shot[i]._pos.y < 0 ||
			_shot[i]._pos.x > Game::GetInstance().GetScreenSize().x ||
			_shot[i]._pos.y > Game::GetInstance().GetScreenSize().y)
		{
			_shot.erase(i + _shot.begin());
		}
	}
}

void Crab::RegistAtkInfo()
{
	at.push_back(AttackInfo(_crab._pos, _crab._size.height / 2));
	
	auto leg = _crab._legs.begin();
	for (; leg != _crab._legs.end(); ++leg)
	{
		auto point = (*leg)._points.begin() + 1;
		for (; point != (*leg)._points.end(); ++point)
		{
			auto cnt = point - (*leg)._points.begin();
			at.push_back(AttackInfo((*leg)._center[cnt - 1], lSize.height / 2));
		}
	}

	/*auto arm = _crab._arms.begin();
	for ()
	*/
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
			/// 移動前の制御点を保存
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
	auto camera = _camera->CameraCorrection();

	for (auto shot : _shot)
	{
		DxLib::DrawCircle(shot._pos.x - camera.x, shot._pos.y - camera.y, 10, 0xccffff, true);
	}

	Vector2 p1, p2, p3, p4;
	for (auto leg : _crab._legs)
	{
		for (int i = 0; i < leg._points.size() - 1; ++i)
		{
			p1 = leg._vert[i][0] - camera; p2 = leg._vert[i][1] - camera;
			p3 = leg._vert[i][2] - camera; p4 = leg._vert[i][3] - camera;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}

	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		p1 = (*scis)[0] - camera; p2 = (*scis)[1] - camera;
		p3 = (*scis)[2] - camera; p4 = (*scis)[3] - camera;

		auto vec = (p1 - p4).Normalized();
		auto vPos = _scisCenter[sCnt] + Vector2((scisSize.width / 3) * vec.x, (scisSize.width / 3) * vec.y) - camera;

		DrawTriangleAA(p1.x, p1.y, vPos.x, vPos.y, p2.x, p2.y, 0xdd0000, true);
		DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdd0000, true);
	}

	for (auto arm : _crab._arms)
	{
		for (int i = 0; i < arm._points.size() - 1; ++i)
		{
			p1 = arm._vert[i][0] - camera; p2 = arm._vert[i][1] - camera; 
			p3 = arm._vert[i][2] - camera; p4 = arm._vert[i][3] - camera;
			DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
		}
	}
	/// 蟹本体の描画
	p1 = _crab._vert[0] - camera; p2 = _crab._vert[1] - camera;
	p3 = _crab._vert[2] - camera; p4 = _crab._vert[3] - camera;
	DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

	auto vec = (p2 - p1).Normalized();
	auto rEyePos = p1 + Vector2((_crab._size.width / 3) * vec.x, (_crab._size.width / 3) * vec.y);
	auto lEyePos = p2 + Vector2((_crab._size.width / 3) * (-vec.x), (_crab._size.width / 3) * (-vec.y));
	
	/// 目の描画
	DxLib::DrawCircle(rEyePos.x, rEyePos.y, 5, 0x000000, true);
	DxLib::DrawCircle(lEyePos.x, lEyePos.y, 5, 0x000000, true);

#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void Crab::SelectDraw(const Vector2 & pos, const float& scale)
{
	auto camera = _camera->CameraCorrection();

	_crab._pos = pos;
	_crab._size = Size(eSize.x * scale, eSize.y * scale);
	lSize	 = Size(100 * scale, 20 * scale);
	scisSize = Size(70 * scale, lSize.height);
	length   = lSize.width * scale;
	aLength  = (lSize.width + (60 * scale)) * scale;

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
	/// 蟹本体の描画
	p1 = _crab._vert[0]; p2 = _crab._vert[1]; p3 = _crab._vert[2]; p4 = _crab._vert[3];
	DxLib::DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

	auto vec = (p2 - p1).Normalized();
	auto rEyePos = p1 + Vector2((_crab._size.width / 3) * vec.x, (_crab._size.width / 3) * vec.y);
	auto lEyePos = p2 + Vector2((_crab._size.width / 3) * (-vec.x), (_crab._size.width / 3) * (-vec.y));

	/// 目の描画
	DxLib::DrawCircle(rEyePos.x, rEyePos.y, 5 * scale, 0x000000, true);
	DxLib::DrawCircle(lEyePos.x, lEyePos.y, 5 * scale, 0x000000, true);
}

void Crab::DebugDraw(const Vector2& camera)
{
	/// 制御点の描画
	for (auto leg : _crab._legs)
	{
		DrawCircle(leg._ctlPoint.x - camera.x, leg._ctlPoint.y - camera.y, 4, 0xffff00, true);
	}

	// 腕が動く終点までの距離
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

	/// 移動する足の制御点の描画
	for (int i = 0; i < _crab._legs.size(); ++i)
	{
		DxLib::DrawCircle(_legMovePos[i].x - camera.x, _legMovePos[i].y - camera.y, 4, 0xff0000, true);
		DxLib::DrawCircle(_legPrePos[i].x  - camera.x, _legPrePos[i].y  - camera.y, 4, 0x00ff00, true);
	}

	/// 回転するときの中心点の描画
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);

	for (auto a : at)
	{
		DrawCircle(a._pos.x - camera.x, a._pos.y - camera.y, a._r, 0x0000ff, true);
	}
}

void Crab::OnDamage()
{
}

void Crab::Update()
{
	(this->*_updater)();
	ChangeAtkMode();

	ShotDelete();

	/// 腕の移動
	for (auto& arm : _crab._arms)
	{
		if (_type != AtkType::PITCH)
		arm._ctlPoint += arm._vel;
	}
	/// ｼｮｯﾄの移動
	for (auto& shot : _shot)
	{
		shot._pos += shot._vel;
	}
	if (_type == AtkType::NORMAL && atkCnt >= 0)
	{
		Rotation();
		MoveLeg();
	}
	CalVert();			/// 矩形の頂点を設定
	MoveJoint();
}