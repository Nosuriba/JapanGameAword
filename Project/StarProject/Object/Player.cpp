#include "Player.h"
#include "../Input.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(const Input & p)
{
	Move(p);
	pos   += vel;
	angle += Vector3(0.f, 0.01f, 0.f);			/// �n�������]�����Ă邾��

	DxLib::GetMousePoint(&mouseX, &mouseY);
	mouseDebug = p.IsMousePressed(MOUSE_INPUT_LEFT);
}

void Player::Draw()
{
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));			/// ���f�����W�̐ݒ�
	DxLib::MV1SetRotationXYZ(handle, VGet(angle.x, angle.y, angle.z));  /// ���f���̉�]
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Player::DebugDraw()
{
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0,		0xffffff, "�n���̍��W(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "�n���̍��W(Y)  %d", (int)(pos.y));
	if (mouseDebug)
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "�N���b�N��", 0x0000ff);
	}
	else
	{
		DxLib::DrawExtendString(300, 0, 2.0, 2.0, "�N���b�N���ĂȂ���", 0x0000ff);
	}
	
}

void Player::Move(const Input & p)
{
	/// �n���𓊂��鏈���Œǉ������Ƃ���B
	auto velControl = [](const Vector3& vel)
	{
		auto v = vel;
		v.x = (abs(v.x) < 0.1f ? v.x = 0 : v.x = v.x * 0.85f);
		v.y = (abs(v.y) < 0.1f ? v.y = 0 : v.y = v.y * 0.85f);

		return v;
	};

	if (p.IsPressed(PAD_INPUT_LEFT))
	{
		vel.x -= 0.1f;
	}
	else if (p.IsPressed(PAD_INPUT_RIGHT))
	{
		vel.x += 0.1f;
	}
	else if (p.IsPressed(PAD_INPUT_UP))
	{
		vel.y += 0.1f;
	}
	else if (p.IsPressed(PAD_INPUT_DOWN))
	{
		vel.y -= 0.1f;
	}
	else
	{
		vel = velControl(vel);
	}
}