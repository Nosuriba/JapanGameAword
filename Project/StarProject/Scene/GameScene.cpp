#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Fish.h"
#include "../Diodon.h"
#include "../Collision.h"
#include "../Camera.h"
#include "../Obstacle.h"

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

	_obj.reset(new Obstacle(_camera));

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

	_obj->ObjCreate(Vector2(300,300),Size(40,40));
}

GameScene::~GameScene()
{
	
}

void GameScene::Draw()
{
	int sizex, sizey;
	DxLib::GetWindowSize(&sizex, &sizey);
	//DxLib::DrawExtendGraph(0, 0, sizex, sizey, gameimg, true);

	_camera->Draw();
	_pl->Draw();

	for (auto itr : _enemies)
	{
		itr->Draw();
	}

	_obj->Draw();
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
		for (int i = 0; i < _pl->debugShot().size(); ++i)
		{
			if (_col->WatarToSqr(_pl->GetInfo().center, itr->GetInfo()._pos, _pl->debugShot()[i]))
			{
				itr->ChangeColor();
				break;
			}
			
		}
	}

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);
}