#include "Crab.h"
#include "../Camera.h"

const int length   = 80;
const Size eSize = Size(length, 30);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	for (int i = 0; i < 1; ++i)
	{
		joints.push_back(Joint());
		ctlPoints.push_back(Vector2());
		joints[i].sPoint = Vector2(200, 200 + (i * 30));
		joints[i].mPoint = joints[i].sPoint + Vector2(length, 0);
		joints[i].ePoint = joints[i].mPoint + Vector2(length, 0);

		ctlPoints[i] = joints[i].ePoint + Vector2(length, 0);
	}
	
	Neutral();
}

Crab::~Crab()
{
}

void Crab::Neutral()
{
	moveCnt = 60;
	_vel.x = 2.0f;
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
	_vel = Vector2(0, 0);
	if (CheckHitKey(KEY_INPUT_A))
		_vel.x = -2.0f;
	if (CheckHitKey(KEY_INPUT_D))
		_vel.x = 2.0f;
	if (CheckHitKey(KEY_INPUT_W))
		_vel.y = -2.0f;
	if (CheckHitKey(KEY_INPUT_S))
		_vel.y = 2.0f;
}

void Crab::ShotUpdate()
{
}

void Crab::DieUpdate()
{
}

void Crab::CalVert()
{
	Vector2 pos;
	Size size;
	float right, left, top, bottom, lengthX, lengthY;

	auto vec = joints[0].mPoint - joints[0].sPoint;
	vec.Normalize();
	/// とりあえず、仮で点の設定をしている
	for (int i = 0; i < 4; ++i)
	{
		if (i == 0 || i == 3)
		{
			lengthX = (i / 4 ? (eSize.height / 2) * vec.x :
							  -(eSize.height / 2) * vec.x);
			lengthY = (!(i / 4) ? (eSize.height / 2) * vec.y :
								 -(eSize.height / 2) * vec.y);

			joints[0].sqrVert[0][i] = joints[0].sPoint + Vector2(lengthX, lengthY);
		}
		else
		{
			lengthX = (i % 2 ? (eSize.height / 2) * vec.x :
							  -(eSize.height / 2) * vec.x);
			lengthY = (!(i % 2) ? (eSize.height / 2) * vec.y :
								 -(eSize.height / 2) * vec.y);

			joints[0].sqrVert[0][i] = joints[0].mPoint + Vector2(lengthX, lengthY);
		}
	}
}

void Crab::LegMove(const Vector2& pos, const int& i)
{
	///余弦定理
	auto pLength = pos - joints[i].sPoint;

	joints[i].cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
	auto rad = acos(joints[i].cos);			/// cosの角度
	joints[i].sin = sin(rad);				/// sinの値

	/// 値が異常値になった時の初期化
	if (!(std::isnan(joints[i].cos) && std::isnan(joints[i].sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// 長さが一定距離より短くなった時の処理

			/// 外積を使って、Y方向の単位ﾍﾞｸﾄﾙを求めている
			auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
			auto cross2f = Vector2(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * joints[i].cos);		/// X方向の成分
			auto sinD = cross2f * (length * joints[i].sin);					/// Y方向の成分

			joints[i].mPoint = joints[i].sPoint + cosD + sinD;
			joints[i].ePoint = pos;
		}
		else
		{
			/// 長さが一定距離よりながくなった場合
			joints[i].mPoint = joints[i].sPoint + (pLength.Normalized() * length);
			joints[i].ePoint = joints[i].mPoint + (pLength.Normalized() * length);
		}
	}
	else
	{
		joints[i].cos = 0.f;
		joints[i].sin = 0.f;
	}
}

void Crab::Draw()
{
	// 余弦定理を使っての関節移動
	for (int i = 0; i < joints.size(); ++i)
	{
		DrawLine(joints[i].mPoint.x, joints[i].mPoint.y, joints[i].ePoint.x, joints[i].ePoint.y, 0x00ff00);		/// 終点から中間点
		DrawLine(joints[i].sPoint.x, joints[i].sPoint.y, joints[i].mPoint.x, joints[i].mPoint.y, 0x00ff00);			/// 中間点から始点

		DrawCircle(joints[i].sPoint.x, joints[i].sPoint.y, 5, 0xff0000, true);
		DrawCircle(joints[i].mPoint.x, joints[i].mPoint.y, 5, 0xff0000, true);
		DrawCircle(joints[i].ePoint.x, joints[i].ePoint.y, 5, 0xff0000, true);
	}
	

#ifdef _DEBUG
	DebugDraw();
#endif
}

void Crab::DebugDraw()
{
	/// 足の制御点描画
	for (int i = 0; i < joints.size(); ++i)
	{
		DrawCircle(ctlPoints[i].x, ctlPoints[i].y, 4, 0xffff00, true);
	}

	Vector2 p1, p2, p3, p4;
	p1 = joints[0].sqrVert[0][0];
	p2 = joints[0].sqrVert[0][1];
	p3 = joints[0].sqrVert[0][2];
	p4 = joints[0].sqrVert[0][3];

	DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xff0000, true);

	for (int i = 0; i < joints[0].sqrVert[0].size(); ++i)
	{
		DxLib::DrawCircle(joints[0].sqrVert[0][i].x, joints[0].sqrVert[0][i].y, 5, 0xff00ff, true);
	}

	DxLib::DrawCircle(dCenter.x, dCenter.y, 5, 0x0000ff, true);
}

void Crab::Update()
{
	(this->*_updater)();
	for (int i = 0; i < joints.size(); ++i)
	{
		CalVert();
		LegMove(ctlPoints[i], i);
		ctlPoints[i] += _vel;
	}
	
	boss._crab._prePos = boss._crab._pos;
	boss._crab._pos += _vel;
}

BossInfo Crab::GetInfo()
{
	return boss;
}
