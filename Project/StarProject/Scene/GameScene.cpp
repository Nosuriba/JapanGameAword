#include "GameScene.h"
#include "../Input.h"
#include "../Game.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy.h"
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

	_ene.reset(new Enemy());

	_col.reset(new Collision());

	_updater = &GameScene::Wait;
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

	_ene->Draw();
}

void GameScene::Update(const Input & p)
{
	_pl->Update();

	_ene->Update();

	if (_col->TriToSqr(_pl->GetInfo().vertexs, _ene->GetInfo()._pos, _ene->GetInfo()._size)) {
		_ene->ChangeColor();
	}
	else {
		_ene->ResetColor();
	}

	(this->*_updater)(p);
}