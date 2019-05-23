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

const int shader_offset = 50;

void GameScene::LoadStageUpdate(const Input & p)
{
	auto &_stage = Stage::GetInstance();

	if(_stage.LoadCheck()) {
		LoadResource();
		_updater = &GameScene::LoadResourceUpdate;
	}
}

void GameScene::LoadResourceUpdate(const Input & p)
{
	int i = GetASyncLoadNum();
	if (GetASyncLoadNum() == 0)
	{
		wait = 0;
		_updater = &GameScene::FadeIn;
	}
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
	_pl->Update(p);
	for (auto &enemy : _enemies)
	{
		enemy->Update();
	}

	for (auto &boss : _bosses)
	{
		boss->Update();
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
}

void GameScene::LoadResource()
{
	auto &_stage = Stage::GetInstance();
	auto& size = Game::GetInstance().GetScreenSize();

	SetUseASyncLoadFlag(true);

	/// 敵の生成(debug用)
	_enemies.push_back(std::make_shared<Fish>(_camera));
	_enemies.push_back(std::make_shared<Diodon>(_camera));
	_enemies.push_back(std::make_shared<SeaCucumber>(_camera));
	_bosses.push_back(std::make_shared<Octopus>(_camera));

	_bosses.push_back(std::make_shared<Crab>(_camera));

	//スクリーン作成
	firstscreen = MakeScreen(size.x, size.y);
	secondscreen = MakeScreen(size.x - 1, size.y - 1);
	thirdscreen = MakeScreen(size.x - 1, size.y - 1);
	_4thscreen = MakeScreen(size.x, size.y);


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
			_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera, s.x, s.y));
		}
		if (s.no == 3) {
			_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera, s.x, s.y));
		}
	}

	//score初期化
	score = ScoreInfo(0, 0, 0, 0);

	SetUseASyncLoadFlag(false);

}

GameScene::GameScene()
{
	_camera.reset(new Camera());

	_pl.reset(new Player(_camera));

	_col.reset(new Collision());

	flame = 0;
	wait = 0;

	time = 60;
	totaltime = 60;

	waitNum = 3;
	waitCnt = 0;

	shader_time = 0;
	num = 0;
	_updater = &GameScene::LoadStageUpdate;
}

GameScene::~GameScene()
{
	if (_cutCol.joinable()) {
		_cutCol.join();
	}
	if (__eneCol.joinable()) {
		__eneCol.join();
	}
}

void GameScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	int mode, palam;
	GetDrawBlendMode(&mode, &palam);

	//firstスクリーン(砂浜)
	SetDrawScreen(firstscreen);

	ClearDrawScreen();

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

	//バック描画
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawGraph(0, 0, firstscreen, true);

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

	for (auto &destroy : _destroyObj) {
		destroy->Draw();
	}
	for (auto &predatory : _predatoryObj) {
		predatory->Draw();
	}
	for (auto &immortal : _immortalObj) {
		immortal->Draw();
	}

	auto one = totaltime % 10;
	auto ten = totaltime / 10;

	DrawFormatString(size.x / 2, GetFontSize() / 2, 0xff00ff, "%d", one);
	DrawFormatString(size.x / 2 - GetFontSize(), GetFontSize() / 2, 0xff00ff, "%d", ten);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);

	DrawExtendGraph(0 - shader_offset, 0 - shader_offset, size.x + shader_offset, size.y + shader_offset, thirdscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);

	DrawExtendGraph(0 - shader_offset, 0 - shader_offset, size.x + shader_offset, size.y + shader_offset, _4thscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(128);

	if (_updater == &GameScene::Wait && waitNum >= 1) {
		DrawFormatString(size.x / 2 - GetFontSize() / 2, size.y / 2 - GetFontSize() / 2, 0xff00ff, "%d", waitNum);
	}

	SetFontSize(64);
}

void GameScene::Update(const Input & p)
{
	wait++; shader_time++; waitCnt++;

	auto size = Game::GetInstance().GetScreenSize();

	const auto laser = _pl->GetLaser(); 
	const auto camera = _camera->CameraCorrection();

	auto _eth = [&](std::list<Laser> _laser) {
		std::lock_guard<std::mutex> _lock(_mutex);

		for (int i = 0; i < _enemies.size(); ++i)
		{
			/// 敵の死亡処理
			if (_enemies[i]->GetInfo()._dieFlag)
			{
				_enemies.erase(_enemies.begin() + i);
				continue;
			}
;
			/// ﾌﾟﾚｲﾔｰｼｮｯﾄと敵の当たり判定
			//for (int p = 0; p < _pl->GetLaser().size(); ++p)
			for (auto& l : _laser)
			{
				if (_col->WaterToSqr(l.pos, l.vel, _enemies[i]->GetInfo()._rect))
				{
					auto vec = _enemies[i]->GetInfo()._pos - l.pos;
					vec.Normalize();

					_enemies[i]->CalEscapeDir(vec);
					break;
				}
				if (_col->CircleToCircle(_pl->GetInfo().center, _pl->GetInfo().r, _enemies[i]->GetInfo()._searchVert))
				{
					_enemies[i]->CalTrackVel(_pl->GetInfo().center);
				}
			
			}

			/// ﾌﾟﾚｲﾔｰと敵ｼｮｯﾄの当たり判定
			for (int s = 0; s < _enemies[i]->GetShotInfo().size(); ++s)
			{
				if (_col->TriToSqr(_pl->GetInfo().legs, _enemies[i]->GetShotInfo()[s]._pos, _enemies[i]->GetShotInfo()[s]._size))
				{
					_enemies[i]->ShotDelete(s);		/// ﾌﾟﾚｲﾔｰに当たった弾の色を変えている。
				}
			}
		}

		if (_bosses.size() != 0) {
			_bosses[0]->CalTrackVel(_pl->GetInfo().center);
		}

	auto th = [&](std::list<Laser> _laser, Vector2 camera) {
		std::lock_guard<std::mutex> _lock(_mutex);
		num++;
		for (auto &l : _laser) {

			//破壊可能オブジェクト
			for (auto destroy : _destroyObj) {

				if (destroy->GetInfo()._pos.x - camera.x <= size.x &&
					destroy->GetInfo()._pos.y - camera.y <= size.y) {

					if (_cutAreaScreen[num % 4].left <= destroy->GetInfo()._pos.x - camera.x &&

						destroy->GetInfo()._pos.x - camera.x <= _cutAreaScreen[num % 4].right &&

						_cutAreaScreen[num % 4].top <= destroy->GetInfo()._pos.y - camera.y &&

						destroy->GetInfo()._pos.y - camera.y <= _cutAreaScreen[num % 4].bottom) {

							if (_col->WaterToSqr(l.pos, l.vel, l.size, destroy->GetInfo()._rect))
							{
								destroy->Break();
								//l.Hit();
							}
					}
					/*if (_col->TriToSqr(_pl->GetInfo().legs, destroy->GetInfo()._pos, destroy->GetInfo()._size)) {

					}*/
				}

			}

			//捕食対象
			for (auto predatry : _predatoryObj) {

				if (predatry->GetInfo()._pos.x - _camera->CameraCorrection().x <= size.x &&
					predatry->GetInfo()._pos.y - _camera->CameraCorrection().y <= size.y) {

					if (_cutAreaScreen[num % 4].left <= predatry->GetInfo()._pos.x - camera.x &&

						predatry->GetInfo()._pos.x - camera.x <= _cutAreaScreen[num % 4].right &&

						_cutAreaScreen[num % 4].top <= predatry->GetInfo()._pos.y - camera.y &&

						predatry->GetInfo()._pos.y - camera.y <= _cutAreaScreen[num % 4].bottom) {

						if (_col->WaterToSqr(l.pos, l.vel, l.size, predatry->GetInfo()._rect))
						{
							predatry->Break();
						}

						if (_col->CircleToSqr(_pl->GetInfo().center, _pl->GetInfo().r, predatry->GetInfo()._rect)) {

							if (_col->TriToSqr(_pl->GetInfo().legs, predatry->GetInfo()._pos, predatry->GetInfo()._size))
							{
								predatry->Predatory();
							}

						}
					}
				}
			}

			//破壊不可オブジェクト
			for (auto immortal : _immortalObj) {

				if (immortal->GetInfo()._pos.x - _camera->CameraCorrection().x <= size.x &&
					immortal->GetInfo()._pos.y - _camera->CameraCorrection().y <= size.y) {

					if (_cutAreaScreen[num % 4].left <= immortal->GetInfo()._pos.x - camera.x &&

						immortal->GetInfo()._pos.x - camera.x <= _cutAreaScreen[num % 4].right&&

						_cutAreaScreen[num % 4].top <= immortal->GetInfo()._pos.y - camera.y &&

						immortal->GetInfo()._pos.y - camera.y <= _cutAreaScreen[num % 4].bottom) {

						if (_col->WaterToSqr(l.pos, l.vel, l.size, immortal->GetInfo()._rect))
						{
							immortal->Break();
						}
						/*if (_col->TriToSqr(_pl->GetInfo().legs, immortal->GetInfo()._pos, immortal->GetInfo()._size)) {

						}*/
					}

				}
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

			if (_predatoryObj[i]->GetInfo()._predatoryflag)
			{
				_predatoryObj.erase(_predatoryObj.begin() + i);
				continue;
			}
		}
	};
	
	if (!_cutCol.joinable()) {
		
		_cutCol = std::thread(th, std::ref(laser), std::ref(camera));
	}
	else {
		_cutCol.join();
	}

	if (!__eneCol.joinable()) {

		__eneCol = std::thread(_eth, std::ref(laser));
	}
	else {
		__eneCol.join();
	}

	totaltime = time - (flame / 60);

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);

	(*FadeBubble).Draw();
}