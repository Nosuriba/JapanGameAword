#include "ResultScene.h"
#include "../Processing/Input.h"
#include "../ResourceManager.h"
#include "../Game.h"
#include "TitleScene.h"

void ResultScene::FadeIn(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &ResultScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)(flame) / WAITFRAME);
	Draw();

}

void ResultScene::FadeOut(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		(*FadeBubble).Draw();
		Game::GetInstance().ChangeScene(new TitleScene());
	}
	else {
		(*FadeBubble).Create();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / WAITFRAME);
		Draw();
	}
}

void ResultScene::Wait(const Input & p)
{
	Draw();
	_updater = &ResultScene::Run;
}

void ResultScene::Run(const Input & p)
{
	auto size = Game::GetInstance().GetScreenSize();
	ResultCnt++;
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		flame = 0;
		_updater = &ResultScene::FadeOut;
	}
}

ResultScene::ResultScene(const int& enemy, const int& bite, const int & breakobj, const float& time)
{
	_updater = &ResultScene::FadeIn;

	flame = 0;

	imgbuff = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	ResultCnt = 0;
	this->enemy = enemy;
	this->bite = bite;
	this->breakobj = breakobj;
	this->time = time;
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update(const Input & p)
{
	flame++;
	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();
	DrawExtendGraph(0, 0, size.x, size.y, imgbuff, true);
	DrawString(0, 0, "^p^", 0xff00ff);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(size.x / 2 - GetFontSize()*9, size.y / 10,
		size.x / 2 + GetFontSize() * 9, size.y /5*4, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawString(size.x / 2 - GetFontSize() * 2, 5, "Result", 0xff00ff);
	if (ResultCnt >  60)DrawFormatString(size.x / 2 - GetFontSize() * 4, size.y / 10 * 2, 0xff00ff, "Kill Enemy:%d", enemy);
	if (ResultCnt > 100)DrawFormatString(size.x / 2 - GetFontSize() * 6, size.y / 10 * 3, 0xff00ff, "Absorption Star:%d", bite);
	if (ResultCnt > 140)DrawFormatString(size.x / 2 - GetFontSize() * 5, size.y / 10 * 4, 0xff00ff, "Break Block:%d", breakobj);
	if (ResultCnt > 180)DrawFormatString(size.x / 2 - GetFontSize() * 7, size.y / 10 * 5, 0xff00ff, "Remaining Time:%f", time);
	if (ResultCnt > 220)DrawFormatString(size.x / 2 - GetFontSize() * 3, size.y / 10 * 7, 0xff00ff, "Total:%d", (enemy * 100 + bite * 100 + breakobj * 10 + time * 1000));
	(*FadeBubble).Draw();
}
