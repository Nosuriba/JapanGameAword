#include "TitleScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "SelectScene.h"
#include <iostream>

void TitleScene::FadeIn(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 - 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame >= WAITFRAME) {
		_updater = &TitleScene::Wait;
	}
}

void TitleScene::FadeOut(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame > WAITFRAME) {
		Game::GetInstance().ChangeScene(new SelectScene());
	}
	else {
		(*FadeBubble).Create();
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

	if (p.Trigger(BUTTON::A)) {
		if (!CheckHandleASyncLoad(se))
		{
			flame = 0;
			PlaySoundMem(se, DX_PLAYTYPE_BACK);
			_updater = &TitleScene::FadeOut;
		}
	}
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_SPACE)) 
	{
		if (!CheckHandleASyncLoad(se))
		{
			flame = 0;
			PlaySoundMem(se, DX_PLAYTYPE_BACK);
			_updater = &TitleScene::FadeOut;
		}
	}
#endif // _DEBUG
}

TitleScene::TitleScene()
{
	title = ResourceManager::GetInstance().LoadImg("../img/title.png");
	titleback = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	BGM = ResourceManager::GetInstance().LoadSound("System/title.mp3");
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
	if (CheckSoundMem(BGM)) {
		StopSoundMem(BGM);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::Update(const Input & p)
{
	flame++;
	colorflame += blendcolor * 3;
	if (!CheckSoundMem(BGM)) {
		PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
	}
	(this->*_updater)(p);

	(*FadeBubble).Draw();
}

void TitleScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	int mode, palam,titlex, titley;

	GetGraphSize(title, &titlex, &titley);
	GetDrawBlendMode(&mode, &palam);

	DxLib::DrawExtendGraph(0, 0, size.x, size.y, titleback, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 172);
	//DrawBox((size.x- titlex)/2, (size.y + titley) / 2 - titley - titley / 2, (size.x+ titlex)/2, (size.y - titley) - titley / 8, 0x003377, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DxLib::DrawRotaGraph((size.x) / 2 - 35, (size.y) / 2 - titley /2, 1 , 0, title, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 172);
	if (colorflame >= 252) {
		blendcolor = -1;
	}
	else if (colorflame <= 0) {
		blendcolor = 1;
	}
	SetFontSize(84);
	DrawBox(0, size.y / 6*4,size.x, size.y / 6*5, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (colorflame % 255));
	DrawString((size.x - (float)(GetFontSize()) * 9) / 2.0f, size.y / 2 + GetFontSize() * 2, "PRESS A BUTTON",0xffffff);
	DrawString((size.x - (float)(GetFontSize()) ) / 2.0f, size.y / 2 + GetFontSize() * 2, "A", 0x30ff30);
	SetDrawBlendMode(mode, palam);
}
