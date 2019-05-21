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

ResultScene::ResultScene(const int& enemy, const int& bite, const int & breakobj, const int& time)
{
	_updater = &ResultScene::FadeIn;

	flame = 0;

	//フォントのロード
	LPCSTR font = "CP_Revenge.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	
	ResultStr = { "倒した敵の数　:%6d" ,"捕食した数　　:%6d" , "壊したブロック:%6d","残りタイム　　:%6d","","　　総合点　　:%6d", };

	imgbuff = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	ResultCnt = 0;

	ResultData[(int)R_Data::enemy]		= enemy		*100;
	ResultData[(int)R_Data::bite]		= bite		*100;
	ResultData[(int)R_Data::breakobj]	= breakobj	*10;
	ResultData[(int)R_Data::time]		= time		*1000;
	ResultData[(int)R_Data::blank]		= 0;
	for (int i = 0;i< (int)R_Data::total;i++)
	{
		ResultData[(int)R_Data::total] += ResultData[i];
	}

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
	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(size.x / 2 - GetFontSize()*9, size.y / 10,
		size.x / 2 + GetFontSize() * 9, size.y /5*4, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	DrawString(size.x / 2 - GetFontSize() * 2, 5, "リザルト", 0xff8c00);

	int Cnt = 0;
	auto ScoreData = 0;
	for (auto str: ResultStr)
	{
		if (ResultCnt > Cnt * 40)
		{
			DrawFormatString(size.x / 2 - GetFontSize() * 6, size.y / 10 * (2 + Cnt), 0xff8c00, str.c_str(), ResultData[Cnt]);
			Cnt++;
		}
	}

	(*FadeBubble).Draw();

	ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);
}
