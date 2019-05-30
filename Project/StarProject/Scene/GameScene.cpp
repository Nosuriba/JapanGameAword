#include "GameScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "ResultScene.h"
#include "../Player.h"
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
#include "../Stage.h"


#include <iostream>
#include <algorithm>

#define CC _camera->CameraCorrection()

const int shader_offset = 50;

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
		wait = 0;
		_updater = &GameScene::FadeIn;
	}
	nlDraw();
}

void GameScene::FadeIn(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 - 255 * (float)(wait) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (wait >= WAITFRAME) {
		waitCnt = 0;
		_updater = &GameScene::Wait;
	}
}

void GameScene::FadeOut(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * (float)(wait) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (wait >= WAITFRAME) {
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
	else {
		if ((waitCnt % 60) == 0) {
			waitNum--;
		}
	}
}

void GameScene::Run(const Input & p)
{
	auto& size = Game::GetInstance().GetScreenSize();

	auto& laser = _pl->GetLaser();

	_pl->Update(p);

	for (auto &enemy : _enemies)
	{
		enemy->Update();
	}

	for (auto &boss : _bosses)
	{
		boss->Update();
	}

	num++;
/*
	if (num % 4 == 0) {*/
		for (int i = 0; i < _enemies.size(); ++i)
		{
			/// 敵の死亡処理
			if (!_enemies[i]->GetInfo()._isAlive)
			{
				_enemies.erase(_enemies.begin() + i);
				continue;
			}

			// プレイヤーと敵
			if ((_pl->GetInfo().center - _enemies[i]->GetInfo()._pos).Magnitude() < _pl->GetInfo().r + _enemies[i]->GetInfo()._size.width / 2)
			{
				if (_enemies[i]->GetInfo()._isAlive)
					_pl->OnDamage();
			}

			/// ﾌﾟﾚｲﾔｰｼｮｯﾄと敵の当たり判定
			//for (int p = 0; p < _pl->GetLaser().size(); ++p)
			auto _p = _enemies[i]->GetInfo()._pos - CC;
			if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;
			for (int j = 0; j < 2; j++)
			{
				for (auto l = laser[j].begin(); l != laser[j].end(); l++)
				{
					if (_col->WaterToCircle(
						(*l).pos, (*l).isEnd ? (*l).pos : ((++l != laser[j].end()) ? (*l--).pos : (*--l).pos),
						_enemies[i]->GetInfo()._pos, _enemies[i]->GetInfo()._size.height))
					{
						_enemies[i]->OnDamage();
					}
				}
			}
		}

		for (auto boss : _bosses) {
			//ﾌﾟﾚｲﾔｰｼｮｯﾄとボス
			for (auto b : boss->GetDamageInfo()) {
				auto _p = b._pos - CC;
				if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;
				for (int j = 0; j < 2; ++j) {
					for (auto l = laser[j].begin(); l != laser[j].end(); l++) {
						if (_col->WaterToCircle(
							(*l).pos, (*l).isEnd ? (*l).pos : ((++l != laser[j].end()) ? (*l--).pos : (*--l).pos),
							b._pos, b._r))
						{
							boss->OnDamage();
						}
					}
				}
			}
			//ﾌﾟﾚｲﾔｰとボス
			for (auto b : boss->GetAttackInfo()) {
				auto _p = b._pos - CC;
				if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;
				if (_col->CircleToCircleBoss(_pl->GetInfo().center, _pl->GetInfo().r, b._pos, b._r)) {
					_pl->OnDamage();
				}
			}
		}
	//}
	

	for (int i = 0; i < 2; i++)
	{
		for (auto l = laser[i].begin(); l != laser[i].end(); l++) {

			//if (num % 4 == 1) {
				//破壊可能オブジェクト
				for (auto destroy : _destroyObj) {

					auto _p = destroy->GetInfo()._pos - CC;
					if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

					if (_cutAreaScreen[num % 4].left <= destroy->GetInfo()._pos.x - CC.x &&

						destroy->GetInfo()._pos.x - CC.x <= _cutAreaScreen[num % 4].right &&

						_cutAreaScreen[num % 4].top <= destroy->GetInfo()._pos.y - CC.y &&

						destroy->GetInfo()._pos.y - CC.y <= _cutAreaScreen[num % 4].bottom) {

						auto e = (*l).isEnd ? (*l).pos : ((++l != laser[i].end()) ? (*l--).pos : (*--l).pos);

						if (_pl->GetInfo().level >= destroy->GetInfo()._level && !destroy->GetInfo()._hitflag) {
							if (_col->WaterToSqr((*l).pos,
								e, (e - (*l).pos).Magnitude(),
								destroy->GetInfo()._rect))
							{
								destroy->Break();
								//(*l).Hit();
							}
						}
					}
				}
			//}
			

			//if (num % 4 == 2) {
				//捕食対象
				for (auto predatry : _predatoryObj) {

					auto _p = predatry->GetInfo()._pos - CC;
					if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

					if (_cutAreaScreen[num % 4].left <= predatry->GetInfo()._pos.x - CC.x &&

						predatry->GetInfo()._pos.x - CC.x <= _cutAreaScreen[num % 4].right &&

						_cutAreaScreen[num % 4].top <= predatry->GetInfo()._pos.y - CC.y &&

						predatry->GetInfo()._pos.y - CC.y <= _cutAreaScreen[num % 4].bottom) {

						auto e = (*l).isEnd ? (*l).pos : ((++l != laser[i].end()) ? (*l--).pos : (*--l).pos);
						if (_col->WaterToSqr((*l).pos,
							e, (e - (*l).pos).Magnitude(),
							predatry->GetInfo()._rect))
						{
							predatry->Break();
						}

						if (_col->CircleToSqr(_pl->GetInfo().center, _pl->GetInfo().r, predatry->GetInfo()._rect)) {

							if (_col->TriToSqr(_pl->GetInfo().legs, predatry->GetInfo()._pos, predatry->GetInfo()._size))
							{
								_pl->ToCatch(predatry->GetInfo()._pos);
								predatry->Predatory();
								score.bite++;
								//if (score.bite % 5 == 1) {
								//	_pl->LevelUP();
								//}
							}

						}
					}
				}
			//}


			//if (num % 4 == 3) {
				//破壊不可オブジェクト
				for (auto immortal : _immortalObj) {

					auto _p = immortal->GetInfo()._pos - CC;
					if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

					//if (_cutAreaScreen[num % 4].left <= immortal->GetInfo()._pos.x - CC.x &&

					//	immortal->GetInfo()._pos.x - CC.x <= _cutAreaScreen[num % 4].right&&

					//	_cutAreaScreen[num % 4].top <= immortal->GetInfo()._pos.y - CC.y &&

					//	immortal->GetInfo()._pos.y - CC.y <= _cutAreaScreen[num % 4].bottom) {

					//	auto e = (*l).isEnd ? (*l).pos : ((++l != laser[i].end()) ? (*l--).pos : (*--l).pos);
					//		
					//}
				}
			//}
		}
	}

	for (auto destroy : _destroyObj) {
		auto _p = destroy->GetInfo()._pos - CC;
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		if (_col->CircleToSqr(_pl->GetInfo().center, _pl->GetInfo().r, destroy->GetInfo()._rect)) {
			_pl->SetStar(_col->Pushback(_pl->GetInfo(), destroy->GetInfo()._rect), _pl->GetInfo().r);
		}
	}
	for (auto immortal : _immortalObj) {
		auto _p = immortal->GetInfo()._pos - CC;
		if (_p.x < 0 || _p.x > size.x || _p.y < 0 || _p.y > size.y) continue;

		if (_col->CircleToSqr(_pl->GetInfo().center, _pl->GetInfo().r, immortal->GetInfo()._rect)) {
			_pl->SetStar(_col->Pushback(_pl->GetInfo(), immortal->GetInfo()._rect), _pl->GetInfo().r);
		}
	}

	for (int i = 0; i < _destroyObj.size(); i++) {
		if (_destroyObj[i]->GetInfo()._breakflag)
		{
			_destroyObj.erase(_destroyObj.begin() + i);
			continue;
		}
	}

	for (int i = 0; i < _predatoryObj.size(); i++) {
		if (_predatoryObj[i]->GetInfo()._breakflag)
		{
			_predatoryObj.erase(_predatoryObj.begin() + i);
			continue;
		}

		if (_predatoryObj[i]->GetInfo()._hitflag)
		{
			_predatoryObj.erase(_predatoryObj.begin() + i);
			continue;
		}
	}

	Draw();

	flame++;

	if (p.Trigger(BUTTON::A) || p.IsTrigger(PAD_INPUT_10)) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
	else if (totaltime == 0) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
	if (_pl->CheckDie())
	{
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
}

void GameScene::BossScene(const Input & p)
{
	auto& size = Game::GetInstance().GetScreenSize();

	Draw();
	_pl->Update(p);

	if ((_pl->GetInfo().center.x >= size.x * 3 + _pl->GetInfo().r * 3) &&
		(_pl->GetInfo().center.y >= size.y)) {
		bosssceneflag = true;
		StageLock();
		_updater = &GameScene::Run;
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

	leveluiInfo.circlePos = Vector2(0, size.y);
	leveluiInfo.circle_r = 200;
	leveluiInfo.backCircle_r = 250;

	SetUseASyncLoadFlag(false);

}

void GameScene::StageLock()
{
	auto& size = Game::GetInstance().GetScreenSize();

	for (int i = 0; i < _camera->GetRange().y / 32; i++) {
		_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera, size.x * 3, i * 32 + 16));
	}
}

GameScene::GameScene(const int& stagenum)
{
	stageNum = stagenum;

	_camera.reset(new Camera());

	_pl.reset(new Player(_camera, Vector2(200, 200)));

	_col.reset(new Collision());

	flame = 0;
	wait = 0;

	time = 60;
	totaltime = 60;

	waitNum = 3;
	waitCnt = 0;

	nlpl = nlCnt = 0;
	
	shader_time = 0;
	num = 0;

	bosssceneflag = false;

	_updater = &GameScene::LoadStageUpdate;
}

GameScene::~GameScene()
{
	/*if (_cutCol.joinable()) {
		_cutCol.join();
	}
	if (__eneCol.joinable()) {
		__eneCol.join();
	}*/

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
	if (nlpl==0)
	{
		pl->SelectDraw({ 600, 400 }, lpsize);
	}
	else if (nlpl!=-1)
	{		
		DrawRotaGraph(size.x - GetFontSize() * 8 - lpsize * 1.5, size.y - lpsize, 1, nlCnt / 10, nlpl, true);
	}
	nlCnt++;
	(*FadeBubble).Draw();
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

	for (auto itr : _enemies)
	{
		itr->Draw();
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

	DrawFormatString(size.x / 2, GetFontSize() / 2, 0xff00ff, "%d", one);
	DrawFormatString(size.x / 2 - GetFontSize(), GetFontSize() / 2, 0xff00ff, "%d", ten);

	SetFontSize(128);

	if (_updater == &GameScene::Wait && waitNum >= 1) {
		DrawFormatString(size.x / 2 - GetFontSize() / 2, size.y / 2 - GetFontSize() / 2, 0xff00ff, "%d", waitNum);
	}

	DrawFormatString(GetFontSize() / 2 + GetFontSize() / 4, size.y - GetFontSize(), 0xff8000, "%d", _pl->GetInfo().level);

	SetFontSize(64);

	DrawString(GetFontSize() / 6, size.y - GetFontSize() - 5, "Lv ", 0xff8000);




	//バック描画
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawGraph(0, 0, firstscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);

	DrawGraph(0, 0, secondscreen, true);

	SetDrawBlendMode(mode, palam);

	_pl->Draw();

	for (auto &immortal : _immortalObj) {
		immortal->Draw();
	}

	for (auto &destroy : _destroyObj) {
		destroy->Draw();
	}

	for (auto &predatory : _predatoryObj) {
		predatory->Draw();
	}

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
	wait++; shader_time++; waitCnt++;

	auto size = Game::GetInstance().GetScreenSize();

	auto& laser = _pl->GetLaser(); 
	//const auto camera = _camera->CameraCorrection();


	//auto th = [&](std::array<std::list<Laser>,2> _laser, Vector2 camera) {
		
	//};

	totaltime/* = time - (flame / 60)*/;

	_camera->Update(_pl->GetInfo().center);

	for (auto predatry : _predatoryObj) {
		predatry->Update();
	}
	for (auto destroy : _destroyObj) {
		destroy->Update(p);
	}

	(this->*_updater)(p);

	(*FadeBubble).Draw();
}