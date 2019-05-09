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

void GameScene::Wait(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		Game::GetInstance().ChangeScene(new ResultScene());
	}
}

GameScene::GameScene()
{
	gameimg = DxLib::LoadGraph("../img/game.png");

	_camera.reset(new Camera());

	_pl.reset(new Player(_camera));

	_col.reset(new Collision());

	_destroy.reset(new DestroyableObject(_camera));

	_predatory.reset(new PredatoryObject(_camera));

	_immortal.reset(new ImmortalObject(_camera));

	_updater = &GameScene::Wait;

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

	//オブジェクトの生成
	_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera));
	_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera));
	_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera));
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
}

void GameScene::Update(const Input & p)
{
	_pl->Update();


	for (auto itr : _enemies)
	{
		itr->Update();
	}

	for (auto itr : _enemies)
	{
		auto sVec = _pl->GetShot();
		for (int i = 0; i < _pl->GetShot().size(); ++i)
		{
			/*if (_col->WaterToSqr(_pl->GetInfo().vertexs[i], sVec[i],itr->GetInfo()._rect))
			{
				itr->ChangeColor();
				break;
			}*/
			if (_col->TriToTri(_pl->GetInfo().vertexs, itr->GetInfo()._searchVert))
			{
				itr->ChangeColor();
			}
		}
	}

	//破壊可能オブジェクト
	for (auto &destroy : _destroyObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().vertexs[i], sVec[i], destroy->GetInfo()._rect))
			{
				destroy->Break();
			}
		}
	}

	//破壊不可オブジェクト
	for (auto &predatry : _predatoryObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().vertexs[i], sVec[i], predatry->GetInfo()._rect))
			{
				predatry->Break();
			}
		}
	}

	//捕食対象
	for (auto &immortal : _immortalObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().vertexs[i], sVec[i], immortal->GetInfo()._rect))
			{
				immortal->Break();
			}
		}
	}

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);
}