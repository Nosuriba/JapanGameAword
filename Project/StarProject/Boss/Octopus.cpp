#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"

#define LEG(x) _oct.legs[x]

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	
	_oct.center = Vector2(900, 300);
	auto size = Size(100, 150);
	auto rect = Rect(_oct.center, size);
	_oct.root = _oct.center + Vector2(-80,0);
	_oct.r = 500;
	_oct.legs.resize(8);
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		auto c = cos(radian * i);
		auto s = sin(radian * i);
		LEG(i).tip.x = _oct.root.x + c * _oct.r;
		LEG(i).tip.y = _oct.root.y + s * _oct.r;
		LEG(i).angle = 0;
		for (int j = 0; j <= LEG(i).T; ++j) {
			LEG(i).joint.push_back(_oct.root + Vector2(c, s)*(_oct.r / LEG(i).T*(j + 1)));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
	}
	angle = 0;
	_updater = &Octopus::NeturalUpdate;
}

void Octopus::Die()
{
}

void Octopus::DieUpdate()
{
}

void Octopus::Normal(E_Leg& leg,Vector2 pos)
{
	
	for (int it = 0; it < 6; ++it) {
		for (int j = leg.T; j > 0; --j) {
			auto p_vec = pos - leg.joint[j - 1];		//目標→関節
			auto t_vec = leg.tip - leg.joint[j - 1];		//先端→関節
			auto mat = MGetTranslate((-leg.joint[j - 1]).V_Cast());			//原点まで移動
			mat = MMult(mat, MGetRotVec2(t_vec.V_Cast(), p_vec.V_Cast()));	//回転
			mat = MMult(mat, MGetTranslate(leg.joint[j - 1].V_Cast()));		//元の位置に移動
			for (int itr = j; itr <= leg.T; ++itr) {
				leg.joint[itr] = VTransform(leg.joint[itr].V_Cast(), mat);
			}
			leg.tip = leg.joint[leg.T];
		}
	}
}

void Octopus::Attack()
{
}

void Octopus::Damage()
{
}

void Octopus::NeturalUpdate()
{
	for (int i = 0; i < _oct.legs.size(); ++i) {
		if (LEG(i).state == E_LEG_STATE::NORMAL) {
			if (LEG(i).angle == 0) {
				LEG(i).angle = 160 + GetRand(5);
			}
			auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
			auto rad = radian * i;

			auto ang = abs(angle % LEG(i).angle - LEG(i).angle / 2);
			rad = rad + DX_PI_F / 180 * ang;
			auto c = _oct.root.x + cos(rad)*_oct.r;
			auto s = _oct.root.y + sin(rad)*_oct.r;
			auto p_pos = Vector2(c, s);
			Normal(LEG(i), p_pos);
		}
		if (LEG(i).state == E_LEG_STATE::PUNCH) {

		}
		
	}
}

void Octopus::Draw()
{
	auto c = _camera->CameraCorrection();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto v = LEG(i).joint[j] - LEG(i).joint[j + 1];
		auto mat = MGetRotZ(DX_PI_F / 180 * 90.0f);
		v = VTransform(v.V_Cast(), mat);
		auto p = v.Normalized() * 25;
		v = LEG(i).joint[j] - LEG(i).joint[j + 1];
		mat = MGetRotZ(DX_PI_F / 180 * (-90.0f));
		v = VTransform(v.V_Cast(), mat);
		auto _p = v.Normalized() * 25;
		auto p1 = LEG(i).joint[j] + p;
		auto p2 = LEG(i).joint[j+1] + p;
		auto p3 = LEG(i).joint[j+1] + _p;
		auto p4 = LEG(i).joint[j] + _p;
		DrawQuadrangleAA(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xaa0000, true);
		//DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 5, 0x00ff00, true);
		for (j = 1; j < LEG(i).T; ++j) {
			v = LEG(i).joint[j] - LEG(i).joint[j + 1];
			mat = MGetRotZ(DX_PI_F / 180 * 90.0f);
			v = VTransform(v.V_Cast(), mat);
			p = v.Normalized() * 25;
			v = LEG(i).joint[j] - LEG(i).joint[j + 1];
			mat = MGetRotZ(DX_PI_F / 180 * (-90.0f));
			v = VTransform(v.V_Cast(), mat);
			_p = v.Normalized() * 25;
			p1 = p2;
			p2 = LEG(i).joint[j + 1] + p;
			p4 = p3;
			p3 = LEG(i).joint[j + 1] + _p;
			DrawQuadrangleAA(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xaa0000, true);
			//DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 5, 0x00ff00, true);
		}
	}
	DrawOval(_oct.center.x , _oct.center.y, 150,100, 0xaa0000, true);
	DrawOval(_oct.center.x , _oct.center.y, 150,100, 0x111111, false);
}

void Octopus::Update()
{
	angle++;
	(this->*_updater)();
}

BossInfo Octopus::GetInfo()
{
	return BossInfo();
}

Octopus::~Octopus()
{
}