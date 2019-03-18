#include "ResultScene.h"
#include "../Input.h"
#include "../Game.h"
#include "TitleScene.h"

void ResultScene::Wait(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_1)) {
		Game::GetInstance().ChangeScene(new TitleScene());
	}
	Draw();
}

ResultScene::ResultScene()
{
	resultimg = DxLib::LoadGraph("../img/result.png");
	_updater = &ResultScene::Wait;
}

ResultScene::~ResultScene()
{
	DxLib::DeleteGraph(resultimg);
}

void ResultScene::Update(const Input & p)
{
	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	DxLib::DrawExtendGraph(0, 0, 1280, 740, resultimg, true);
}
