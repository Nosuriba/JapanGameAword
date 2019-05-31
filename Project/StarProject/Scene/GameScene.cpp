#include "GameScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Shot.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Fish.h"
#include "../Enemy/Diodon.h"
#include "../Enemy/SeaCucumber.h"
#include "../Boss/Boss.h"
#include "../Boss/Crab.h"
#include "../Boss/Octopus.h"
#include "../Processing/Collision.h"
#include "../Camera.h"
#include "../Object/Obstacle.h"
#include "../Object/DestroyableObject.h"
#include "../Object/PredatoryObject.h"
#include "../Object/ImmortalObject.h"
#include "../Object/GoalObject.h"
#include "../Stage.h"
#include "../Hart.h"

#include <iostream>
#include <algorithm>

#define CC _camera->CameraCorrection()

const int shader_offset = 50;
const auto size = Game::GetInstance().GetScreenSize();

void GameScene::LoadStageUpdate(const Input & p)
{
	auto &_stage = Stage::GetInstance();

	if(_stage.LoadCheck()) {
		LoadResource();
		_camera->SetRange(Vector2(_stage.GetStageSize().x, _stage.GetStageSize().y));
		_updater = &GameScene::LoadResourceUpdate;
	}
	nlDraw();
}

void GameScene::LoadResourceUpdate(const Input & p)
{
	int i = GetASyncLoadNum();
	if (GetASyncLoadNum() == 0)
	{
		fadewait = 0;
		_updater = &GameScene::FadeIn;
	}
	nlDraw();
}

void GameScene::FadeIn(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 - 255 * (float)(fadewait) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (fadewait >= WAITFRAME) {
		waitCnt = 0;
		_updater = &GameScene::Wait;
	}
}

void GameScene::FadeOut(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	DrawRotaGraph(size.x - cutinCnt, size.y / 2, 1, 0, gameclear, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * (float)(fadewait) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (fadewait >= WAITFRAME) {
		Game::GetInstance().ChangeScene(new ResultScene(score.enemy,score.bite,score.breakobj,totaltime));
	}
	else {
		(*FadeBubble).Create();
	}
}

void GameScene::Wait(const Input & p)
{
	auto& size = Game::GetInstance().GetScreenSize();

	Draw();
	
	if (waitNum == 0) {
		_updater = &GameScene::Run;
	}
	else if ((waitCnt % 60) == 0) {
		waitNum--;
	}
}

void GameScene::Run(const Input & p)
{
	//////////////////////// 削除 ///////////////////////////////
	// 敵
	_enemies.remove_if([](std::shared_ptr<Enemy>& e) {return !e->GetInfo()._isAlive; });
	
	// 破壊可能オブジェクト
	_destroyObj.remove_if([](std::shared_ptr<DestroyableObject>& d) {return d->GetInfo()._breakflag; });

	//捕食可能オブジェクト
	_predatoryObj.remove_if([](std::shared_ptr<PredatoryObject>& p) {return p->GetInfo()._breakflag || p->GetInfo()._hitflag; });

	/////////////////////////////////////////////////////////////

	//////////////////////// 更新 ///////////////////////////////

	_pl->Update(p);

	for (auto &enemy : _enemies)
	{
		enemy->Update();
	}

	for (auto &boss : _bosses)
	{
		boss->Update();
	}

	for (auto predatry : _predatoryObj) {
		predatry->Update();
	}

	for (auto destroy : _destroyObj) {
		destroy->Update();
	}

	gameCnt++;
	score.bite = _pl->GetEatCnt();

	/////////////////////////////////////////////////////////////

	////////////////////// 当たり判定 ///////////////////////////

	auto& size		= Game::GetInstance().GetScreenSize();

	auto& lasers	= _pl->GetLaser();
	auto& pRec		= _pl->GetDamageRec();

	// 敵
	for (auto& e : _enemies)
	{
		auto _p = e->GetInfo()._pos - CC;
		// 画面外
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		// ダメージを受けている
		if (e->GetInfo()._isDamage) continue;

		// Laserの当たり
		for (auto& laser : lasers)
		{
			for (auto l = laser.begin(); l != laser.end(); l++)
			{
				auto np = (*l)->EndCheck() ? (*l)->GetPos() : (std::next(l) != laser.end()) ? (*std::next(l))->GetPos() : (*l)->GetPos();

				for (auto& er : e->GetDamaRec())
				{
					if (_col->WaterToCircle((*l)->GetPos(), np, er.pos, er.r))
					{
						e->OnDamage();
						if (l != laser.begin())
						{
							(*--l)->End();
							(*++l)->Hit();
						}
						else
							(*l)->Hit();
						break;
					}
				}
			}
		}

		// ダメージを受けた
		if (e->GetInfo()._isDamage) continue;

		// プレイヤーの当たり
		for (auto& er : e->GetAttaRec())
		{
			for (auto& pr : pRec)
			{
				if (_col->CircleToCircle(pr.pos, pr.r, er.pos, er.r))
				{
					_pl->OnDamage();
					break;
				}
			}
		}
	}

	// ボス
	for (auto& b : _bosses)
	{
		// Laserの当たり
		for (auto& laser : lasers)
		{
			for (auto l = laser.begin(); l != laser.end(); l++)
			{
				auto np = (*l)->EndCheck() ? (*l)->GetPos() : (std::next(l) != laser.end()) ? (*std::next(l))->GetPos() : (*l)->GetPos();

				for (auto& br : b->GetDamageInfo())
				{
					if (_col->WaterToCircle((*l)->GetPos(), np, br._pos, br._r))
					{
						b->OnDamage();
						if (l != laser.begin())
						{
							(*--l)->End();
							(*++l)->Hit();
						}
						else
							(*l)->Hit();
						break;
					}
				}
			}
		}

		// ボスの物理で殴る
		for (auto& br : b->GetAttackInfo())
		{
			for (auto& pr : pRec)
			{
				if (_col->CircleToCircle(pr.pos, pr.r, br._pos, br._r))
				{
					_pl->OnDamage();
				}
			}
			for (auto immortal : _immortalObj)
			{
				auto _p = immortal->GetInfo()._pos - CC;
				if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

				if (_col->CircleToSqr(br._pos, br._r, immortal->GetInfo()._rect))
				{
					b->HitBlock();
				}
			}
		}

		// ボスの遠距離攻撃
		for (auto& bs : b->GetShotInfo())
		{
			for (auto& pr : pRec)
			{
				if (_col->CircleToCircle(pr.pos, pr.r, bs._pos, bs._r))
				{
					_pl->OnDamage();
				}
			}
		}
	}

	// 捕食対象
	for (auto& predatry : _predatoryObj)
	{
		auto _p = predatry->GetInfo()._pos - CC;
		// 画面外
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		// Laserの当たり
		for (auto& laser : lasers)
		{
			for (auto l = laser.begin(); l != laser.end(); l++)
			{
				auto np = (*l)->EndCheck() ? (*l)->GetPos() : (std::next(l) != laser.end()) ? (*std::next(l))->GetPos() : (*l)->GetPos();

				if (_col->WaterToSqr((*l)->GetPos(), np, predatry->GetInfo()._rect))
				{
					predatry->Break();
					if (l != laser.begin())
					{
						(*--l)->End();
						(*++l)->Hit();
					}
					else
						(*l)->Hit();
					break;
				}
			}
		}

		// ダメージを受けた
		if (predatry->GetInfo()._breakflag) continue;

		// プレイヤーの当たり
		for (auto& pr : pRec)
		{
			if (_col->CircleToSqr(pr.pos, pr.r, predatry->GetInfo()._rect))
			{
				_pl->ToCatch(predatry->GetInfo()._pos);
				predatry->Predatory();
				break;
			}
		}
	}

	// 破壊可能オブジェクト
	for (auto destroy : _destroyObj)
	{
		auto _p = destroy->GetInfo()._pos - CC;
		// 画面外
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;
		// 壊れてる
		if (destroy->GetInfo()._hitflag) continue;

		// レーザーの当たり
		for (auto& laser : lasers)
		{
			// レベル不足
			if (_pl->GetInfo().level < destroy->GetInfo()._level) break;

			for (auto l = laser.begin(); l != laser.end(); l++)
			{
				auto np = (*l)->EndCheck() ? (*l)->GetPos() : (std::next(l) != laser.end()) ? (*std::next(l))->GetPos() : (*l)->GetPos();

				if (_col->WaterToSqr((*l)->GetPos(), np, destroy->GetInfo()._rect))
				{
					destroy->Break();
					if (l != laser.begin())
					{
						(*--l)->End();
						(*++l)->Hit();
					}
					else
						(*l)->Hit();
					break;
				}
			}
		}

		// ダメージを受けた
		if (destroy->GetInfo()._breakflag) continue;

		// プレイヤーの当たり
		for (auto& pr : pRec)
		{
			if (_col->CircleToSqr(pr.pos, pr.r, destroy->GetInfo()._rect))
			{
				_pl->SetStar(_col->Pushback(_pl->GetInfo(), destroy->GetInfo()._rect), _pl->GetInfo().r);
			}
		}
	}

	//破壊不可オブジェクト
	for (auto immortal : _immortalObj)
	{
		auto _p = immortal->GetInfo()._pos - CC;
		// 画面外
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		// レーザーの当たり
		for (auto& laser : lasers)
		{
			for (auto l = laser.begin(); l != laser.end(); l++)
			{
				auto np = (*l)->EndCheck() ? (*l)->GetPos() : (std::next(l) != laser.end()) ? (*std::next(l))->GetPos() : (*l)->GetPos();

				if (_col->WaterToSqr((*l)->GetPos(), np, immortal->GetInfo()._rect))
				{

					if (l != laser.begin())
					{
						(*--l)->End();
						(*++l)->Hit();
					}
					else
						(*l)->Hit();
					break;
				}
			}
		}
		
		// プレイヤーの当たり
		for (auto& pr : pRec)
		{
			if (_col->CircleToSqr(pr.pos, pr.r, immortal->GetInfo()._rect))
			{
				_pl->SetStar(_col->Pushback(_pl->GetInfo(), immortal->GetInfo()._rect), _pl->GetInfo().r);
			}
		}
	}

	//ゴールオブジェクト
	for (auto goal : _goalObject)
	{
		auto _p = goal->GetInfo()._pos - CC;
		// 画面外
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		// プレイヤーの当たり
		for (auto& pr : pRec)
		{
			if (_col->CircleToSqr(pr.pos, pr.r, goal->GetInfo()._rect))
			{
				_updater = &GameScene::CutinUpdate;
			}
		}
	}

	///////////////////////////////////////////////////////////////


	Draw();

	flame++;

	if (totaltime == 0) {
		fadewait = 0;
		_updater = &GameScene::FadeOut;
	}
	if (_pl->CheckDie())
	{
		fadewait = 0;
		_updater = &GameScene::FadeOut;
	}

#ifdef _DEBUG

	if (p.Trigger(BUTTON::A) || p.IsTrigger(PAD_INPUT_10)) {
		fadewait = 0;
		_updater = &GameScene::FadeOut;
	}

#endif // _DEBUG
	int c = 0;
	for (auto hart = _Harts.begin(); hart != _Harts.end(); hart++)
	{
		(*hart)->UpDate();
		if (_pl->GetLife() < ++c)
		{
			(*hart)->Break();
		}
	}
}

void GameScene::CutinUpdate(const Input & p)
{
	Draw();
	cutinCnt += 10;

	DrawRotaGraph(size.x - cutinCnt, size.y / 2, 1, 0, gameclear, true);

	if (cutinCnt >= size.x / 2) {
		fadewait = 0;
		_updater = &GameScene::FadeOut;
	}
}

void GameScene::LoadResource()
{
	auto &_stage = Stage::GetInstance();
	auto& size = Game::GetInstance().GetScreenSize();

	SetUseASyncLoadFlag(true);

	//スクリーン作成
	firstscreen		= MakeScreen(size.x,		size.y);
	secondscreen	= MakeScreen(size.x - 1,	size.y - 1);
	thirdscreen		= MakeScreen(size.x - 1,	size.y - 1);
	_4thscreen		= MakeScreen(size.x,		size.y);
	uiscreen		= MakeScreen(size.x,		size.y, true);

	//当たり範囲の指定のための領域
	for (int i = 0; i < 4; i++) {

		cutscr.top = 0 + size.y / 2 * (i / 2);
		cutscr.bottom = size.y / 2 + size.y / 2 * (i / 2);
		cutscr.right = size.x / 2 + size.x / 2 * (i % 2);
		cutscr.left = 0 + size.x / 2 * (i % 2);

		_cutAreaScreen.push_back(cutscr);
	}

	//画像の読み込み
	auto& manager = ResourceManager::GetInstance();
	sea = manager.LoadImg("../img/sea.png");
	sea_effect = manager.LoadImg("../img/sea2.png");
	guage = manager.LoadImg("../img/maru.png"); 
	beach = manager.LoadImg("../img/砂浜.png");
	gameclear = manager.LoadImg("../img/gameclear.png");
	gameover = manager.LoadImg("../img/gameover.png");

	//波のシェーダー頂点
	for (int i = 0; i < 4; i++)
	{
		wave_vertex[i].pos = VGet((i % 2)* size.x - 1, (i / 2)*size.y - 1, 0);
		wave_vertex[i].rhw = 1.0f;
		wave_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		wave_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		wave_vertex[i].u = wave_vertex[i].su = (float)(i % 2);
		wave_vertex[i].v = wave_vertex[i].sv = (float)(i / 2);
	}

	//影のシェーダー
	for (int i = 0; i < 4; i++)
	{
		shadow_vertex[i].pos = VGet((i % 2)* size.x - 1, (i / 2)*size.y - 1, 0);
		shadow_vertex[i].rhw = 1.0f;
		shadow_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		shadow_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		shadow_vertex[i].u = shadow_vertex[i].su = (float)(i % 2);
		shadow_vertex[i].v = shadow_vertex[i].sv = (float)(i / 2);
	}

	//オブジェクトの生成
	auto _stagedata = _stage.GetStageData();
	for (auto& s : _stagedata) {
		if (s.no == 1) {
			_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera, s.x, s.y));
		}
		if (s.no == 2) {
			_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera, s.x, s.y, 1));
		}
		if (s.no == 3) {
			_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera, s.x, s.y));
		}
		if (s.no == 8) {
			_goalObject.emplace_back(std::make_shared<GoalObject>(_camera, s.x, s.y));
		}
		if (s.no == 9) {
			_bosses.push_back(std::make_shared<Octopus>(_camera, _pl, Vector2(s.x, s.y)));
		}
		if (s.no == 10) {
			_bosses.push_back(std::make_shared<Crab>(_camera, _pl, Vector2(Stage::GetInstance().GetStageSize().x / 2,
																		   Stage::GetInstance().GetStageSize().y / 2)));
		}
		if (s.no == 11) {
			_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera, s.x, s.y, 2));
		}
		if (s.no == 12) {
			_enemies.push_back(std::make_shared<Fish>(_camera, _pl, Vector2(s.x, s.y)));
		}
		if (s.no == 13) {
			_enemies.push_back(std::make_shared<Diodon>(_camera,_pl, Vector2(s.x, s.y)));
		}
		if (s.no == 14) {
			_enemies.push_back(std::make_shared<SeaCucumber>(_camera,_pl, Vector2(s.x, s.y)));
		}
	}

	//score初期化
	score = ScoreInfo(0, 0, 0, 0);

	leveluiInfo.circlePos		= Vector2(0, size.y);
	leveluiInfo.circle_r		= 200;
	leveluiInfo.backCircle_r	= 250;

	SetUseASyncLoadFlag(false);

}

GameScene::GameScene(const int& stagenum)
{
	stageNum = stagenum;

	_camera.reset(new Camera());

	_pl.reset(new Player(_camera, Vector2(200, 200)));

	_col.reset(new Collision());

	flame	= 0;
	fadewait = 0;

	time		= 90;
	totaltime	= 60;

	waitNum = 3;
	waitCnt = 0;

	nlpl = nlCnt = 0;

	cutinCnt = 0;
	
	Lvimg  = ResourceManager::GetInstance().LoadImg("../img/Lv.png");
	Numimg = ResourceManager::GetInstance().LoadImg("../img/数字.png");
	cgauge = ResourceManager::GetInstance().LoadImg("../img/timegauge.png");
	shader_time = 0;

	for (int i = 0; i < 3; i++)
	{
		_Harts.push_back(std::make_unique<Hart>(Vector2(10+i*45,650),i));
	}

	_updater = &GameScene::LoadStageUpdate;
}

GameScene::~GameScene()
{
	DeleteGraph(firstscreen);
	DeleteGraph(secondscreen);
	DeleteGraph(thirdscreen);
	DeleteGraph(_4thscreen);
	DeleteGraph(uiscreen);
}

void GameScene::nlDraw()
{
	auto pl = std::make_unique<Player>(nullptr);
	auto size = Game::GetInstance().GetScreenSize();
	std::string str = "NowLoading";
	int lpsize = 100;
	pl->SelectDraw({ 600, 400 }, lpsize);
	if (nlCnt&& (nlpl == 0))
	{
		nlpl = MakeScreen(lpsize * 2, lpsize * 2, true);
		GetDrawScreenGraph(600 - lpsize, 400 - lpsize, 600 + lpsize, 400 + lpsize, nlpl);
	}
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_BACK);
	for (int i = 0;i < (nlCnt/30)%4;i++)
	{
		str += ".";
	}

	DrawString(size.x-GetFontSize()*8, size.y-GetFontSize(), str.c_str(), 0x00ffff);

	if (nlpl!=-1)DrawRotaGraph(size.x - GetFontSize() * 8 - lpsize * 1.4, size.y - lpsize, 1, (nlCnt / 30) % 360, nlpl, true);

	nlCnt++;
}

void GameScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	int mode, palam;
	GetDrawBlendMode(&mode, &palam);

	//firstスクリーン(砂浜)
	SetDrawScreen(firstscreen);

	ClearDrawScreen();

	//背景
	DrawExtendGraph(0 - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y,
		size.x - _camera->CameraCorrection().x, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x, 0 - _camera->CameraCorrection().y,
		0 - _camera->CameraCorrection().x + size.x, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x + size.x * 2, 0 - _camera->CameraCorrection().y,
		size.x - _camera->CameraCorrection().x + size.x * 2, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x * 3, 0 - _camera->CameraCorrection().y,
		0 - _camera->CameraCorrection().x + size.x * 3, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x, size.y - _camera->CameraCorrection().y + size.y,
		size.x - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x, size.y - _camera->CameraCorrection().y + size.y,
		0 - _camera->CameraCorrection().x + size.x, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x + size.x * 2, size.y - _camera->CameraCorrection().y + size.y,
		size.x - _camera->CameraCorrection().x + size.x * 2, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x * 3, size.y - _camera->CameraCorrection().y + size.y,
		0 - _camera->CameraCorrection().x + size.x * 3, 0 - _camera->CameraCorrection().y + size.y, beach, true);



	//secondスクリーン(影)
	SetDrawScreen(secondscreen);

	ClearDrawScreen();

	for (auto &enemy : _enemies)
	{
		enemy->Shadow();
	}

	for (auto &boss : _bosses)
	{
		boss->ShadowDraw();
	}

	//シェーダで使うテクスチャは先ほど作った描画可能画像
	SetUseTextureToShader(0, secondscreen);

	//ピクセルシェーダのセット
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[1]);

	DrawPrimitive2DToShader(shadow_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);




	//thirdスクリーン(波シェーダー)
	SetDrawScreen(thirdscreen);

	ClearDrawScreen();

	DrawExtendGraph(size.x, size.y, 0, 0, sea_effect, true);

	//シェーダで使うテクスチャは先ほど作った描画可能画像
	SetUseTextureToShader(0, thirdscreen);

	//シェーダーに情報を渡す
	SetPSConstSF(0, shader_time / 100.0f);

	//ピクセルシェーダのセット
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[0]);

	DrawPrimitive2DToShader(wave_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);




	//_4thスクリーン(波)
	SetDrawScreen(_4thscreen);

	ClearDrawScreen();

	DrawExtendGraph(0, 0, size.x, size.y, sea, true);

	//シェーダで使うテクスチャは先ほど作った描画可能画像
	SetUseTextureToShader(0, _4thscreen);

	//ピクセルシェーダのセット
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[0]);

	DrawPrimitive2DToShader(wave_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);




	//UI描画
	SetDrawScreen(uiscreen);

	ClearDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ADD, 150);

	DrawCircle(leveluiInfo.circlePos.x, leveluiInfo.circlePos.y, leveluiInfo.backCircle_r, 0x777777,true);

	DrawCircleGauge(leveluiInfo.circlePos.x, leveluiInfo.circlePos.y, score.bite % 5 * 5 , guage, 0.0);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawCircle(leveluiInfo.circlePos.x, leveluiInfo.circlePos.y, leveluiInfo.circle_r, 0x00cccc, true);

	auto one = totaltime % 10;
	auto ten = totaltime / 10;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(size.x / 2- GetFontSize(), 0, size.x / 2 + GetFontSize(), GetFontSize(), 0x003377,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawCircle(size.x / 2, 30, 45, 0);
	if (_updater == &GameScene::Wait && waitNum >= 1) {
		DrawRectRotaGraph(size.x / 2, size.y / 2, 300 * waitNum, 0, 300, 300, 0.5, 0, Numimg, true);
	}
	else
	{
		DrawCircleGauge(size.x / 2, 30, (gameCnt % 60)*1.6666f, cgauge, 0.0);
	}
	DrawRectRotaGraph(size.x / 2 - 30, 30, 300 * ten, 0, 300, 300, 0.3f, 0, Numimg, true);
	DrawRectRotaGraph(size.x / 2 + 30, 30, 300 * one, 0, 300, 300, 0.3f, 0, Numimg, true);

	DrawRectRotaGraph(GetFontSize()*2.5, size.y -75,300*_pl->GetInfo().level,0,300,300, abs((((gameCnt/2)%20-10)))*0.01f+0.5f,0,Numimg,true);
	DrawGraph(0, size.y - GetFontSize()*1.5,Lvimg,true);

	for (auto hart:_Harts)
	{
		hart->Draw();
	}


	//バック描画
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawGraph(0, 0, firstscreen, true);

	for (auto &immortal : _immortalObj) {
		immortal->Draw();
	}

	for (auto &destroy : _destroyObj) {
		destroy->Draw();
	}

	for (auto &predatory : _predatoryObj) {
		predatory->Draw();
	}

	for (auto &goal : _goalObject) {
		goal->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);

	DrawGraph(0, 0, secondscreen, true);

	SetDrawBlendMode(mode, palam);

	_pl->Draw();

	for (auto &enemy : _enemies)
	{
		enemy->Draw();
	}

	for (auto &boss : _bosses)
	{
		boss->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);

	DrawExtendGraph(0 - shader_offset, 0 - shader_offset, size.x + shader_offset, size.y + shader_offset, thirdscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);

	DrawExtendGraph(0 - shader_offset, 0 - shader_offset, size.x + shader_offset, size.y + shader_offset, _4thscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawGraph(0, 0, uiscreen, true);
}

void GameScene::Update(const Input & p)
{
	fadewait++,shader_time++,waitCnt++;

	totaltime = time - (flame / 60);

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);

	(*FadeBubble).Draw();
}