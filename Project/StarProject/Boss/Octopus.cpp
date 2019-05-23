#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"
#include "../Scene/GameScene.h"

#define LEG(x) _oct.legs[x]

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	
	_oct.center = Vector2(900, 400);
	
	_oct.root.resize(8);

	_oct.r = 500;
	_oct.legs.resize(8);
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		auto c = cos(radian * i);
		auto s = sin(radian * i);
		auto pos = Vector2(c, s);
		_oct.root[i] = _oct.center + pos * 50;
		LEG(i).tip = _oct.root[i]  + pos * _oct.r;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint.push_back(_oct.root[i] + Vector2(c, s)*(_oct.r / LEG(i).T*(j + 1)));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
		LEG(i).angle = GetRand(radian / (2.0f*DX_PI_F)*360.0f) + 30;
	}
	id = 0;
	angle = 0;
	cnt = 0;
	_updater = &Octopus::NeturalUpdate;
}

void Octopus::Die()
{
}

void Octopus::DieUpdate()
{
}

void Octopus::Normal(int idx)
{
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	auto rad = radian * idx;

	auto ang = abs((++angle / 30) % LEG(idx).angle - LEG(idx).angle / 2) - LEG(idx).angle / 4;
	rad = rad + DX_PI_F / 180 * ang;
	auto c = _oct.root[idx].x + cos(rad)*_oct.r;
	auto s = _oct.root[idx].y + sin(rad)*_oct.r;
	auto p_pos = Vector2(c, s);
	auto v = LEG(idx).joint[0] - _oct.root[idx];
	auto p = p_pos - _oct.root[idx];
	auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());
	mat = MMult(mat, MGetRotVec2(v.V_Cast(), p.V_Cast()));
	mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));

	for (int j = 0; j < LEG(idx).T; ++j) {				//‘æˆêŠÖß‚©‚ç‰ñ“]

		LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
	}
	if (cnt > 0) {
		LegMove(LEG(idx), 1);
	}

	LEG(idx).mat = MGetRotVec2(v.V_Cast(), p.V_Cast());			//‘æˆêŠÖß‚Ì‰ñ“]s—ñ•Û‘¶
	LEG(idx).tip = LEG(idx).joint[LEG(idx).T-1];
}

void Octopus::Punch(E_Leg& leg, int idx)
{

}

void Octopus::OctInk(E_Leg& leg, int idx)
{
}

void Octopus::Chase(E_Leg& leg, int idx)
{
	for (int it = 0; it < 6; ++it) {
		for (int j = leg.T-1; j > 0; --j) {
			auto p_vec = targetPos - leg.joint[j - 1];		//–Ú•W¨ŠÖß
			auto t_vec = leg.tip - leg.joint[j - 1];		//æ’[¨ŠÖß
			auto mat = MGetTranslate((-leg.joint[j - 1]).V_Cast());			//Œ´“_‚Ü‚ÅˆÚ“®
			mat = MMult(mat, MGetRotVec2(t_vec.V_Cast(), p_vec.V_Cast()));	//‰ñ“]
			mat = MMult(mat, MGetTranslate(leg.joint[j - 1].V_Cast()));		//Œ³‚ÌˆÊ’u‚ÉˆÚ“®
			for (int itr = j; itr < leg.T; ++itr) {
				leg.joint[itr] = VTransform(leg.joint[itr].V_Cast(), mat);
			}
			leg.tip = leg.joint[leg.T-1];
		}
	}
}

void Octopus::Damage()
{
}

void Octopus::ReMove(E_Leg & leg, int idx)
{
}

void Octopus::LegMove(E_Leg & leg, int idx)
{
	if (idx == leg.T) {
		return;
	}
	for (int j = idx; j < leg.T; ++j) {				//‘æ“ñŠÖß‚©‚ç‰ñ“]
		auto mat = MGetTranslate((-leg.joint[idx]).V_Cast());
		mat = MMult(mat, leg.mat);
		mat = MMult(mat, MGetTranslate(leg.joint[idx].V_Cast()));
		leg.joint[j] = VTransform(leg.joint[j].V_Cast(), mat);
	}
	++idx;
	LegMove(leg, idx);
}

void Octopus::NeturalUpdate()
{
	int j = 0;
	float distance = 9999;
	for (auto& leg : _oct.legs) {
		if ((targetPos - leg.tip).Magnitude() < distance) {
			distance = (targetPos - leg.tip).Magnitude();
			id = j;
		}
		++j;
	}
	for (int i = 0; i < _oct.legs.size(); ++i) {
		if (LEG(i).state == E_LEG_STATE::NORMAL) {
			Normal(i);
		}
		if (LEG(i).state == E_LEG_STATE::PUNCH) {

		}
		if (LEG(i).state == E_LEG_STATE::OCT_INK) {

		}
		if (LEG(i).state == E_LEG_STATE::CHASE) {
			auto p = LEG(i).tip - targetPos;
			auto pos = LEG(i).tip - p.Normalized() * 1;

			Chase(LEG(i), i);

		}
		if (id == i) {
			LEG(i).state = E_LEG_STATE::CHASE;
		}
		else {
			LEG(i).state = E_LEG_STATE::NORMAL;
		}
	}
	
}

void Octopus::Draw()
{
	auto c = _camera->CameraCorrection();
	for (int i = 0; i <= _oct.legs.size(); ++i) {
		int j = 1;
		auto p1 = _oct.root[i%_oct.legs.size()];
		auto p2 = LEG(i%_oct.legs.size()).joint[j];
		auto p3 = LEG((i + 1) % _oct.legs.size()).joint[j];
		auto p4 = _oct.root[(i + 1) % _oct.legs.size()];
		DrawQuadrangle(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xbb0000, true);
	}

	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto width = 50;

		/*auto p1 = _oct.root[i] + Vector2(0, 1)*width;
		auto p2 = LEG(i).joint[j] + Vector2(0, 1)*width;
		auto p3 = LEG(i).joint[j] + Vector2(0, -1)*width;
		auto p4 = _oct.root[i] + Vector2(0, -1)*width;*/
		//DrawQuadrangleAA(p1.x-c.x, p1.y-c.y, p2.x-c.x, p2.y-c.y, p3.x-c.x, p3.y-c.y, p4.x-c.x, p4.y-c.y, 0xaa0000, true);

		DrawLineAA(_oct.root[i].x - c.x, _oct.root[i].y - c.y, LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 0xcc0000, width);

		for (j = 0; j < LEG(i).T - 1; ++j) {

			/*p1 = p2;
			p2 = LEG(i).joint[j + 1] + Vector2(0, 1)*width;
			p4 = p3;
			p3 = LEG(i).joint[j + 1] + Vector2(0, -1)*width;*/
			//DrawQuadrangleAA(p1.x-c.x, p1.y-c.y, p2.x-c.x, p2.y-c.y, p3.x-c.x, p3.y-c.y, p4.x-c.x, p4.y-c.y, 0xaa0000, true);
			DrawLineAA(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, LEG(i).joint[j + 1].x - c.x, LEG(i).joint[j + 1].y - c.y, 0xcc0000, width-=2);
			DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, (width-=2)/2, 0xcc0000, true);
		}
	}
	
	DrawOval(_oct.center.x + 50-c.x, _oct.center.y-c.y, 125, 75, 0xee0000, true);
	//DrawOval(_oct.center.x + 50-c.x, _oct.center.y-c.y, 125, 75, 0x111111, false);
	for (int i = 0; i < 2; ++i) {
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 8,6, 0xffa500, true);
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 6, 3, 0x000000, true);
	}
}

void Octopus::Update()
{
	(this->*_updater)();
	++cnt;
}

BossInfo Octopus::GetInfo()
{
	return BossInfo();
}

void Octopus::CalTrackVel(const Vector2 & pos)
{
	targetPos = pos;
}

Octopus::~Octopus()
{
}