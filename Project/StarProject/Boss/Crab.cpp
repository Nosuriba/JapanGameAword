#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(3.f, 3.f);	// 脚の動く速度
const Vector2 fVel  = Vector2(5.f, 5.f);	// 殴る速度
const VECTOR rotVec = { 0,0,1.f };
const float rotVel  = DX_PI_F / 540.f;
const int length    = 100;
const int aLength	= length + 60;
const int typeMax   = static_cast<int>(AtkType::MAX);
const int atkCnt	= 60;
const Size eSize    = Size(250, 150);			// 蟹の大きさ
const Size lSize    = Size(length, 20);			// 脚の大きさ
const Size scisSize = Size(60, 20);		// はさみの大きさ				

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_plPos	   = Vector2();
	_armPrePos = Vector2();
	atkInvCnt  = atkCnt;
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
		_legPrePos.push_back(Vector2());
		_legMovePos.push_back(Vector2());

		auto cnt = leg - boss._crab._legs.begin();
		if (!(cnt / (boss._crab._legs.size() / 2)))
		{
			/// 右足
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
			/// 左足
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
	/// 腕の初期化
	boss._crab._arms.resize(2);
	/// はさみの初期化
	_scisCenter.resize(boss._crab._arms.size());
	_scissors.resize(_scisCenter.size() * 2);
	auto arm = boss._crab._arms.begin();
	for (; arm != boss._crab._arms.end(); ++arm)
	{
		auto cnt = arm - boss._crab._arms.begin();
		if (!(cnt % 2))
		{
			/// 右腕
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
			/// 左腕
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
				atkInvCnt  = atkCnt;
				break;
			}
		}
	}
	else
	{
		/// 腕を伸ばす時
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

	/// 脚の矩形設定
	for (auto& leg : boss._crab._legs)
	{
		for (int p = 0; p < leg._vert[0].size(); ++p)
		{
			/// 始点から中間点までの矩形の設定
			theta = atan2f(leg._mPoint.y - leg._sPoint.y, leg._mPoint.x - leg._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[0][p] = (p == 0 || p == 3 ? leg._sPoint : leg._mPoint) + sizePos;

			/// 中間点から終点までの矩形の設定
			theta = atan2f(leg._ePoint.y - leg._mPoint.y, leg._ePoint.x - leg._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			leg._vert[1][p] = (p == 0 || p == 3 ? leg._mPoint : leg._ePoint) + sizePos;
		}
	}

	/// 腕の矩形設定
	for (auto& arm : boss._crab._arms)
	{
	
		for (int p = 0; p < arm._vert[0].size(); ++p)
		{
			/// 始点から中間点までの矩形の設定
			theta = atan2f(arm._mPoint.y - arm._sPoint.y, arm._mPoint.x - arm._sPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);

			arm._vert[0][p] = (p == 0 || p == 3 ? arm._sPoint : arm._mPoint) + sizePos;
			

			/// 中間点から終点までの矩形の設定
			theta = atan2f(arm._ePoint.y - arm._mPoint.y, arm._ePoint.x - arm._mPoint.x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			size.x = cost * (lSize.height / 2);
			size.y = sint * (lSize.height / 2);
			sizePos = (p < 2 ? -size : size);
			auto thickSize = Vector2();
			if (p == 1 || p == 2)
			{
				auto upTheta = atan2f(arm._vert[1][1].y - arm._vert[1][0].y,
									  arm._vert[1][1].x - arm._vert[1][0].x);
				auto downTheta = atan2f(arm._vert[1][0].y - arm._vert[1][1].y,
									   arm._vert[1][0].x - arm._vert[1][1].x);
				theta = (p == 2 ? upTheta : downTheta);
				theta =  upTheta ;
				cost = cos(theta);
				sint = sin(theta);
				thickSize = Vector2(0, lSize.height * sint);
			}
			arm._vert[1][p] = (p == 0 || p == 3 ? arm._mPoint : arm._ePoint) + sizePos + thickSize;
			
		}
	}
	std::vector<Vector2> debugList;
	auto aCnt = boss._crab._arms.size();		/// 腕の数
	auto scis = _scissors.begin();
	for (; scis != _scissors.end(); ++scis)
	{
		auto sCnt = scis - _scissors.begin();
		auto vert = _scissors[sCnt].begin();		/// はさみの場所を指定
		for (; vert != _scissors[sCnt].end(); ++vert)
		{
			auto vCnt = vert - _scissors[sCnt].begin();
			/// 始点の設定
			auto sPos = boss._crab._arms[sCnt / aCnt]._vert[1][(sCnt % 2) + 1];

			/// 方向の指定
			theta = atan2f(boss._crab._arms[sCnt / aCnt]._vert[1][1].y - boss._crab._arms[sCnt / aCnt]._vert[1][0].y,
						   boss._crab._arms[sCnt / aCnt]._vert[1][1].x - boss._crab._arms[sCnt / aCnt]._vert[1][0].x);
			cost = cos(theta + DX_PI / 2);
			sint = sin(theta + DX_PI / 2);

			/// 矩形頂点の設定
			size.x = cost * (scisSize.height / 2);
			size.y = sint * (scisSize.height / 2);
			sizePos = (vCnt < 2 ? -size : size);

			/// 距離を出すために、三角比の再設定している
			cost = cos(theta);
			sint = sin(theta);

			/// 頂点からの距離を設定
			auto lengPos = (vCnt != 0 && vCnt != 3 ? Vector2(scisSize.width * cost, scisSize.height * sint) 
												   : Vector2());

			/// 頂点座標の決定
			(*vert) = sPos + sizePos + lengPos;
			debugList.push_back((*vert));
		}
	}
	auto debug = 0;
}

void Crab::Rotation()
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());
	mat = MMult(mat, MGetRotAxis(rotVec, rotVel)); 	 // Z軸の回転行列を作っている
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

	/// 腕の回転
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
	Vector2 vec;		// 移動する方向のﾍﾞｸﾄﾙ取得用

	/// 蟹本体の左頂点から右頂点の方向ﾍﾞｸﾄﾙ(右脚用)
	auto rightVec = boss._crab._vert[1] - boss._crab._vert[0];
	/// 蟹本体の右頂点から左頂点の方向ﾍﾞｸﾄﾙ(左脚用)
	auto leftVec = boss._crab._vert[0] - boss._crab._vert[1];
	rightVec.Normalize();
	leftVec.Normalize();
	
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
				_legMovePos[cnt] = (*leg)._ePoint + Vector2((length / 2) * rightVec.x, (length / 2) * rightVec.y);
				_legPrePos[cnt] = (*leg)._ctlPoint;

				(*leg)._vel = Vector2(lVel.x * rightVec.x, lVel.y * rightVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚を伸ばす
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
				/// 脚を戻す
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
			/// 脚を伸ばす先が決まっていない時、移動先を登録する
			if (_legMovePos[cnt].x == 0 && _legPrePos[cnt].x == 0)
			{
				_legMovePos[cnt] = (*leg)._ePoint + Vector2((length / 2) * leftVec.x, (length / 2) * leftVec.y);
				_legPrePos[cnt] = (*leg)._ctlPoint;

				(*leg)._vel = Vector2(lVel.x * leftVec.x, lVel.y * leftVec.y);
			}

			if (_legMovePos[cnt].x != 0)
			{
				/// 脚を伸ばす
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
				/// 脚を戻す
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

	/// 脚関節の移動
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
				auto cnt = leg - boss._crab._legs.begin();	/// 足の番地指定用

				/// Y軸の方向ﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (length * cost);		/// X方向の成分
				auto sinD = cross2f * (length * sint);					/// Y方向の成分

				/// 左関節が逆に向かないための処理
				sinD = (!(cnt / (boss._crab._legs.size() / 2)) ? sinD : -sinD);

				(*leg)._mPoint = (*leg)._sPoint + cosD + sinD;
				(*leg)._ePoint = (*leg)._ctlPoint;
			}
			else
			{
				/// 長さが一定距離よりながくなった場合
				(*leg)._mPoint = (*leg)._sPoint + (pLength.Normalized() * length);
				(*leg)._ePoint = (*leg)._mPoint + (pLength.Normalized() * length);
			}
		}
	}

	/// 腕関節の移動
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
				auto cnt = arm - boss._crab._arms.begin();	/// 足の番地指定用

				/// Y軸の方向ﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (aLength * cost);	/// X方向の成分
				auto sinD = cross2f * (aLength * sint);					/// Y方向の成分

				/// 左関節が逆に向かないための処理
				sinD = (cnt / (boss._crab._arms.size() / 2) ? sinD : -sinD);

				(*arm)._mPoint = (*arm)._sPoint + cosD + sinD;
				(*arm)._ePoint = (*arm)._ctlPoint;
			}
			else
			{
				/// 長さが一定距離よりながくなった場合
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
				_armPrePos = (*arm)._ctlPoint;			/// 移動前の制御点を保存
			}
		}
		Fist();
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// 脚の描画
	Vector2 p1, p2, p3, p4;
	/*for (auto leg : boss._crab._legs)
	{
		/// 始点から中間までの矩形
		p1 = leg._vert[0][0] - camera;
		p2 = leg._vert[0][1] - camera;
		p3 = leg._vert[0][2] - camera;
		p4 = leg._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// 中間から先端までの矩形
		p1 = leg._vert[1][0] - camera;
		p2 = leg._vert[1][1] - camera;
		p3 = leg._vert[1][2] - camera;
		p4 = leg._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
	}
	*/
	/// 腕の描画
	for (auto arm : boss._crab._arms)
	{
		/// 始点から中間までの矩形
		p1 = arm._vert[0][0] - camera;
		p2 = arm._vert[0][1] - camera;
		p3 = arm._vert[0][2] - camera;
		p4 = arm._vert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// 中間から先端までの矩形
		p1 = arm._vert[1][0] - camera;
		p2 = arm._vert[1][1] - camera;
		p3 = arm._vert[1][2] - camera;
		p4 = arm._vert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);
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

	

	/*/// 蟹本体の描画
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
	/// 制御点の描画
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

	/// 移動する足の制御点の描画
	for (int i = 0; i < boss._crab._legs.size(); ++i)
	{
		DxLib::DrawCircle(_legMovePos[i].x, _legMovePos[i].y, 4, 0xff0000, true);
		DxLib::DrawCircle(_legPrePos[i].x, _legPrePos[i].y, 4, 0x00ff00, true);
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
		//arm._ctlPoint += arm._vel;
	}

	//if (_type == AtkType::NORMAL && atkInvCnt >= 0)
	//{
	//	Rotation();
	//	MoveLeg();
	//}
	//CalVert();			/// 矩形の頂点を設定
	//
	//MoveJoint();

	boss._crab._prePos = boss._crab._pos;
}
