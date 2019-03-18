#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"

void GameScene::Wait(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_1)) {
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	Draw();
}

GameScene::GameScene()
{
	gameimg = DxLib::LoadGraph("../img/gamescene.png");
	_updater = &GameScene::Wait;
}

GameScene::~GameScene()
{
	DxLib::DeleteGraph(gameimg);
}

void GameScene::Draw()
{
	DxLib::DrawExtendGraph(0, 0, 1280, 740, gameimg, true);
}

void GameScene::Update(const Input & p)
{
	(this->*_updater)(p);
}