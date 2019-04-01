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
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));			/// モデル座標の設定
	DxLib::MV1SetRotationXYZ(handle, VGet(angle.x, angle.y, angle.z));  /// モデルの回転
	DxLib::MV1DrawModel(handle);
}
