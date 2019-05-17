#include "Crab.h"
#include "../Camera.h"

const int length   = 80;
const Size eSize = Size(length, 20);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	for (int i = 0; i < 4; ++i)
	{
		legs.push_back(Leg());
		ctlPoints.push_back(Vector2());
		legs[i].sPoint = Vector2(200, 200 + (i * 60));
		legs[i].mPoint = legs[i].sPoint + Vector2(length, 0);
		legs[i].ePoint = legs[i].mPoint + Vector2(length, 0);

		ctlPoints[i] = legs[i].ePoint + Vector2(length, 0);
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

void Crab::CalVert(const int& i)
{
	Vector2 size;

	/// �n�_���璆�ԓ_�܂ł̋�`�̐ݒ�
	auto theta = atan2f(legs[i].mPoint.y - legs[i].sPoint.y,
						legs[i].mPoint.x - legs[i].sPoint.x);
	auto cosD = cos(theta + DX_PI / 2);
	auto sinD = sin(theta + DX_PI / 2);

  	size.x = cosD * (eSize.height / 2);
	size.y = sinD * (eSize.height / 2);
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

	size.x = cosD * (eSize.height / 2);
	size.y = sinD * (eSize.height / 2);

	for (int p = 0; p < legs[i].sqrVert[1].size(); ++p)
	{
		auto sizePos = (p < 2 ? -size : size);
		legs[i].sqrVert[1][p] = (p == 0 || p == 3 ? legs[i].mPoint : legs[i].ePoint) + sizePos;
	}
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
	// �]���藝���g���Ă̊֐߈ړ�
	for (int i = 0; i < legs.size(); ++i)
	{
		DrawLine(legs[i].mPoint.x, legs[i].mPoint.y, legs[i].ePoint.x, legs[i].ePoint.y, 0x00ff00);		/// �I�_���璆�ԓ_
		DrawLine(legs[i].sPoint.x, legs[i].sPoint.y, legs[i].mPoint.x, legs[i].mPoint.y, 0x00ff00);			/// ���ԓ_����n�_

		/*DrawCircle(joints[i].sPoint.x, joints[i].sPoint.y, 5, 0xff0000, true);
		DrawCircle(joints[i].mPoint.x, joints[i].mPoint.y, 5, 0xff0000, true);
		DrawCircle(joints[i].ePoint.x, joints[i].ePoint.y, 5, 0xff0000, true);*/
	}
	

#ifdef _DEBUG
	DebugDraw();
#endif
}

void Crab::DebugDraw()
{
	/// ���̕`��
	Vector2 p1, p2, p3, p4;
	for (int i = 0; i < legs.size(); ++i)
	{
		/// �n�_���璆�Ԃ܂ł̋�`
		p1 = legs[i].sqrVert[0][0];
		p2 = legs[i].sqrVert[0][1];
		p3 = legs[i].sqrVert[0][2];
		p4 = legs[i].sqrVert[0][3];
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xff0000, true);

		/// ���Ԃ���I�_�܂ł̋�`
		p1 = legs[i].sqrVert[1][0];
		p2 = legs[i].sqrVert[1][1];
		p3 = legs[i].sqrVert[1][2];
		p4 = legs[i].sqrVert[1][3];
		DxLib::DrawQuadrangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0xff0000, true);
	}
}

void Crab::Update()
{
	(this->*_updater)();
	for (int i = 0; i < legs.size(); ++i)
	{
		CalVert(i);
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
