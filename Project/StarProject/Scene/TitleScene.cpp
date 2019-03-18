#include "TitleScene.h"
#include "../Input.h"
#include "../Game.h"
#include "GameScene.h"

void TitleScene::Wait(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_1)) {
		Game::GetInstance().ChangeScene(new GameScene());
	}
	Draw();
}

TitleScene::TitleScene()
{
	titleimg = DxLib::LoadGraph("../img/title.png");
	_updater = &TitleScene::Wait;
}


TitleScene::~TitleScene()
{
	DxLib::DeleteGraph(titleimg);
}

void TitleScene::Update(const Input & p)
{
	(this->*_updater)(p);
}

void TitleScene::Draw()
{
	DxLib::DrawExtendGraph(0, 0, 1280, 740, titleimg, true);
}
