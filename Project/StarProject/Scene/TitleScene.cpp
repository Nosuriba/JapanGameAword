#include "TitleScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "SelectScene.h"

void TitleScene::FadeIn(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &TitleScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)(flame) / WAITFRAME);
	Draw();
}

void TitleScene::FadeOut(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BubbleDraw();
		Game::GetInstance().ChangeScene(new SelectScene());
	}
	else {
		BubbleCreate();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / WAITFRAME);
		Draw();
	}
}

void TitleScene::Wait(const Input & p)
{
	Draw();
	_updater = &TitleScene::Run;
}

void TitleScene::Run(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		flame = 0;
		_updater = &TitleScene::FadeOut;
	}
}

TitleScene::TitleScene()
{
	titleimg = DxLib::LoadGraph("../img/title.png");
	flame = 0;

	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	_updater = &TitleScene::FadeIn;
}


TitleScene::~TitleScene()
{
	DxLib::DeleteGraph(titleimg);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::Update(const Input & p)
{
	flame++;
	(this->*_updater)(p);
}

void TitleScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	DxLib::DrawExtendGraph(0, 0, 1280, 740, titleimg, true);

	DrawString(size.x / 2 - (float)(GetFontSize()) * 4.0f / 2.0f, size.y / 2 + GetFontSize() * 3, "START",0xa000f0);
	BubbleDraw();
}
