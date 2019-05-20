#include "ResultScene.h"
#include "../Processing/Input.h"
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
	if (p.IsTrigger(PAD_INPUT_10)) {
		flame = 0;
		_updater = &ResultScene::FadeOut;
	}
}

void ResultScene::Run(const Input & p)
{
}

ResultScene::ResultScene(const int& enemy, const int& bite, const int & breakobj, const float& time)
{
	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	_updater = &ResultScene::FadeIn;

	flame = 0;

	info.enemy = enemy;
	info.bite = bite;
	info.breakobj = breakobj;
	info.time = time;
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
	DrawString(size.x / 2 - GetFontSize() * 2, size.y / 2, "Result", 0xff00ff);
	(*FadeBubble).Draw();
}
