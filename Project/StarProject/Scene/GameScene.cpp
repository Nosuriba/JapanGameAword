#include "GameScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Fish.h"
#include "../Enemy/Diodon.h"
#include "../Processing/Collision.h"
#include "../Camera.h"
#include "../Object/Obstacle.h"
#include "../Object/DestroyableObject.h"
#include "../Object/PredatoryObject.h"
#include "../Object/ImmortalObject.h"

void GameScene::FadeIn(const Input & p)
{
	if (wait >= 60) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &GameScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)wait / 60.0f);
	Draw();
}

void GameScene::FadeOut(const Input & p)
{
	if (wait >= 180) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BubbleDraw();
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	else {
		BubbleCreate();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)wait / 180.0f);
		Draw();
	}
}

void GameScene::Wait(const Input & p)
{
	Draw();
	_updater = &GameScene::Run;
}

void GameScene::Run(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
	else if (totaltime == 0) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
}

GameScene::GameScene()
{
	_camera.reset(new Camera());

	_pl.reset(new Player(_camera));

	_col.reset(new Collision());

	_destroy.reset(new DestroyableObject(_camera));

	_predatory.reset(new PredatoryObject(_camera));

	_immortal.reset(new ImmortalObject(_camera));

	_updater = &GameScene::FadeIn;

	for (int i = 0; i < 2; ++i)
	{
		if (i <= 0)
		{
			_enemies.push_back(std::make_shared<Fish>(_camera));
		}
		else
		{
			_enemies.push_back(std::make_shared<Diodon>(_camera));
		}
	}

	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	//オブジェクトの生成
	_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera));
	_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera));
	_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera));

	flame = 0;
	wait = 0;
	time = 60;
	totaltime = 60;
}

GameScene::~GameScene()
{
	
}

void GameScene::Draw()
{
	int sizex, sizey;
	DxLib::GetWindowSize(&sizex, &sizey);

	_camera->Draw();
	_pl->Draw();

	for (auto itr : _enemies)
	{
		itr->Draw();
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

	DrawFormatString(sizex / 2, GetFontSize() / 2, 0xff00ff, "%d", one);
	DrawFormatString(sizex / 2 - GetFontSize(), GetFontSize() / 2, 0xff00ff, "%d", ten);
	BubbleDraw();
}

void GameScene::Update(const Input & p)
{
	flame++;
	wait++;

	_pl->Update(p);

	for (auto itr : _enemies)
	{
		itr->Update();
	}

	for (int i = 0; i < _enemies.size(); ++i)
	{
		/// 敵の死亡処理
		if (_enemies[i]->GetInfo()._dieFlag)
		{
			_enemies.erase(_enemies.begin() + i);
			continue;
		}

		auto sVec = _pl->GetShot();
		/// ﾌﾟﾚｲﾔｰｼｮｯﾄと敵の当たり判定
		for (int p = 0; p < _pl->GetShot().size(); ++p)
		{
			if (_col->WaterToSqr(_pl->GetInfo().legs[p].tip, sVec[p],_enemies[i]->GetInfo()._rect))
			{
				auto vec = _enemies[i]->GetInfo()._pos - _pl->GetInfo().legs[p].tip;
				vec.Normalize();

				_enemies[i]->CalEscapeDir(vec);
				break;
			}
			_enemies[i]->CalTrackVel(_pl->GetInfo().center, _col->TriToTri(_pl->GetInfo().legs, _enemies[i]->GetInfo()._searchVert));
		}
		
		/// ﾌﾟﾚｲﾔｰと敵ｼｮｯﾄの当たり判定
		for (int s = 0; s < _enemies[i]->GetShotInfo().size(); ++s)
		{
			if (_col->TriToSqr(_pl->GetInfo().legs, _enemies[i]->GetShotInfo()[s]._pos, _enemies[i]->GetShotInfo()[s]._size))
			{
				_enemies[i]->ChangeShotColor(s);		/// ﾌﾟﾚｲﾔｰに当たった弾の色を変えている。
			}
		}
	}

	//破壊可能オブジェクト
	for (auto &destroy : _destroyObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], destroy->GetInfo()._rect))
			{
				destroy->Break();
			}
		}
	}

	//破壊不可オブジェクト
	for (auto &predatry : _predatoryObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], predatry->GetInfo()._rect))
			{
				predatry->Break();
			}
		}
	}

	//捕食対象
	for (auto &immortal : _immortalObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], immortal->GetInfo()._rect))
			{
				immortal->Break();
			}
		}
	}

	if (_enemies.size() <= 0)
	{
		DrawString(0, 0, "Not Enemy", 0xffffff);
	}

	totaltime = time - (flame / 60);

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);
}