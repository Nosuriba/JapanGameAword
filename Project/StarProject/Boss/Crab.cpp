#include "Crab.h"
#include "../Camera.h"

const VECTOR rotVec = { 0,0,1.f };
const float rotVel = DX_PI_F / 540.f;
const int length = 150;
const Size eSize = Size(500, 400);
const Size lSize = Size(length, 50);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	/// �I�{�̂̐ݒ�
	boss._crab._pos = Vector2(800, 500);
	boss._crab._size = eSize;

	/// ���̐ݒ�
	for (int i = 0; i < 6; ++i)
	{
		/// ���ނ̊m��
		legs.push_back(LegInfo());
		ctlPoints.push_back(Vector2());

		if (!(i / 3))
		{
			auto pos = Vector2(boss._crab._pos.x + eSize.width / 2,
							   boss._crab._pos.y - (lSize.height / 2));
			legs[i].sPoint = pos + Vector2(0,(i * (lSize.height * 2)));
			legs[i].mPoint = legs[i].sPoint + Vector2(length, 0);
			legs[i].ePoint = legs[i].mPoint + Vector2(length, 0);

			ctlPoints[i] = legs[i].ePoint - Vector2(length / 2, 0);
		}
		else
		{
			auto pos = Vector2(boss._crab._pos.x - eSize.width / 2,
							   boss._crab._pos.y - (lSize.height / 2));
			legs[i].sPoint = pos + Vector2(0,((i % 3) * (lSize.height * 2)));
			legs[i].mPoint = legs[i].sPoint - Vector2(length, 0);
			legs[i].ePoint = legs[i].mPoint - Vector2(length, 0);

			ctlPoints[i] = legs[i].ePoint + Vector2(length / 2, 0);
		}
	}

	/// �I�{�̂̒��_��ݒ肵�Ă���
	Vector2 p1, p2, p3, p4;
	p1 = Vector2(boss._crab._pos.x - eSize.width / 2, boss._crab._pos.y - eSize.height / 2);
	p2 = Vector2(boss._crab._pos.x + eSize.width / 2, boss._crab._pos.y - eSize.height / 2);
	p3 = Vector2(boss._crab._pos.x + eSize.width / 2, boss._crab._pos.y + eSize.height / 2);
	p4 = Vector2(boss._crab._pos.x - eSize.width / 2, boss._crab._pos.y + eSize.height / 2);
	boss._crab._vert[0] = p1;
	boss._crab._vert[1] = p2;
	boss._crab._vert[2] = p3;
	boss._crab._vert[3] = p4;
	
	Neutral();
}

Crab::~Crab()
{
}

void Crab::Neutral()
{
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

void Crab::CalVert(const int& i)
{
	Vector2 size;

	/// �n�_���璆�ԓ_�܂ł̋�`�̐ݒ�
	auto theta = atan2f(legs[i].mPoint.y - legs[i].sPoint.y,
						legs[i].mPoint.x - legs[i].sPoint.x);
	auto cosD = cos(theta + DX_PI / 2);
	auto sinD = sin(theta + DX_PI / 2);

   	size.x = cosD * (lSize.height / 2);
	size.y = sinD * (lSize.height / 2);
	for (int p = 0; p < legs[i].sqrVert[0].size(); ++p)
	{
		auto sizePos = (p < 2 ? -size : size);
		legs[i].sqrVert[0][p] = (p == 0 || p == 3 ? legs[i].sPoint : legs[i].mPoint) + sizePos;
	}

	/// ���ԓ_����I�_�܂ł̋�`�̐ݒ�
	theta = atan2f(legs[i].ePoint.y - legs[i].mPoint.y,
				   legs[i].ePoint.x - legs[i].mPoint.x);

	cosD = cos(theta + DX_PI / 2);
	sinD = sin(theta + DX_PI / 2);

	size.x = cosD * (lSize.height / 2);
	size.y = sinD * (lSize.height / 2);

	for (int p = 0; p < legs[i].sqrVert[1].size(); ++p)
	{
		auto sizePos = (p < 2 ? -size : size);
		legs[i].sqrVert[1][p] = (p == 0 || p == 3 ? legs[i].mPoint : legs[i].ePoint) + sizePos;
	}
}

void Crab::Rotation()
{
	auto mat = MGetRotAxis(rotVec, rotVel); 	 // Z���̉�]�s�������Ă���

	/// �I�{�̂̉�]
	boss._crab._pos = VTransform(boss._crab._pos.V_Cast(), mat);

	/// �I�̒��_�̉�]
	for (int i = 0; i < boss._crab._vert.size(); ++i)
	{
		boss._crab._vert[i] = VTransform(boss._crab._vert[i].V_Cast(), mat);
	}
}

void Crab::Rotation(const int & i)
{
	auto mat = MGetRotAxis(rotVec, rotVel);		 // Z���̉�]�s�������Ă���

	/// ��]������̂��w�肵�Ă���
	legs[i].sPoint = VTransform(legs[i].sPoint.V_Cast(), mat);
	legs[i].mPoint = VTransform(legs[i].mPoint.V_Cast(), mat);
	legs[i].ePoint = VTransform(legs[i].ePoint.V_Cast(), mat);
	ctlPoints[i]   = VTransform(ctlPoints[i].V_Cast(), mat);
}

void Crab::LegMove(const Vector2& pos, const int& i)
{
	///�]���藝
	auto pLength = pos - legs[i].sPoint;

	legs[i].cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
	auto rad = acos(legs[i].cos);			/// cos�̊p�x
	legs[i].sin = sin(rad);				/// sin�̒l

	/// �l���ُ�l�ɂȂ������̏�����
	if (!(std::isnan(legs[i].cos) && std::isnan(legs[i].sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// ��������苗�����Z���Ȃ������̏���

			/// �O�ς��g���āAY�����̒P���޸�ق����߂Ă���
			auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
			auto cross2f = Vector2(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * legs[i].cos);		/// X�����̐���
			auto sinD = cross2f * (length * legs[i].sin);					/// Y�����̐���

			/// ���̊֐߂��t�Ɍ����Ȃ����߂̏���
			sinD = (legs[i].sPoint.x > pos.x ? -sinD : sinD);

			legs[i].mPoint = legs[i].sPoint + cosD + sinD;
			legs[i].ePoint = pos;
		}
		else
		{
			/// ��������苗�����Ȃ����Ȃ����ꍇ
			legs[i].mPoint = legs[i].sPoint + (pLength.Normalized() * length);
			legs[i].ePoint = legs[i].mPoint + (pLength.Normalized() * length);
		}
	}
	else
	{
		legs[i].cos = 0.f;
		legs[i].sin = 0.f;
	}
}

void Crab::Draw()
{
	auto camera = _camera->CameraCorrection();
#ifdef _DEBUG
	DebugDraw(camera);
#endif
}

void Crab::DebugDraw(const Vector2& camera)
{
	/// ���̕`��
	Vector2 p1, p2, p3, p4;
	for (int i = 0; i < legs.size(); ++i)
	{
		/// �n�_���璆�Ԃ܂ł̋�`
		p1 = legs[i].sqrVert[0][0] - camera;
		p2 = legs[i].sqrVert[0][1] - camera;
		p3 = legs[i].sqrVert[0][2] - camera;
		p4 = legs[i].sqrVert[0][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		/// ���Ԃ���I�_�܂ł̋�`
		p1 = legs[i].sqrVert[1][0] - camera;
		p2 = legs[i].sqrVert[1][1] - camera;
		p3 = legs[i].sqrVert[1][2] - camera;
		p4 = legs[i].sqrVert[1][3] - camera;
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xcc3300, true);

		DxLib::DrawCircle(ctlPoints[i].x, ctlPoints[i].y, 4, 0xffff00, true);
	}

	DxLib::DrawQuadrangle(boss._crab._vert[0].x - camera.x, boss._crab._vert[0].y - camera.y,
						  boss._crab._vert[1].x - camera.x, boss._crab._vert[1].y - camera.y,
						  boss._crab._vert[2].x - camera.x, boss._crab._vert[2].y - camera.y,
						  boss._crab._vert[3].x - camera.x, boss._crab._vert[3].y - camera.y,
						  0xcc3300, true);
}

void Crab::Update()
{
	
	(this->*_updater)();
	for (int i = 0; i < legs.size(); ++i)
	{
		CalVert(i);
		LegMove(ctlPoints[i], i);
		ctlPoints[i] += _vel;
		Rotation(i);
	}
	
	Rotation();
	boss._crab._prePos = boss._crab._pos;
	boss._crab._pos += _vel;
}

BossInfo Crab::GetInfo()
{
	return boss;
}
