#include "Enemy.h"
#include "../Input.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(const Input & p)
{
}

void Enemy::Draw()
{
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));			/// ���f�����W�̐ݒ�
	DxLib::MV1SetRotationXYZ(handle, VGet(angle.x, angle.y, angle.z));  /// ���f���̉�]
	DxLib::MV1DrawModel(handle);
}
