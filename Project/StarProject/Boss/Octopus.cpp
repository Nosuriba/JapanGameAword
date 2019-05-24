#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"
#include "../Scene/GameScene.h"

#define LEG(x) _oct.legs[x]

constexpr int SPEED = 5;

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_maxAngle = 30;
	_wait = 0;
	_oct.center = Vector2(1000, 400);
	
	_oct.root.resize(8);

	_oct.legs.resize(8);
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		auto c = cos(radian / 2 * i - DX_PI_F / 180 * -90);
		auto s = sin(radian / 2 * i - DX_PI_F / 180 * -90);
		auto pos = Vector2(c, s);
		_oct.root[i] = _oct.center + pos * 50;
		LEG(i).tip = _oct.root[i]  + pos * _oct.r;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint.push_back(_oct.root[i] + Vector2(c, s)*(_oct.r / LEG(i).T*(j + 1)));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
		LEG(i).angle = (_maxAngle - _maxAngle / 2 - _maxAngle / 4) * SPEED*(i+1);
		LEG(i).cnt = 0;
	}
	_idx = 0;
	_updater = &Octopus::NeturalUpdate;
}

void Octopus::IkCcd(E_Leg & leg, Vector2 pos, int idx, int numMaxItaration)
{
	for (int it = 0; it < numMaxItaration; ++it) {
		for (int j = leg.T - 1; j > 0; --j) {
			auto t_vec = pos - leg.joint[j - 1];		//–Ú•W¨ŠÖß
			auto p_vec = leg.tip - leg.joint[j - 1];		//æ’[¨ŠÖß
			auto mat = MGetTranslate((-leg.joint[j - 1]).V_Cast());			//Œ´“_‚Ü‚ÅˆÚ“®
			mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//‰ñ“]
			mat = MMult(mat, MGetTranslate(leg.joint[j - 1].V_Cast()));		//Œ³‚ÌˆÊ’u‚ÉˆÚ“®
			for (int itr = j; itr < leg.T; ++itr) {
				leg.joint[itr] = VTransform(leg.joint[itr].V_Cast(), mat);
			}
			leg.tip = leg.joint[leg.T - 1];
		}
		auto t_vec = pos - _oct.root[idx];		//–Ú•W¨ŠÖß
		auto p_vec = LEG(idx).tip - _oct.root[idx];		//æ’[¨ŠÖß
		auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());			//Œ´“_‚Ü‚ÅˆÚ“®
		mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//‰ñ“]
		mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));		//Œ³‚ÌˆÊ’u‚ÉˆÚ“®
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
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
	auto rad = radian / 2 * idx - DX_PI_F / 180 * -90;

	auto ang = abs((++LEG(idx).angle / SPEED) % _maxAngle - _maxAngle / 2) - _maxAngle / 4;
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
	if (LEG(idx).cnt > 0) {											
		//‘æ“ñŠÖßˆÈ~‚Ì‰ñ“]
		LegMove(LEG(idx), 1);
	}

	LEG(idx).mat = MGetRotVec2(v.V_Cast(), p.V_Cast());			//‘æˆêŠÖß‚Ì‰ñ“]s—ñ•Û‘¶
	LEG(idx).tip = LEG(idx).joint[LEG(idx).T-1];
}

void Octopus::Punch(E_Leg& leg, int idx)
{
	auto p = LEG(idx).tip - _targetPos;
	auto pos = LEG(idx).tip - p.Normalized() * 10;
	auto t_vec = pos - _oct.root[idx];
	auto p_vec = LEG(idx).tip - _oct.root[idx];
	if (++_wait < _oct.r/LEG(idx).T*10) {
		auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());
		mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));
		mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T-1];
		p_vec = LEG(idx).tip - _oct.root[idx];
		for (int j = 2; j < LEG(idx).T; ++j) {				
			LEG(idx).joint[j] = LEG(idx).joint[j-1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j-1]).Magnitude() - 0.2f);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if ((_wait > _oct.r / LEG(idx).T*10)&&(_wait < _oct.r / LEG(idx).T * 11)) {
		auto target = LEG(idx).tip - _oct.root[idx];
		for (int j = 2; j < LEG(idx).T; ++j) {				
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + target.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() +j);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if ((_wait > _oct.r / LEG(idx).T * 11)&&(p_vec.Magnitude()>_oct.r)) {
		for (int j = 2; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() - j);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else {
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = _oct.root[idx] + p_vec.Normalized()*(_oct.r / LEG(idx).T*(j + 1));
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];

		LEG(idx).state = E_LEG_STATE::RE_MOVE;
	}
}

void Octopus::OctInk(E_Leg& leg, int idx)
{

}

void Octopus::Chase(E_Leg& leg, int idx)
{
	auto p = LEG(idx).tip - _targetPos;
	auto pos = LEG(idx).tip - p.Normalized() * 1;
	IkCcd(LEG(idx), pos, idx, 12);
}

void Octopus::Damage()
{

}

void Octopus::ReMove(E_Leg & leg, int idx)
{
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	auto c = cos(radian / 2 * idx - DX_PI_F / 180 * -90);
	auto s = sin(radian / 2 * idx - DX_PI_F / 180 * -90);
	auto target = Vector2(c, s);
	target = _oct.root[idx] + target * _oct.r;
	auto p = LEG(idx).tip - target;
	auto pos = LEG(idx).tip - p.Normalized() * 2;
	IkCcd(LEG(idx), pos, idx, 100);
	if ((LEG(idx).tip - target).Magnitude() < 8) {
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = _oct.root[idx] + Vector2(c, s)*(_oct.r / LEG(idx).T*(j + 1));
		}
		LEG(idx).angle = (_maxAngle - _maxAngle / 2 - _maxAngle / 4) * 5;
		LEG(idx).cnt = 0;
		LEG(idx).state = E_LEG_STATE::NORMAL;
	}
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
		if (((_targetPos - leg.tip).Magnitude() < distance)) {
			distance = (_targetPos - leg.tip).Magnitude();
			_idx = j;
		}
		++j;
	}
	for (int i = 0; i < _oct.legs.size(); ++i) {
		if (LEG(i).state == E_LEG_STATE::NORMAL) {
			Normal(i);
			++LEG(i).cnt;
		}
		else {
			if (_idx != i) {
				LEG(i).state = E_LEG_STATE::RE_MOVE;
			}
		}
		if (LEG(i).state == E_LEG_STATE::PUNCH) {
			Punch(LEG(i), i);
		}
		if (LEG(i).state == E_LEG_STATE::OCT_INK) {

		}
		if (LEG(i).state == E_LEG_STATE::CHASE) {
			Chase(LEG(i), i);
		}
		if (LEG(i).state == E_LEG_STATE::RE_MOVE) {
			ReMove(LEG(i), i);
		}
		if (_idx == i) {
			LEG(i).state = E_LEG_STATE::CHASE;
		}
	}
	
}

void Octopus::Draw()
{
	auto c = _camera->CameraCorrection();
	//‘«‚ÌŠÔ‚Ì–Œ‚Ì•`‰æ
	for (int i = 0; i <= _oct.legs.size(); ++i) {
		int j = 1;
		auto p1 = _oct.root[i%_oct.legs.size()];
		auto p2 = LEG(i%_oct.legs.size()).joint[j];
		auto p3 = LEG((i + 1) % _oct.legs.size()).joint[j];
		auto p4 = _oct.root[(i + 1) % _oct.legs.size()];
		DrawQuadrangle(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xbb0000, true);
	}
	//‘«‚Ì•`‰æ
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto width = 50;
		DrawLineAA(_oct.root[i].x - c.x, _oct.root[i].y - c.y, LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 0xcc0000, width);
		for (j = 0; j < LEG(i).T - 1; ++j) {
			DrawLineAA(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, LEG(i).joint[j + 1].x - c.x, LEG(i).joint[j + 1].y - c.y, 0xcc0000, width-=2);
			DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, (width-=2)/2, 0xcc0000, true);
		}
	}
	//“ª‚Ì•`‰æ
	DrawOval(_oct.center.x + 50-c.x, _oct.center.y-c.y, 125, 75, 0xee0000, true);
	for (int i = 0; i < 2; ++i) {
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 8,6, 0xffa500, true);
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 6, 3, 0x000000, true);
	}
}

void Octopus::Update()
{
	(this->*_updater)();
}

BossInfo Octopus::GetInfo()
{
	return BossInfo();
}

void Octopus::CalTrackVel(const Vector2 & pos)
{
	_targetPos = pos;
}

Octopus::~Octopus()
{
}