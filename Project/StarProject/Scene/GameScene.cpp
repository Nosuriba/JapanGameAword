#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"
//#include "../Object/Object.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"

void GameScene::Wait(const Input & p)
{
	_player->Update(p);
	Draw();
	for (auto itr = _enemy.begin(); itr != _enemy.end(); itr++)
	{
		(*itr)->Update(p);
		(*itr)->Draw();
	}


	if (p.IsTrigger(PAD_INPUT_1)) {
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	//Draw();
}

GameScene::GameScene()
{
	gameimg = DxLib::LoadGraph("../img/gamescene.png");
	_player.reset(new Player());

	_player->Init("../Model/Moon/moon_100.pmx",Vector3(0,0,50));
	for (int i = 0; i < 5; i++)
	{
		_enemy.push_back(std::make_shared<Enemy>());
		_enemy[i]->Init("../Model/Earth/earth_100.pmx", Vector3(i * 10, 0, 50));
	}
	_updater = &GameScene::Wait;
}

GameScene::~GameScene()
{
	DxLib::DeleteGraph(gameimg);
}

void GameScene::Draw()
{
	DxLib::DrawExtendGraph(0, 0, 1280, 740, gameimg, true);
	_player->Draw();
}

void GameScene::Update(const Input & p)
{
	(this->*_updater)(p);
}