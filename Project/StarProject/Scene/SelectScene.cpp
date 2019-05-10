#include "SelectScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "GameScene.h"


void SelectScene::FadeIn(const Input & p)
{
	if (flame >= 60) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &SelectScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)(flame) / 60.0f);
	Draw();
}

void SelectScene::FadeOut(const Input & p)
{
	if (flame >= 180) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BubbleDraw();
		Game::GetInstance().ChangeScene(new GameScene());
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / 180.0f);
		Draw();
		BubbleCreate();
	}
}

void SelectScene::Wait(const Input & p)
{
	Draw();

	_updater = &SelectScene::Run;
}

void SelectScene::Run(const Input & p)
{
	Draw();

	if (p.IsTrigger(PAD_INPUT_10)) {
		flame = 0;
		_updater = &SelectScene::FadeOut;
	}
}

SelectScene::SelectScene()
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

	_updater = &SelectScene::FadeIn;

	flame = 0;
}

SelectScene::~SelectScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectScene::Draw()
{
	auto& game = Game::GetInstance();
	auto size = game.GetScreenSize();

	DrawString(size.x / 2 - (float)(GetFontSize()) * 3.0f / 2.0f, size.y / 2, "Select", 0xa000f0);
	BubbleDraw();
}

void SelectScene::Update(const Input & p)
{
	flame++;
	(this->*_updater)(p);
}
