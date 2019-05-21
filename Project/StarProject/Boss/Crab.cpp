#include "Crab.h"
#include "../Game.h"
#include "../Camera.h"

const Vector2 center = Vector2(Game::GetInstance().GetScreenSize().x / 2,
							   Game::GetInstance().GetScreenSize().y / 2);
const Vector2 lVel = Vector2(5.f, 5.f);
const VECTOR rotVec = { 0,0,1.f };
const float rotVel = DX_PI_F / 540.f;
const int length = 150;
const Size eSize = Size(450, 300);
const Size lSize = Size(length, 30);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	/// 蟹本体の設定
	boss._crab._pos = Vector2(center.x, 800);
	boss._crab._size = eSize;
	/// 足の数
	boss._crab.legs.resize(8);
	ctlPoints.resize(8);
	/// 足の設定
	for (int i = 0; i < 8; ++i)
	{
		/// ｻｲｽﾞの確保
		

		if (!(i / (boss._crab.legs.size() / 2)))
		{
			/// 右足の設定
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 2,
							   boss._crab._pos.y - eSize.height / 4);
			boss._crab.legs[i].sPoint = pos + Vector2(0,(i * (lSize.height * 2)));
			boss._crab.legs[i].mPoint = boss._crab.legs[i].sPoint + Vector2(length, 0);
			boss._crab.legs[i].ePoint = boss._crab.legs[i].mPoint + Vector2(length, 0);

			ctlPoints[i] = boss._crab.legs[i].ePoint - Vector2(length / 2, 0);
			_legVel.push_back(Vector2(-lVel.x, lVel.y));
		}
		else
		{
			/// 左足の設定
			auto pos = Vector2(boss._crab._pos.x - eSize.width / 2,
							   boss._crab._pos.y - eSize.height / 4);
			boss._crab.legs[i].sPoint = pos + Vector2(0,((i % (boss._crab.legs.size() / 2)) * (lSize.height * 2)));
			boss._crab.legs[i].mPoint = boss._crab.legs[i].sPoint - Vector2(length, 0);
			boss._crab.legs[i].ePoint = boss._crab.legs[i].mPoint - Vector2(length, 0);

			ctlPoints[i] = boss._crab.legs[i].ePoint + Vector2(length / 2, 0);
			_legVel.push_back(Vector2(lVel.x, -lVel.y));
		}
	}

	/// 蟹本体の頂点を設定
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		auto posX = (i != 0 && i != 3 ? eSize.width / 2 : -eSize.width / 2);
		auto posY = (!(i / 2) ? eSize.height / 2 : -eSize.height / 2);

		boss._crab._vert[i] = boss._crab._pos + Vector2(posX, posY);
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

void Crab::CalVert(const int& l)
{
	Vector2 size;

	/// 始点から中間点までの矩形の設定
	auto theta = atan2f(boss._crab.legs[l].mPoint.y - boss._crab.legs[l].sPoint.y,
						boss._crab.legs[l].mPoint.x - boss._crab.legs[l].sPoint.x);
	auto cosD = cos(theta + DX_PI / 2);
	auto sinD = sin(theta + DX_PI / 2);

   	size.x = cosD * (lSize.height / 2);
	size.y = sinD * (lSize.height / 2);
	for (int p = 0; p < boss._crab.legs[l].legVert[0].size(); ++p)
	{
		auto sizePos = (p < 2 ? -size : size);
		boss._crab.legs[l].legVert[0][p] = (p == 0 || p == 3 ? boss._crab.legs[l].sPoint : boss._crab.legs[l].mPoint) + sizePos;
	}

	/// 中間点から終点までの矩形の設定
	theta = atan2f(boss._crab.legs[l].ePoint.y - boss._crab.legs[l].mPoint.y,
				   boss._crab.legs[l].ePoint.x - boss._crab.legs[l].mPoint.x);

	cosD = cos(theta + DX_PI / 2);
	sinD = sin(theta + DX_PI / 2);

	size.x = cosD * (lSize.height / 2);
	size.y = sinD * (lSize.height / 2);

	for (int p = 0; p < boss._crab.legs[l].legVert[1].size(); ++p)
	{
		auto sizePos = (p < 2 ? -size : size);
		boss._crab.legs[l].legVert[1][p] = (p == 0 || p == 3 ? boss._crab.legs[l].mPoint 
															 : boss._crab.legs[l].ePoint) + sizePos;
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
	boss._crab.legs[l].sPoint = VTransform(boss._crab.legs[l].sPoint.V_Cast(), mat);
	boss._crab.legs[l].mPoint = VTransform(boss._crab.legs[l].mPoint.V_Cast(), mat);
	boss._crab.legs[l].ePoint = VTransform(boss._crab.legs[l].ePoint.V_Cast(), mat);
	ctlPoints[l]   = VTransform(ctlPoints[l].V_Cast(), mat);
}

void Crab::MovePoint(const int & p)
{
	auto d = Vector2(abs(boss._crab.legs[p].ePoint.x - boss._crab.legs[p].sPoint.x),
					 abs(boss._crab.legs[p].ePoint.y - boss._crab.legs[p].sPoint.y));

	auto vec = boss._crab._vert[1] - boss._crab._vert[0];
	vec.Normalize();

	
	if (!(p / (boss._crab.legs.size() / 2)))
	{
		/// 右足の動き

		/// 横方向の往復
		if (_legVel[p].x > 0)
		{
			_legVel[p].x = (d.x > length + (length / 2) ? -_legVel[p].x : _legVel[p].x);
		}
		else
		{
			_legVel[p].x = (d.x < length - (length / 2) ? -_legVel[p].x : _legVel[p].x);
		}

		/// 縦方向の往復
		if (_legVel[p].y > 0)
		{
			_legVel[p].y = (d.y > length + (length / 2) ? -_legVel[p].y : _legVel[p].y);
		}
		else
		{
			_legVel[p].y = (d.y < length - (length / 2) ? -_legVel[p].y : _legVel[p].y);
		}
	}
	else
	{
		/// 左足の動き
		
		/// 横方向の往復
		if (_legVel[p].x > 0)
		{
			_legVel[p].x = (d.x < length - (length / 2) ? -_legVel[p].x : _legVel[p].x);
		}
		else
		{
			_legVel[p].x = (d.x > length + (length / 2) ? -_legVel[p].x : _legVel[p].x);
		}
		/// 縦方向の往復
		if (_legVel[p].y > 0)
		{
			_legVel[p].y = (d.y < length - (length / 2) ? -_legVel[p].y : _legVel[p].y);
		}
		else
		{
			_legVel[p].y = (d.y > length + (length / 2) ? -_legVel[p].y : _legVel[p].y);
		}
	}
	/// 制御点の移動
	ctlPoints[p] += Vector2(_legVel[p].x * vec.x ,_legVel[p].y * vec.y);	/// 回転あり
}

void Crab::MoveLeg(const Vector2& pos, const int& l)
{
	/// 余弦定理
	auto pLength = pos - boss._crab.legs[l].sPoint;
	boss._crab.legs[l].cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());

	auto rad = acos(boss._crab.legs[l].cos);		/// cosの角度
	boss._crab.legs[l].sin = sin(rad);				/// sinの値

	/// 値が異常値になった時の初期化
	if (!(std::isnan(boss._crab.legs[l].cos) && std::isnan(boss._crab.legs[l].sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// 長さが一定距離より短くなった時の処理

			/// 外積を使って、Y方向の単位ﾍﾞｸﾄﾙを求めている
			auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
			auto cross2f = Vector2(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * boss._crab.legs[l].cos);		/// X方向の成分
			auto sinD = cross2f * (length * boss._crab.legs[l].sin);					/// Y方向の成分

			/// 左関節が逆に向かないための処理
			sinD = (!(l / (boss._crab.legs.size() / 2)) ? sinD : -sinD);

			boss._crab.legs[l].mPoint = boss._crab.legs[l].sPoint + cosD + sinD;
			boss._crab.legs[l].ePoint = pos;
		}
		else
		{
			/// 長さが一定距離よりながくなった場合
			boss._crab.legs[l].mPoint = boss._crab.legs[l].sPoint + (pLength.Normalized() * length);
			boss._crab.legs[l].ePoint = boss._crab.legs[l].mPoint + (pLength.Normalized() * length);
		}
	}
	else
	{
		boss._crab.legs[l].cos = 0.f;
		boss._crab.legs[l].sin = 0.f;
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();

	/// 足の描画
	Vector2 p1, p2, p3, p4;
	for (int i = 0; i < boss._crab.legs.size(); ++i)
	{
		/// 始点から中間までの矩形
		p1 = boss._crab.legs[i].legVert[0][0] - camera;
		p2 = boss._crab.legs[i].legVert[0][1] - camera;
		p3 = boss._crab.legs[i].legVert[0][2] - camera;
		p4 = boss._crab.legs[i].legVert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// 中間から先端までの矩形
		p1 = boss._crab.legs[i].legVert[1][0] - camera;
		p2 = boss._crab.legs[i].legVert[1][1] - camera;
		p3 = boss._crab.legs[i].legVert[1][2] - camera;
		p4 = boss._crab.legs[i].legVert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// 制御点の描画
		DxLib::DrawCircle(ctlPoints[i].x, ctlPoints[i].y, 4, 0xffff00, true);
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
	for (int i = 0; i < boss._crab.legs.size(); ++i)
	{
		CalVert(i);
		Rotation(i);
		MovePoint(i);
		MoveLeg(ctlPoints[i], i);
	}
	
	Rotation();
	boss._crab._prePos = boss._crab._pos;
	// boss._crab._pos += _vel;
}

BossInfo Crab::GetInfo()
{
	return boss;
}
