#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"
//#include "../Object/Object.h"
#include "../Object/Player.h"

void GameScene::Wait(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_1)) {
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	//Draw();
}

GameScene::GameScene()
{
	gameimg = DxLib::LoadGraph("../img/gamescene.png");

	_obj.reset(new Player());

	_obj->Init("../Model/Earth/earth_100.pmx",Vector3(0,0,0));

	_updater = &GameScene::Wait;
}

GameScene::~GameScene()
{
	DxLib::DeleteGraph(gameimg);
}

void GameScene::Draw()
{
	DxLib::DrawExtendGraph(0, 0, 1280, 740, gameimg, true);
	_obj->Update();
	_obj->Draw();
}

void GameScene::Update(const Input & p)
{
	(this->*_updater)(p);
}