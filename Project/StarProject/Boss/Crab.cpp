#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(3.f, 3.f);	// 脚の動く速度
const Vector2 fVel  = Vector2(5.f, 5.f);	// 殴る速度
const VECTOR rotDir = { 0,0,1.f };								// 回転方向
const VECTOR revRotDir = { -rotDir.x, -rotDir.y, -rotDir.z };	// 逆の回転方向
const float rotVel  = DX_PI_F / 540.f;
const int length    = 100;
const int aLength	= length + 60;
const int typeMax   = static_cast<int>(AtkType::MAX);
const int invCntMax	= 60;
const Size eSize    = Size(250, 150);			// 蟹の大きさ
const Size lSize    = Size(length, 20);			// 脚の大きさ
const Size scisSize = Size(60, 25);		// はさみの大きさ				

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_plPos	   = Vector2();
	_armPrePos = Vector2();
	atkInvCnt  = invCntMax;
	_type	   = AtkType::NORMAL;

	/// 蟹の初期化
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
	/// 蟹本体の初期化
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
	/// 脚の初期化
	boss._crab._legs.resize(8);
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		/// 関節を追加している
		(*leg)._points.resize(3);
		/// 移動制御用の座標
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
	/// 腕の初期化
	boss._crab._arms.resize(2);
	/// はさみの初期化
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
		/// ﾗﾝﾀﾞﾑで攻撃ﾊﾟﾀｰﾝを決定
		// _type = static_cast<AtkType>(GetRand(typeMax - 1));

		/// debug用
		_type = static_cast<AtkType>(3);	/// 殴る攻撃
		 //type = static_cast<AtkType>(2);	/// 挟む攻撃
		//_type = static_cast<AtkType>(1);	/// 泡の攻撃
	}
	
}

void Crab::FistUpdate()
{
	if (_type == AtkType::NORMAL)
	{
		/// 腕を引き戻す時
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
		/// 腕を伸ばす時
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

	/// 脚の矩形設定
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
	/// 腕の矩形設定
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		auto aCnt = arm - boss._crab._arms.begin();
		for (int i = 0; i < (*arm)._vert[0].size(); ++i)
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
				sizePos = (i < 2 ? -size : size);

				(*arm)._vert[cnt - 1][i] = (i == 0 || i == 3 ? (*arm)._points[cnt - 1] : (*point)) + sizePos;
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
			cost = cos(theta);
			sint = sin(theta);
			(*arm)._vert[1][i] = (*arm)._vert[1][i] + Vector2(length / 4 * cost, length / 4 * sint);

		}
	}

	if (_type != AtkType::PITCH)
	{
		/// はさみの爪の矩形設定
		auto aSize = boss._crab._arms.size();		/// 腕の数
		auto scis = _scissors.begin();
		for (; scis != _scissors.end(); ++scis)
		{
			auto sCnt = scis - _scissors.begin();
			auto vert = _scissors[sCnt].begin();
			for (; vert != _scissors[sCnt].end(); ++vert)
			{
				auto vCnt = vert - _scissors[sCnt].begin();
				auto sPos = boss._crab._arms[sCnt / aSize]._vert[1][(sCnt % 2) + 1];

				theta = dirTheta[sCnt / aSize];		/// 腕の中間点から先端に向けてのﾍﾞｸﾄﾙ
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
			theta = atan2f((*scis)[2].y - (*scis)[0].y, (*scis)[2].x - (*scis)[0].x);
			cost = cos(theta);
			sint = sin(theta);

			_scisCenter[sCnt] = (*scis)[0] + Vector2(scisSize.width / 2 * cost, scisSize.width / 2 * sint);
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
				///	はさむ時間の調整をしている
				if (rotInvCnt <= invCntMax / 2)
				{
					rot = (sCnt % 2 ? rotDir : revRotDir);
				}
				auto mat = MGetTranslate((-center).V_Cast());		/// 回転する中心点の設定
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

	if (rotInvCnt < 0)
	{
		_type = AtkType::NORMAL;
		return;
	}
	rotInvCnt--;
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotDir, rotVel)); 	 // Z軸の回転行列を作っている
	mat = MMult(mat, MGetTranslate(cPos.V_Cast()));
	/// 蟹本体の回転
	boss._crab._pos = VTransform(boss._crab._pos.V_Cast(), mat);

	/// 蟹の頂点の回転
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		boss._crab._vert[i] = VTransform(boss._crab._vert[i].V_Cast(), mat);
	}

	/// 脚の回転
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

	/// 腕の回転
	for (auto& arm : boss._crab._arms)
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
	auto dirVec = boss._crab._vert[1] - boss._crab._vert[0];	// 移動方向のﾍﾞｸﾄﾙ	
	dirVec.Normalize();

	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// 右足の動き
			/// 脚を伸ばす先が決まっていない時、移動先を登録する
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * dirVec.x, (length / 2) * dirVec.y);

				(*leg)._vel = Vector2(lVel.x * dirVec.x, lVel.y * dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚を伸ばす
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
				/// 脚を戻す
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
			/// 脚を伸ばす先が決まっていない時、移動先を登録する
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._points[2] + Vector2((length / 2) * -dirVec.x, (length / 2) * -dirVec.y);
				(*leg)._vel = Vector2(lVel.x * -dirVec.x, lVel.y * -dirVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚を伸ばす
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
				/// 脚を戻す
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

	/// 脚関節の移動
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
				auto cnt = leg - boss._crab._legs.begin();	/// 足の番地指定用

				/// Y軸の方向ﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (length * cost);		/// X方向の成分
				auto sinD = cross2f * (length * sint);					/// Y方向の成分

				/// 左関節が逆に向かないための処理
				sinD = (!(cnt / (boss._crab._legs.size() / 2)) ? sinD : -sinD);

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
				auto cnt = arm - boss._crab._arms.begin();	/// 足の番地指定用

				/// Y軸の方向ﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (aLength * cost);	/// X方向の成分
				auto sinD = cross2f * (aLength * sint);					/// Y方向の成分

				/// 左関節が逆に向かないための処理
				sinD = (cnt / (boss._crab._arms.size() / 2) ? sinD : -sinD);

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

	///	ここに泡攻撃する時の処理も追加しておく
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
				_armPrePos = (*arm)._ctlPoint;			/// 移動前の制御点を保存
			}
		}
		Fist();
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// 脚の描画(後で修正)
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

	/// はさみの描画
	for (auto scissor : _scissors)
	{
		p1 = scissor[0] - camera;
		p2 = scissor[1] - camera;
		p3 = scissor[2] - camera;
		p4 = scissor[3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xdddd0000, true);
	}
	
	/// 腕の描画
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

	/// 蟹本体の描画
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
	/// 制御点の描画
	for (auto leg : boss._crab._legs)
	{
		DrawCircle(leg._ctlPoint.x - camera.x, leg._ctlPoint.y - camera.y, 4, 0xffff00, true);
	}

	// 腕が動く終点までの距離
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

	/// 移動する足の制御点の描画
	for (int i = 0; i < boss._crab._legs.size(); ++i)
	{
		DxLib::DrawCircle(_legMovePos[i].x, _legMovePos[i].y, 4, 0xff0000, true);
		DxLib::DrawCircle(_legPrePos[i].x, _legPrePos[i].y, 4, 0x00ff00, true);
	}

	/// 中心点の描画
	for (int i = 0; i < _scisCenter.size(); ++i)
	{
		DxLib::DrawCircle(_scisCenter[i].x, _scisCenter[i].y, 4, 0x00ff00, true);
	}

	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		auto sPos = (*scis)[0];

		DrawLine(sPos.x, sPos.y, _scisCenter[sCnt].x, _scisCenter[sCnt].y, 0xffff00);
	}

	/// 回転するときの中心点の描画
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);
}

void Crab::Update()
{
	(this->*_updater)();

	/// 腕の移動
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
	
	CalVert();			/// 矩形の頂点を設定
	
	MoveJoint();
}
