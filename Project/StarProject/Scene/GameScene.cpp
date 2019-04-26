#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Fish.h"
#include "../Diodon.h"
#include "../Collision.h"

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

	_pl.reset(new Player());

	_col.reset(new Collision());

	_updater = &GameScene::Wait;

	for (int i = 0; i < 2; ++i)
	{
		if (i <= 0)
		{
			_enemies.push_back(std::make_shared<Fish>());
		}
		else
		{
			_enemies.push_back(std::make_shared<Diodon>());
		}
	}
}

GameScene::~GameScene()
{
	
}

void GameScene::Draw()
{
	int sizex, sizey;
	DxLib::GetWindowSize(&sizex, &sizey);
	//DxLib::DrawExtendGraph(0, 0, sizex, sizey, gameimg, true);

	_pl->Draw();

	for (auto itr : _enemies)
	{
		itr->Draw();
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
		if (_col->TriToSqr(_pl->GetInfo().vertexs, itr->GetInfo()._pos, itr->GetInfo()._size)) {
			itr->ChangeColor();
		}
		else {
			itr->ResetColor();
		}
	}

	(this->*_updater)(p);
}