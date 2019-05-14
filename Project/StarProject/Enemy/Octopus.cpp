#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"

#define LEG(x) _oct.legs[x]

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Enemy(camera), _camera(camera)
{
	_oct.center = Vector2(700, 300);
	_oct.root = _oct.center + Vector2(0, 30);
	_oct.r = 100;
	_oct.legs.resize(8);
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		LEG(i).tip.x = _oct.root.x + cos(radian * i + DX_PI_F / 180.0f * -90.0f) * _oct.r;
		LEG(i).tip.y = _oct.root.y + sin(radian * i + DX_PI_F / 180.0f * -90.0f) * _oct.r;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint.push_back(_oct.root + LEG(i).tip.Normalized()*((_oct.r / LEG(i).T)*j));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
	}
}

void Octopus::Die()
{
}

void Octopus::DieUpdate()
{
}

void Octopus::FootMove()
{
	for (int i = 0; i < _oct.legs.size(); ++i) {
		for (int j = LEG(i).T - 1; j >= 0; --j) {
			auto p_vec = LEG(i).tip - LEG(i).joint[j];		//目標→関節
			auto t_vec = LEG(i).tip - LEG(i).joint[j];		//先端→関節
			auto mat = MGetTranslate((-LEG(i).joint[j]).V_Cast());			//原点まで移動
			mat = MMult(mat, MGetRotVec2(t_vec.V_Cast(), p_vec.V_Cast()));	//回転
			mat = MMult(mat, MGetTranslate(LEG(i).joint[j].V_Cast()));		//元の位置に移動
		}
	}
}

void Octopus::Draw()
{
	auto c = _camera->CameraCorrection();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j;
		for (j = 1; j < LEG(i).T; ++j) {
			DrawLine(LEG(i).joint[j - 1].x - c.x, LEG(i).joint[j - 1].y - c.y, LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 0xff0000);
		}
		DrawLine(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, LEG(i).tip.x - c.x, LEG(i).tip.y - c.y, 0xff0000);
	}
	DrawCircle(_oct.center.x - c.x, _oct.center.y - c.y, 30, 0xff0000, true);
}

void Octopus::Update()
{
}

EnemyInfo Octopus::GetInfo()
{
	return EnemyInfo();
}

shot_vector Octopus::GetShotInfo()
{
	return shot_vector();
}

void Octopus::CalEscapeDir(const Vector2 & vec)
{
}

void Octopus::ChangeShotColor(const int & num)
{
}

void Octopus::CalTrackVel(const Vector2 & pos, bool col)
{
}

Octopus::~Octopus()
{
}