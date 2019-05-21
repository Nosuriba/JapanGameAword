#include "TitleScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "SelectScene.h"
#include <iostream>

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
		(*FadeBubble).Draw();
		Game::GetInstance().ChangeScene(new SelectScene());
	}
	else {
		(*FadeBubble).Create();
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
	if (p.Trigger(BUTTON::A) || p.IsTrigger(PAD_INPUT_10)) {
		if (!CheckHandleASyncLoad(se))
		{
			flame = 0;
			PlaySoundMem(se, DX_PLAYTYPE_BACK);
			_updater = &TitleScene::FadeOut;
		}
	}
}

TitleScene::TitleScene()
{
	title = ResourceManager::GetInstance().LoadImg("../img/海星語.png");
	titleback = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	flame = 0;
	colorflame = 0;
	blendcolor = 0;

	//フォントのロード
	LPCSTR font = "RAINYDAYS.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);

	se = ResourceManager::GetInstance().LoadSound("se_maoudamashii_effect15.mp3");

	_updater = &TitleScene::FadeIn;
}


TitleScene::~TitleScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::Update(const Input & p)
{
	flame++;
	colorflame += blendcolor * 3;
	(this->*_updater)(p);
}

void TitleScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	DxLib::DrawExtendGraph(0, 0, size.x, size.y, titleback, true);	
	DxLib::DrawRotaGraph(size.x / 2, size.y / 2 - GetFontSize(), 1, 0, title, true);
	if (colorflame >= 252) {
		blendcolor = -1;
	}
	else if (colorflame <= 0) {
		blendcolor = 1;
	}
	SetFontSize(84);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 172);
	DrawBox(0, size.y / 6*4,size.x, size.y / 6*5, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, colorflame % 255);
	DrawString((size.x - (float)(GetFontSize()) * 9) / 2.0f, size.y / 2 + GetFontSize() * 2, "PRESS A BUTTON",0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(*FadeBubble).Draw();
}
