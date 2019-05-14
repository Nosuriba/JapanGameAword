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
		auto c = cos(radian * i);
		auto s = sin(radian * i);
		LEG(i).tip.x = _oct.root.x + c * _oct.r;
		LEG(i).tip.y = _oct.root.y + s * _oct.r;
		for (int j = 0; j < LEG(i).T; ++j) {
			auto vec = _oct.root + LEG(i).tip;
			LEG(i).joint.push_back(_oct.root + vec.Normalized()*((_oct.r / LEG(i).T)*j));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
	}
	angle = 0;
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

		auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
		auto rad = radian * i;
		auto ang = abs(angle % 60 - 30);
		rad = rad + DX_PI_F / 180 * ang;
		auto c = _oct.root.x + cos(rad)*_oct.r;
		auto s = _oct.root.y + sin(rad)*_oct.r;
		auto p_pos = Vector2(c, s);
		for (int it = 0; it < 6; ++it) {
			for (int j = LEG(i).T; j > 0; --j) {
				auto p_vec = p_pos - LEG(i).joint[j - 1];		//目標→関節
				auto t_vec = LEG(i).tip - LEG(i).joint[j - 1];		//先端→関節
				auto mat = MGetTranslate((-LEG(i).joint[j - 1]).V_Cast());			//原点まで移動
				mat = MMult(mat, MGetRotVec2(t_vec.V_Cast(), p_vec.V_Cast()));	//回転
				mat = MMult(mat, MGetTranslate(LEG(i).joint[j - 1].V_Cast()));		//元の位置に移動
				for (int itr = j; itr <= LEG(i).T; ++itr) {
					LEG(i).joint[itr] = VTransform(LEG(i).joint[itr].V_Cast(), mat);
				}
				LEG(i).tip = LEG(i).joint[LEG(i).T];
			}
		}
	}
}

void Octopus::Draw()
{
	auto c = _camera->CameraCorrection();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		DrawCircle(LEG(i).joint[j].x, LEG(i).joint[j].y, 5, 0x00ff00, true);
		for (j = 1; j <= LEG(i).T; ++j) {
			DrawLine(LEG(i).joint[j - 1].x, LEG(i).joint[j - 1].y, LEG(i).joint[j].x, LEG(i).joint[j].y, 0xff0000);
			DrawCircle(LEG(i).joint[j].x, LEG(i).joint[j].y, 5, 0x00ff00, true);
		}
		DrawLine(LEG(i).joint[j - 1].x, LEG(i).joint[j - 1].y, LEG(i).tip.x, LEG(i).tip.y, 0xff0000);
		//DrawCircle(LEG(i).tip.x, LEG(i).tip.y, 5, 0xffffff, true);
	}
	//DrawCircle(_oct.center.x , _oct.center.y, 30, 0xff0000, true);
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