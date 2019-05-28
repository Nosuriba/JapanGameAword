#include "Octopus.h"

#include <DxLib.h>

#include "../Camera.h"
#include "../Game.h"
#include "../Scene/GameScene.h"

#define LEG(x) _oct.legs[x]

constexpr int SPEED = 5;

Octopus::Octopus(std::shared_ptr<Camera>& camera) : Boss(camera), _camera(camera)
{
	_damageFlag = true;
	_maxAngle = 30;
	_wait = 0;
	_timer = 0;
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
	_particle.emplace_back(std::make_shared<Water>(_oct.center.x, _oct.center.y, 5000, _camera));

	_idx = 0;
	_updater = &Octopus::NeturalUpdate;
}

void Octopus::IkCcd(Vector2 pos, int idx, int numMaxItaration)
{
	for (int it = 0; it < numMaxItaration; ++it) {
		for (int j = LEG(idx).T - 1; j > 0; --j) {
			auto t_vec = pos - LEG(idx).joint[j - 1];		//ñ⁄ïWÅ®ä÷êﬂ
			auto p_vec = LEG(idx).tip - LEG(idx).joint[j - 1];		//êÊí[Å®ä÷êﬂ
			auto mat = MGetTranslate((-LEG(idx).joint[j - 1]).V_Cast());			//å¥ì_Ç‹Ç≈à⁄ìÆ
			mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//âÒì]
			mat = MMult(mat, MGetTranslate(LEG(idx).joint[j - 1].V_Cast()));		//å≥ÇÃà íuÇ…à⁄ìÆ
			for (int itr = j; itr < LEG(idx).T; ++itr) {
				LEG(idx).joint[itr] = VTransform(LEG(idx).joint[itr].V_Cast(), mat);
			}
			LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		}
		auto t_vec = pos - _oct.root[idx];		//ñ⁄ïWÅ®ä÷êﬂ
		auto p_vec = LEG(idx).tip - _oct.root[idx];		//êÊí[Å®ä÷êﬂ
		auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());			//å¥ì_Ç‹Ç≈à⁄ìÆ
		mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//âÒì]
		mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));		//å≥ÇÃà íuÇ…à⁄ìÆ
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
}

void Octopus::Die()
{
	_updater = &Octopus::DieUpdate;
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

	for (int j = 0; j < LEG(idx).T; ++j) {				//ëÊàÍä÷êﬂÇ©ÇÁâÒì]

		LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
	}
	if (LEG(idx).cnt > 0) {											
		//ëÊìÒä÷êﬂà»ç~ÇÃâÒì]
		LegMove(LEG(idx), 1);
	}

	LEG(idx).mat = MGetRotVec2(v.V_Cast(), p.V_Cast());			//ëÊàÍä÷êﬂÇÃâÒì]çsóÒï€ë∂
	LEG(idx).tip = LEG(idx).joint[LEG(idx).T-1];
}

void Octopus::Punch(int idx)
{
	auto p = LEG(idx).tip - _targetPos;
	auto pos = LEG(idx).tip - p.Normalized() * 10;
	auto t_vec = pos - _oct.root[idx];
	auto p_vec = LEG(idx).tip - _oct.root[idx];
	if (++LEG(idx).cnt < _oct.r / LEG(idx).T * 6) {
		auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());
		mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));
		mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		p_vec = LEG(idx).tip - _oct.root[idx];
		if (p_vec.Magnitude() > _oct.r / 2) {
			for (int j = 2; j < LEG(idx).T; ++j) {
				LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() - 0.3f);
			}
			LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		}
	}
	else if ((LEG(idx).cnt > _oct.r / LEG(idx).T * 6) && (LEG(idx).cnt < _oct.r / LEG(idx).T * 8) && (_oct.r*2 > p_vec.Magnitude())) {
		for (int j = 2; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() + j+0.5f);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if ((LEG(idx).cnt > _oct.r / LEG(idx).T * 8)&&(p_vec.Magnitude()>_oct.r)) {
		for (int j = 2; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() - j);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if((LEG(idx).cnt > _oct.r / LEG(idx).T * 10) && (p_vec.Magnitude() < _oct.r)){
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = _oct.root[idx] + p_vec.Normalized()*(_oct.r / LEG(idx).T*(j + 1));
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		LEG(idx).cnt = 0;
		LEG(idx).state = E_LEG_STATE::RE_MOVE;
	}
}

void Octopus::OctInk()
{
	_particle[0]->SetPos(_oct.center.x, _oct.center.y);
	_particle[0]->SetRota(180);
	_particle[0]->Create();
}

void Octopus::Chase(int idx)
{
	auto p = LEG(idx).tip - _targetPos;
	auto pos = LEG(idx).tip - p.Normalized() * 1;
	IkCcd(pos, idx, 12);
}

void Octopus::Damage()
{
	if (_damageFlag) {
		_oct.helth -= 10;
		_damageFlag = false;
	}
}

void Octopus::ReMove(int idx)
{
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	auto c = cos(radian / 2 * idx - DX_PI_F / 180 * -90);
	auto s = sin(radian / 2 * idx - DX_PI_F / 180 * -90);
	auto t = Vector2(c, s);
	auto target = _oct.root[idx] + t * (_oct.r+30);
	auto p = LEG(idx).tip - target;
	auto pos = LEG(idx).tip - p.Normalized() * 2;
	IkCcd(pos, idx, 100);
	if ((LEG(idx).tip - (_oct.root[idx] + t * _oct.r)).Magnitude() < 10) {
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
	for (int j = idx; j < leg.T; ++j) {				//ëÊìÒä÷êﬂÇ©ÇÁâÒì]
		auto mat = MGetTranslate((-leg.joint[idx]).V_Cast());
		mat = MMult(mat, leg.mat);
		mat = MMult(mat, MGetTranslate(leg.joint[idx].V_Cast()));
		leg.joint[j] = VTransform(leg.joint[j].V_Cast(), mat);
	}
	leg.tip = leg.joint[leg.T - 1];
	++idx;
	LegMove(leg, idx);
}

void Octopus::NeturalUpdate()
{
	int j = 0;
	float distance = 9999;
	if ((++_timer)% 200==0) {
		int i = GetRand(_oct.legs.size() - 3)+1;
		if (LEG(i).state == E_LEG_STATE::NORMAL) {
			LEG(i).cnt = 0;
			LEG(i).state = E_LEG_STATE::PUNCH;
		}
	}

	if ((_timer/100) % 10 == 0) {
		OctInk();
	}

	if (_oct.helth <= 0) {
		Die();
	}

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
			if ((_idx != i) && (LEG(i).state == E_LEG_STATE::CHASE)) {
				LEG(i).state = E_LEG_STATE::RE_MOVE;
			}
		}
		if (LEG(i).state == E_LEG_STATE::PUNCH) {
			Punch(i);
		}
		
		if (LEG(i).state == E_LEG_STATE::CHASE) {
			Chase(i);
		}
		if (LEG(i).state == E_LEG_STATE::RE_MOVE) {
			ReMove(i);
		}
		if ((_idx == i)&&(LEG(i).state==E_LEG_STATE::NORMAL)) {
			LEG(i).state = E_LEG_STATE::CHASE;
		}
	}

	if (!_damageFlag) {
		if (++_oct.interval > 60) {
			_damageFlag = true;
			_oct.interval = 0;
		}
	}
	
}

void Octopus::Draw()
{
	for (auto& p : _particle)
		p->Draw(0x000000);
	auto c = _camera->CameraCorrection();
	if (!_damageFlag&&(_oct.interval%10==0)) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	}
	//ë´ÇÃä‘ÇÃñåÇÃï`âÊ
	for (int i = 0; i < _oct.legs.size()-1; ++i) {
		int j = 1;
		auto p1 = _oct.root[i];
		auto p2 = LEG(i).joint[j];
		auto p3 = LEG((i + 1)).joint[j];
		auto p4 = _oct.root[(i + 1) ];
		DrawQuadrangle(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xbb0000, true);
	}
	for (int i = 0; i <= _oct.legs.size(); ++i) {
		DrawCircle(_oct.root[i].x - c.x, _oct.root[i].y - c.y, 5, 0xfffffff, true);
	}
	DrawCircle(_oct.center.x - c.x, _oct.center.y - c.y, 5, 0xfffffff, true);

	//ë´ÇÃï`âÊ
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto width = 50;
		DrawLineAA(_oct.root[i].x - c.x, _oct.root[i].y - c.y, LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 0xcc0000, width);
		for (j = 0; j < LEG(i).T - 1; ++j) {
			DrawLineAA(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, LEG(i).joint[j + 1].x - c.x, LEG(i).joint[j + 1].y - c.y, 0xcc0000, width-=2);
			DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, (width-=2)/2, 0xcc0000, true);
		}
	}
	//ì™ÇÃï`âÊ
	DrawOval(_oct.center.x + 50-c.x, _oct.center.y-c.y, 125, 75, 0xee0000, true);
	for (int i = 0; i < 2; ++i) {
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 8,6, 0xffa500, true);
		DrawOval(_oct.center.x - 45 - c.x, _oct.center.y + 37 - 75 * i - c.y, 6, 3, 0x000000, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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