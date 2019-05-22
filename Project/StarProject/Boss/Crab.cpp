#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel  = Vector2(5.f, 5.f);
const VECTOR rotVec = { 0,0,1.f };
const float rotVel  = DX_PI_F / 540.f;
const int length    = 150;
const int aLength	= 250;
const Size eSize    = Size(450, 300);
const Size lSize    = Size(length, 30);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
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
		
	/// 足の初期化
	boss._crab._legs.resize(8);
	for (int i = 0; i < boss._crab._legs.size(); ++i)
	{
		if (!(i / (boss._crab._legs.size() / 2)))
		{
			/// 右足
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 2,
							   boss._crab._pos.y - eSize.height / 4);
			boss._crab._legs[i]._sPoint = pos + Vector2(0,(i * (lSize.height * 2)));
			boss._crab._legs[i]._mPoint = boss._crab._legs[i]._sPoint	+ Vector2(length, 0);
			boss._crab._legs[i]._ePoint = boss._crab._legs[i]._mPoint	+ Vector2(length, 0);
			boss._crab._legs[i]._ctlPoint = boss._crab._legs[i]._ePoint - Vector2(length / 2, 0);

			boss._crab._legs[i]._vel = Vector2(lVel.x, lVel.y);
		}
		else
		{
			/// 左足
			auto pos = Vector2(boss._crab._pos.x - eSize.width  / 2,
							   boss._crab._pos.y - eSize.height / 4);
			boss._crab._legs[i]._sPoint = pos + Vector2(0,(i % (boss._crab._legs.size() / 2)) * (lSize.height * 2));
			boss._crab._legs[i]._mPoint = boss._crab._legs[i]._sPoint	- Vector2(length, 0);
			boss._crab._legs[i]._ePoint = boss._crab._legs[i]._mPoint	- Vector2(length, 0);
			boss._crab._legs[i]._ctlPoint = boss._crab._legs[i]._ePoint + Vector2(length, 0);

			boss._crab._legs[i]._vel = Vector2(lVel.x, -lVel.y);
		}
	}

	/// 腕の初期化
	boss._crab._arms.resize(2);
	for (int i = 0; i < boss._crab._arms.size(); ++i)
	{
		
		if (!(i % 2))
		{
			/// 右腕
			auto pos = Vector2(boss._crab._pos.x + eSize.width  / 4,
							   boss._crab._pos.x - eSize.height / 2);
			boss._crab._arms[i]._sPoint = pos;
			boss._crab._arms[i]._mPoint = boss._crab._arms[i]._sPoint	- Vector2(0, aLength);
			boss._crab._arms[i]._ePoint = boss._crab._arms[i]._mPoint	- Vector2(0, aLength);
			boss._crab._arms[i]._ctlPoint = boss._crab._arms[i]._ePoint + Vector2(0, aLength);
		}
		else
		{
			/// 左腕
			auto pos = Vector2(boss._crab._pos.x - eSize.width  / 4,
							   boss._crab._pos.x - eSize.height / 2);
			boss._crab._arms[i]._sPoint = pos;
			boss._crab._arms[i]._mPoint = boss._crab._arms[i]._sPoint	- Vector2(0, aLength);
			boss._crab._arms[i]._ePoint = boss._crab._arms[i]._mPoint	- Vector2(0, aLength);
			boss._crab._arms[i]._ctlPoint = boss._crab._arms[i]._ePoint + Vector2(aLength, 0);
		}
	}
	Neutral();
}

Crab::~Crab()
{
}

void Crab::Neutral()
{
	_updater = &Crab::NeutralUpdate;
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

	/// 腕の矩形設定(幅などは足と同じなので、修正が必要であれば必ずする)
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

			arm._vert[1][p] = (p == 0 || p == 3 ? arm._mPoint : arm._ePoint) + sizePos;
		}
	}
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
}

void Crab::Rotation(const int & l)
{
	auto cPos = center;
	auto mat = MGetTranslate((-cPos).V_Cast());		 // 平行移動をしている
	mat = MMult(mat, MGetRotAxis(rotVec, rotVel)); 	 // Z軸の回転行列を作っている
	mat = MMult(mat, MGetTranslate(cPos.V_Cast()));	 // 原点に戻している

	/// 回転するものを指定している
	boss._crab._legs[l]._sPoint	   = VTransform(boss._crab._legs[l]._sPoint.V_Cast(), mat);
	boss._crab._legs[l]._mPoint	   = VTransform(boss._crab._legs[l]._mPoint.V_Cast(), mat);
	boss._crab._legs[l]._ePoint	   = VTransform(boss._crab._legs[l]._ePoint.V_Cast(), mat);
	boss._crab._legs[l]._ctlPoint   = VTransform(boss._crab._legs[l]._ctlPoint.V_Cast(), mat);
}

void Crab::MoveLeg()
{
	/// 蟹本体の左端と右端の頂点から方向ベクトルを取得している
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
			/// 右足の動き
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
			/// 左足の動き
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
									(*leg)._vel.y * vec.y + (*leg)._liftVel);	
	}
}

void Crab::MoveJoint()
{
	Vector2 pLength;
	float sint, cost, rad;

	/// 足関節の移動
	auto leg = boss._crab._legs.begin();
	for (; leg != boss._crab._legs.end(); ++leg)
	{
		pLength = (*leg)._ctlPoint - (*leg)._sPoint;
		cost = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
		rad = acos(cost);
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
		rad = acos(cost);
		sint = sin(sint);

		if (!(std::isnan(cost) && std::isnan(sint)))
		{
			if (pLength.Magnitude() < aLength * 2)
			{
				auto cnt = arm - boss._crab._arms.begin();	/// 足の番地指定用

				/// Y軸の方向ﾍﾞｸﾄﾙを計算している
				auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
				auto cross2f = Vector2(cross.x, cross.y);
				cross2f.Normalize();

				auto cosD = pLength.Normalized() * (aLength * cost);		/// X方向の成分
				auto sinD = cross2f * (aLength * sint);					/// Y方向の成分

				/// 左関節が逆に向かないための処理(腕の修正はとりあえず後で)
				sinD = (!(cnt / (boss._crab._arms.size() / 2)) ? cosD : -cosD);

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

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// 足の描画
	Vector2 p1, p2, p3, p4;
	for (auto leg : boss._crab._legs)
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

		/// 制御点の描画
		DxLib::DrawCircle(leg._ctlPoint.x, leg._ctlPoint.y, 4, 0xffff00, true);
	}

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

		/// 制御点の描画
		DxLib::DrawCircle(arm._ctlPoint.x, arm._ctlPoint.y, 4, 0xffff00, true);
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
	/// 回転するときの中心点の描画
	DrawCircle(center.x - camera.x, center.y - camera.y, 10, 0xff0000, true);
}

void Crab::Update()
{
	(this->*_updater)();

	Rotation();			/// 蟹の中心点を回転している
	for (int i = 0; i < boss._crab._legs.size(); ++i)
	{
		/// 蟹の頂点、足の回転
			
		Rotation(i);
	}
	CalVert();
	MoveLeg();
	MoveJoint();

	boss._crab._prePos = boss._crab._pos;
}

BossInfo Crab::GetInfo()
{
	return boss;
}
