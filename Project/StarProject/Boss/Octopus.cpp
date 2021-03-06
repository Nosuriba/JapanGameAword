#include "Octopus.h"
#include "../ResourceManager.h"

#include <DxLib.h>

#define LEG(x) _oct.legs[x]

constexpr int SPEED = 5;

Octopus::Octopus(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Player>& player,const Vector2& pos):Boss(camera,player)
{
	///変数初期化
	_damageFlag = true;
	_returnFlag = false;
	_maxAngle = 30;
	_timer = 0;
	_idx = 0;


	///音関係
	SE.damage = ResourceManager::GetInstance().LoadSound("../Sound/damage.mp3");
	SE.die = ResourceManager::GetInstance().LoadSound("../Sound/die2.mp3");
	SE.shot = ResourceManager::GetInstance().LoadSound("../Sound/Octopus/shot.mp3");
	SE.swing = ResourceManager::GetInstance().LoadSound("../Sound/Octopus/swing.mp3");
	SE.pitch = ResourceManager::GetInstance().LoadSound("../Sound/Octopus/punch.mp3");
	extend = ResourceManager::GetInstance().LoadSound("../Sound/Octopus/extend.mp3");
	setup = ResourceManager::GetInstance().LoadSound("../Sound/Octopus/setup.mp3");

	BGM = ResourceManager::GetInstance().LoadSound("../Sound/boss.mp3");

	///タコの初期化
	_oct.center = pos;
	da.emplace_back(DamageInfo(_oct.center, 75));
	_oct.r = 500;
	_oct.hedPos = _oct.center + Vector2(50, 0);
	da.emplace_back(DamageInfo(_oct.hedPos, 75));

	for (int i = 0; i < _oct.eyePos.size(); ++i) {
		_oct.eyePos[i] = _oct.center + Vector2(-45, 37 - 75 * i);
	}
	auto c = cos(DX_PI_F / 180 * 180);
	auto s = sin(DX_PI_F / 180 * 0);
	auto p = Vector2(c, s);
	_oct.movePos = _oct.center + p * _oct.r;

	_oct.root.resize(8);
	_oct.legs.resize(8);

	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		c = cos(radian / 2 * i - DX_PI_F / 180 * -90);
		s = sin(radian / 2 * i - DX_PI_F / 180 * -90);
		auto pos = Vector2(c, s);
		_oct.root[i] = _oct.center + pos * 50;
		LEG(i).tip = _oct.root[i]  + pos * _oct.r;
		LEG(i).joint.clear();
		auto width = 25;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint.emplace_back(_oct.root[i] + Vector2(c, s)*(_oct.r / LEG(i).T*(j + 1)));
			at.emplace_back(AttackInfo(_oct.root[i] + Vector2(c, s)*(_oct.r / LEG(i).T*(j + 1)),width-=2));
		}
		LEG(i).state = E_LEG_STATE::NORMAL;
		LEG(i).angle = (_maxAngle - _maxAngle / 2 - _maxAngle / 4) * SPEED*(i+1);
		LEG(i).cnt = 0;
	}

	///パーティクル初期化
	_particle.emplace_back(std::make_shared<Water>(_oct.center.x, _oct.center.y, 5000, _camera));

	_updater = &Octopus::NeturalUpdate;
}

///Inverse Kinematics(逆運動学)角度制限なし
void Octopus::IkCcd(Vector2 pos, int idx, int numMaxItaration)
{
	for (int it = 0; it < numMaxItaration; ++it) {
		for (int j = LEG(idx).T - 1; j > 0; --j) {
			auto t_vec = pos - LEG(idx).joint[j - 1];		//目標→関節
			auto p_vec = LEG(idx).tip - LEG(idx).joint[j - 1];		//先端→関節
			auto mat = MGetTranslate((-LEG(idx).joint[j - 1]).V_Cast());			//原点まで移動
			mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//回転
			mat = MMult(mat, MGetTranslate(LEG(idx).joint[j - 1].V_Cast()));		//元の位置に移動
			for (int itr = j; itr < LEG(idx).T; ++itr) {
				LEG(idx).joint[itr] = VTransform(LEG(idx).joint[itr].V_Cast(), mat);
			}
			LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		}
		auto t_vec = pos - _oct.root[idx];		//目標→関節
		auto p_vec = LEG(idx).tip - _oct.root[idx];		//先端→関節
		auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());			//原点まで移動
		mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));	//回転
		mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));		//元の位置に移動
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
}

///死亡処理
void Octopus::Die()
{
	quake = Vector2(10.f, 10.f);
	da.clear();
	at.clear();
	shot.clear();
	ChangeVolumeSoundMem((255 * 120) / 100, SE.die);
	PlaySoundMem(SE.die, DX_PLAYTYPE_BACK);
	_updater = &Octopus::DieUpdate;
}

///通常時の足の動き
void Octopus::Normal(int idx)
{
	if (!CheckSoundMem(SE.swing)) {
		ChangeVolumeSoundMem((255 * 90) / 100, SE.swing);
		PlaySoundMem(SE.swing, DX_PLAYTYPE_BACK);
	}
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	auto rad = radian / 2 * idx - DX_PI_F / 180 * -90;

	auto ang = abs((++LEG(idx).angle / SPEED) % _maxAngle - _maxAngle / 2) - _maxAngle / 4;
	rad = rad + DX_PI_F / 180 * ang;
	auto c = _oct.root[idx].x + cos(rad)*_oct.r;
	auto s = _oct.root[idx].y + sin(rad)*_oct.r;
	auto p_pos = Vector2(c, s);
	auto p = p_pos - _oct.root[idx];
	auto v = LEG(idx).joint[0] - _oct.root[idx];
	auto mat = MGetTranslate((-_oct.root[idx]).V_Cast());
	mat = MMult(mat, MGetRotVec2(v.V_Cast(), p.V_Cast()));
	mat = MMult(mat, MGetTranslate(_oct.root[idx].V_Cast()));

	for (int j = 0; j < LEG(idx).T; ++j) {				//第一関節から回転

		LEG(idx).joint[j] = VTransform(LEG(idx).joint[j].V_Cast(), mat);
	}
	if (LEG(idx).cnt > 0) {											
		//第二関節以降の回転
		LegMove(LEG(idx), 1);
	}

	LEG(idx).mat = MGetRotVec2(v.V_Cast(), p.V_Cast());			//第一関節の回転行列保存
	LEG(idx).tip = LEG(idx).joint[LEG(idx).T-1];
}

///足が遅れてついてくる
void Octopus::LegMove(E_Leg & leg, int idx)
{
	if (idx == leg.T) {
		return;
	}
	for (int j = idx; j < leg.T; ++j) {				//第二関節から回転
		auto mat = MGetTranslate((-leg.joint[idx]).V_Cast());
		mat = MMult(mat, leg.mat);
		mat = MMult(mat, MGetTranslate(leg.joint[idx].V_Cast()));
		leg.joint[j] = VTransform(leg.joint[j].V_Cast(), mat);
	}
	leg.tip = leg.joint[leg.T - 1];
	++idx;
	LegMove(leg, idx);
}

///パンチ攻撃
void Octopus::Punch(int idx)
{
	auto p = LEG(idx).tip - _player->GetInfo().center;
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
	else if ((LEG(idx).cnt > _oct.r / LEG(idx).T * 6) && (LEG(idx).cnt < _oct.r / LEG(idx).T * 8) && (_oct.r*2 > p_vec.Magnitude())&&!_returnFlag) {
		if (!CheckSoundMem(extend)) {
			ChangeVolumeSoundMem((255 * 100) / 100, extend);
			PlaySoundMem(extend, DX_PLAYTYPE_BACK);
		}
		for (int j = 2; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() + j+0.5f);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if (((LEG(idx).cnt > _oct.r / LEG(idx).T * 8)&&(p_vec.Magnitude()>_oct.r))||(_returnFlag && (p_vec.Magnitude() > _oct.r))) {
		for (int j = 2; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = LEG(idx).joint[j - 1] + p_vec.Normalized()*((LEG(idx).joint[j] - LEG(idx).joint[j - 1]).Magnitude() - j);
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
	}
	else if(((LEG(idx).cnt > _oct.r / LEG(idx).T * 10) && (p_vec.Magnitude() < _oct.r))||(_returnFlag && (p_vec.Magnitude() < _oct.r))){
		for (int j = 0; j < LEG(idx).T; ++j) {
			LEG(idx).joint[j] = _oct.root[idx] + p_vec.Normalized()*(_oct.r / LEG(idx).T*(j + 1));
		}
		LEG(idx).tip = LEG(idx).joint[LEG(idx).T - 1];
		LEG(idx).cnt = 0;
		LEG(idx).state = E_LEG_STATE::RE_MOVE;
		_returnFlag = false;
	}
	if ((LEG(idx).cnt == _oct.r / LEG(idx).T * 6))
	{
		ChangeVolumeSoundMem((255 * 100) / 100, setup);
		PlaySoundMem(setup, DX_PLAYTYPE_BACK);
	}
}

///タコ墨発射！
void Octopus::OctInk()
{
	if (!CheckSoundMem(SE.shot)){
		PlaySoundMem(SE.shot, DX_PLAYTYPE_BACK);
	}
	auto c = cos( DX_PI_F / 180 * 180);
	auto s = sin(DX_PI_F / 180 * 0);
	auto p = Vector2(c, s);
	auto pos = _oct.center + p * _oct.r;
	auto p_vec = pos - _oct.center;
	auto t_vec = _player->GetInfo().center - _oct.center;
	auto dot = Dot(p_vec.Normalized(), t_vec.Normalized());
	auto rad = acos(dot);
	if (_player->GetInfo().center.y > pos.y) {
		rad = -rad;
	}
	if (_timer % 5==0) {
		auto vel = Vector2(t_vec.Normalized().x * 2.0f, t_vec.Normalized().y * 2.0f);
		shot.emplace_back(ShotInfo(_oct.center, vel, 15));
	}

	_particle[0]->SetPos(_oct.center.x, _oct.center.y);
	_particle[0]->SetVelocity(20);
	_particle[0]->SetRota(rad * 180 / DX_PI_F + 180);
	_particle[0]->Create();
}

///ﾌﾟﾚｲﾔｰに一番近い足が追跡(IK)
void Octopus::Chase(int idx)
{
	auto p = LEG(idx).tip - _player->GetInfo().center;
	auto pos = LEG(idx).tip - p.Normalized() * 1;
	IkCcd(pos, idx, 12);
}

///ダメージ処理
void Octopus::OnDamage()
{
	if (_damageFlag) {
		PlaySoundMem(SE.damage, DX_PLAYTYPE_BACK);
		_oct.helth -= 10;
		_damageFlag = false;
	}
}

///足を元の位置に戻す
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

///ブロック衝突時
void Octopus::HitBlock()
{
	PlaySoundMem(SE.pitch, DX_PLAYTYPE_BACK);
	_returnFlag = true;
}

///当たり判定更新
void Octopus::HitUpd()
{
	auto attack = at.begin();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		auto width = 25;
		for (int j = 0; j < LEG(i).T; ++j) {
			*attack = AttackInfo(LEG(i).joint[j], width -= 2);
			++attack;
		}
	}
	auto damage = da.begin();
	*damage = DamageInfo(_oct.center, 75);
	damage++;
	*damage= DamageInfo(_oct.hedPos, 75);
	for (auto& sh:shot) {
		auto vel = sh._vel * 1.025f;
		auto p = sh._pos + vel;
		sh = ShotInfo(p, vel, 15);
	}
	if (!shot.empty()) {
		auto sh = shot.end();
		auto pos = (--sh)->_pos;
		auto c = _camera->CameraCorrection();
		auto p = pos - c;
		auto size = Stage::GetInstance().GetStageSize();
		if (p.x < 0 || p.x > size.x || p.y < 0 || p.y > size.y) {
			shot.clear();
		}
	}
}

//void Octopus::Move()
//{
//	auto t = _oct.movePos - _targetPos;
//	auto t_pos = _oct.movePos - t.Normalized()*3;
//	auto p_vec = _oct.movePos - _oct.center;
//	auto t_vec = t_pos - _oct.center;
//	
//	if ((_targetPos-t_pos).Magnitude()<1) {
//		return;
//	}
//	auto mat = MGetTranslate((-_oct.center).V_Cast());
//	mat = MMult(mat, MGetRotVec2(p_vec.V_Cast(), t_vec.V_Cast()));
//	mat = MMult(mat, MGetTranslate(_oct.center.V_Cast()));
//	_oct.movePos = VTransform(_oct.movePos.V_Cast(), mat);
//	_oct.hedPos = VTransform(_oct.hedPos.V_Cast(), mat);
//	for (auto& eye : _oct.eyePos) {
//		eye = VTransform(eye.V_Cast(), mat);
//	}
//	for (int i = 0; i < _oct.legs.size(); ++i) {
//		_oct.root[i] = VTransform(_oct.root[i].V_Cast(), mat);
//		for (int j = 0; j < LEG(i).T; j++) {
//			LEG(i).joint[j] = VTransform(LEG(i).joint[j].V_Cast(), mat);
//		}
//		LEG(i).tip = LEG(i).joint[LEG(i).T - 1];
//	}
//}

///戦闘時の更新
void Octopus::NeturalUpdate()
{
	int j = 0;
	HitUpd();

	float distance = 9999;
	if ((++_timer)% 150==0) {
		int i = GetRand(_oct.legs.size() - 3)+1;
		if (LEG(i).state == E_LEG_STATE::NORMAL) {
			LEG(i).cnt = 0;
			LEG(i).state = E_LEG_STATE::PUNCH;
		}
	}

	if ((_timer/100) % 8 == 0) {
		OctInk();
	}

	if (_oct.helth <= 0) {
		Die();
	}

	for (auto& leg : _oct.legs) {
		if (((_player->GetInfo().center - leg.tip).Magnitude() < distance)) {
			distance = (_player->GetInfo().center - leg.tip).Magnitude();
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
	//Move();
	if (!_damageFlag) {
		if (++_oct.interval > 60) {
			_damageFlag = true;
			_oct.interval = 0;
		}
	}
	
}

///死亡時の更新
void Octopus::DieUpdate()
{
	blendCnt--;

	quake *= 0.9995;
	//quake = (quake.x <= 0.05 ? Vector2() : Vector2(quake.x * 0.9995, quake.y * 0.9995));

	if (blendCnt <= 0)
	{
		_isDie = true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////描画関係///////////////////////////////////////////////

///デバッグ用
void Octopus::DebugDraw()
{
	auto c = _camera->CameraCorrection();
	for (auto attack : at) {
		DrawCircle(attack._pos.x - c.x, attack._pos.y - c.y, attack._r, 0x00ff00, true);
	}
	for (auto damage : da) {
		DrawCircle(damage._pos.x - c.x, damage._pos.y - c.y, damage._r, 0x0000ff, true);
	}
	for (auto sh : shot) {
		DrawCircle(sh._pos.x - c.x, sh._pos.y - c.y, sh._r, 0x00ffff, true);
	}
}


void Octopus::Draw()
{
	for (auto& p : _particle)
		p->Draw(0x000000);
	auto c = _camera->CameraCorrection();
	if (!_damageFlag && ((_oct.interval/5) % 2 == 0)) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
	}

	if (_updater == &Octopus::DieUpdate)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendCnt);

		quake.x = (GetRand(1) ? quake.x : -quake.x);
		quake.y = (GetRand(1) ? quake.y : -quake.y);

		c = c + quake;
	}

	
	
	//足の間の膜の描画
	for (int i = 0; i < _oct.legs.size()-1; ++i) {
		int j = 1;
		auto p1 = _oct.root[i];
		auto p2 = LEG(i).joint[j];
		auto p3 = LEG((i + 1)).joint[j];
		auto p4 = _oct.root[(i + 1) ];
		DrawQuadrangle(p1.x - c.x, p1.y - c.y, p2.x - c.x, p2.y - c.y, p3.x - c.x, p3.y - c.y, p4.x - c.x, p4.y - c.y, 0xbb0000, true);
	}

	//足の描画
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto width = 50;
		DrawLineAA(_oct.root[i].x - c.x, _oct.root[i].y - c.y, LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, 0xcc0000, width);
		for (j = 0; j < LEG(i).T - 1; ++j) {
			DrawLineAA(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, LEG(i).joint[j + 1].x - c.x, LEG(i).joint[j + 1].y - c.y, 0xcc0000, width-=4);
			DrawCircle(LEG(i).joint[j].x - c.x, LEG(i).joint[j].y - c.y, width / 2, 0xcc0000, true);
		}
	}

	//頭の描画
	DrawOval(_oct.hedPos.x - c.x, _oct.hedPos.y - c.y,125, 75, 0xee0000, true);
	for (int i = 0; i < 2; ++i) {
		DrawCircle(_oct.eyePos[i].x - c.x, _oct.eyePos[i].y - c.y, 8, 0xffa500, true);
		DrawCircle(_oct.eyePos[i].x - c.x, _oct.eyePos[i].y - c.y, 6,0x000000, true);
	}

	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	
#ifdef _DEBUG
	DebugDraw();
#endif // DEBUG

}

///影
void Octopus::ShadowDraw()
{
	auto c = _camera->CameraCorrection();
	auto s = _camera->GetShadowPos(0.3f);

	if (_damageFlag) {
		//足の間の膜の描画
		for (int i = 0; i < _oct.legs.size() - 1; ++i) {
			int j = 1;
			auto p1 = _oct.root[i];
			auto p2 = LEG(i).joint[j];
			auto p3 = LEG((i + 1)).joint[j];
			auto p4 = _oct.root[(i + 1)];
			DrawQuadrangle(p1.x - c.x + s.x, p1.y - c.y + s.y, p2.x - c.x + s.x, p2.y - c.y + s.y,
				p3.x - c.x + s.x, p3.y - c.y + s.y, p4.x - c.x + s.x, p4.y - c.y + s.y, 0xffffff, true);
		}
		//足の描画
		for (int i = 0; i < _oct.legs.size(); ++i) {
			int j = 0;
			auto width = 50;
			DrawLineAA(_oct.root[i].x - c.x + s.x, _oct.root[i].y - c.y + s.y, LEG(i).joint[j].x - c.x + s.x, LEG(i).joint[j].y - c.y + s.y, 0xffffff, width);
			for (j = 0; j < LEG(i).T - 1; ++j) {
				DrawLineAA(LEG(i).joint[j].x - c.x + s.x, LEG(i).joint[j].y - c.y + s.y, LEG(i).joint[j + 1].x - c.x + s.x, LEG(i).joint[j + 1].y - c.y + s.y, 0xffffff, width -= 4);
				DrawCircle(LEG(i).joint[j].x - c.x + s.x, LEG(i).joint[j].y - c.y + s.y, width / 2, 0xffffff, true);
			}
		}
		//頭の描画
		DrawOval(_oct.hedPos.x - c.x + s.x, _oct.hedPos.y - c.y + s.y, 125, 75, 0xffffff, true);
		for (int i = 0; i < 2; ++i) {
			DrawCircle(_oct.eyePos[i].x - c.x + s.x, _oct.eyePos[i].y - c.y + s.y, 8, 0xffffff, true);
			DrawCircle(_oct.eyePos[i].x - c.x + s.x, _oct.eyePos[i].y - c.y + s.y, 6, 0xffffff, true);
		}
	}
}

///セレクトシーン用
void Octopus::SelectDraw(const Vector2 p, const float s)
{
	_oct.center = p;
	auto range = _oct.r*s;
	_oct.hedPos = _oct.center + Vector2(50, 0)*s;
	for (int i = 0; i < _oct.eyePos.size(); ++i) {
		_oct.eyePos[i] = _oct.center + Vector2(-45, 37 - 75 * i)*s;
	}
	
	auto radian = 2.0f * DX_PI_F / (float)_oct.legs.size();
	for (int i = 0; i < _oct.legs.size(); ++i) {
		auto co = cos(radian / 2 * i - DX_PI_F / 180 * -90);
		auto si = sin(radian / 2 * i - DX_PI_F / 180 * -90);
		auto pos = Vector2(co, si);
		_oct.root[i] = _oct.center + pos * 50 * s;
		LEG(i).tip = _oct.root[i] + pos * range;
		for (int j = 0; j < LEG(i).T; ++j) {
			LEG(i).joint[j]=_oct.root[i] + Vector2(co, si)*(range / LEG(i).T*(j + 1));
		}
	}

	//足の間の膜の描画
	for (int i = 0; i < _oct.legs.size() - 1; ++i) {
		int j = 1;
		auto p1 = _oct.root[i];
		auto p2 = LEG(i).joint[j];
		auto p3 = LEG((i + 1)).joint[j];
		auto p4 = _oct.root[(i + 1)];
		DrawQuadrangle(p1.x, p1.y, p2.x , p2.y, p3.x, p3.y, p4.x, p4.y, 0xbb0000, true);
	}
	

	//足の描画
	for (int i = 0; i < _oct.legs.size(); ++i) {
		int j = 0;
		auto width = 50*s;
		DrawLineAA(_oct.root[i].x, _oct.root[i].y, LEG(i).joint[j].x, LEG(i).joint[j].y, 0xcc0000, width);
		for (j = 0; j < LEG(i).T - 1; ++j) {
			DrawLineAA(LEG(i).joint[j].x , LEG(i).joint[j].y , LEG(i).joint[j + 1].x , LEG(i).joint[j + 1].y , 0xcc0000, (width -= 1));
			DrawCircle(LEG(i).joint[j].x , LEG(i).joint[j].y , (width-=1) / 2, 0xcc0000, true);
		}
	}
	//頭の描画
	DrawOval(_oct.hedPos.x , _oct.hedPos.y ,125*s, 75*s, 0xee0000, true);
	for (int i = 0; i < 2; ++i) {
		DrawCircle(_oct.eyePos[i].x , _oct.eyePos[i].y , 8*s, 0xffa500, true);
		DrawCircle(_oct.eyePos[i].x, _oct.eyePos[i].y , 4*s, 0x000000, true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

///更新
void Octopus::Update()
{
	if (!CheckSoundMem(BGM)){
		ChangeVolumeSoundMem(255 * 200 / 180, BGM);
		PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
	}
	(this->*_updater)();
}


Octopus::~Octopus()
{
	if (CheckSoundMem(BGM)){
		StopSoundMem(BGM);
	}
}