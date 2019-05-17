#include "Crab.h"
#include "../Camera.h"

const int distance = 60;
const int length   = 80;
const Size eSize = Size(40, 40);

Crab::Crab(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	for (int i = 0; i < 1; ++i)
	{
		joints.push_back(Joint());
		ctlPoints.push_back(Vector2());
		joints[i].sPoint = Vector2(200, 200 + (i * 30));
		joints[i].mPoint = joints[i].sPoint + Vector2(distance, 0);
		joints[i].ePoint = joints[i].mPoint + Vector2(distance, 0);

		ctlPoints[i] = joints[i].ePoint + Vector2(50, 50);
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

void Crab::CalRect()
{
	float right, left, top, bottom;

}

void Crab::LegMove(const Vector2& pos, const int& i)
{
	///�]���藝
	auto pLength = pos - joints[i].sPoint;

	joints[i].cos = (pow(length, 2.0) + pow(pLength.Magnitude(), 2.0) - pow(length, 2.0)) / (2 * length * pLength.Magnitude());
	auto rad = acos(joints[i].cos);			/// cos�̊p�x
	joints[i].sin = sin(rad);				/// sin�̒l

	/// �l���ُ�l�ɂȂ������̏�����
	if (!(std::isnan(joints[i].cos) && std::isnan(joints[i].sin)))
	{
		if (pLength.Magnitude() < length * 2)
		{
			/// ��������苗�����Z���Ȃ������̏���

			/// �O�ς��g���āAY�����̒P���޸�ق����߂Ă���
			auto cross = Cross(Vector3(pLength.x, pLength.y, 0), Vector3(0, 0, 1));
			auto cross2f = Vector2(cross.x, cross.y);
			cross2f.Normalize();

			auto cosD = pLength.Normalized() * (length * joints[i].cos);		/// X�����̐���
			auto sinD = cross2f * (length * joints[i].sin);					/// Y�����̐���

			joints[i].mPoint = joints[i].sPoint + cosD + sinD;
			joints[i].ePoint = pos;
		}
		else
		{
			/// ��������苗�����Ȃ����Ȃ����ꍇ
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
	// �]���藝���g���Ă̊֐߈ړ�
	for (int i = 0; i < joints.size(); ++i)
	{
		DrawLine(joints[i].mPoint.x, joints[i].mPoint.y, joints[i].ePoint.x, joints[i].ePoint.y, 0x00ff00);		/// �I�_���璆�ԓ_
		DrawLine(joints[i].sPoint.x, joints[i].sPoint.y, joints[i].mPoint.x, joints[i].mPoint.y, 0x00ff00);			/// ���ԓ_����n�_

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
	/// ���̐���_�`��
	for (int i = 0; i < joints.size(); ++i)
	{
		DrawCircle(ctlPoints[i].x, ctlPoints[i].y, 4, 0xffff00, true);
	}
}

void Crab::Update()
{
	(this->*_updater)();
	for (int i = 0; i < joints.size(); ++i)
	{
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
