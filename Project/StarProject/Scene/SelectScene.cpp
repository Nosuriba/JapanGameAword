#include "SelectScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "GameScene.h"
#include "../ResourceManager.h"


void SelectScene::FadeIn(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &SelectScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)(flame) / WAITFRAME);
	Draw();
}

void SelectScene::FadeOut(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		(*FadeBubble).Draw();
		Game::GetInstance().ChangeScene(new GameScene());
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / WAITFRAME);
		Draw();
		(*FadeBubble).Create();
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
	auto size = Game::GetInstance().GetScreenSize();

	//�t�H���g�̃��[�h
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "���s", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	bubble = ResourceManager::GetInstance().LoadImg("../img/Bubble.png");
	background = ResourceManager::GetInstance().LoadImg("../img/selectback.png");

	//�A�̃V�F�[�_�[���_
	int bubblesizex,bubblesizey;
	GetGraphSize(bubble, &bubblesizex, &bubblesizey);
	for (int i = 0; i < 4; i++)
	{
		bubble_vertex[i].pos = VGet((i % 2)* bubblesizex, (i / 2)*bubblesizey, 0);
		bubble_vertex[i].rhw = 1.0f;
		bubble_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		bubble_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		bubble_vertex[i].u = bubble_vertex[i].su = (float)(i % 2);
		bubble_vertex[i].v = bubble_vertex[i].sv = (float)(i / 2);
	}

	firstscreen = MakeScreen(size.x, size.y);
	secondscreen = MakeScreen(size.x, size.y);
	_updater = &SelectScene::FadeIn;
	shader_time = 0;
	flame = 0;
	Cnt = 0;
}

SelectScene::~SelectScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectScene::Draw()
{
	auto& game = Game::GetInstance();
	auto size = game.GetScreenSize();


	//first(�w�i)
	SetDrawScreen(firstscreen);

	ClearDrawScreen();



	//second(�A)
	SetUseTextureToShader(0, bubble);

	SetPSConstSF(0, shader_time / 10);

	SetUsePixelShader(game.GetShaderHandle()[2]);

	DrawPrimitive2DToShader(bubble_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);


	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	//DrawGraph(0,0,firstscreen,true);

	DrawExtendGraph(0, 0, size.x, size.y, background, true);

	(*BuckBubble).Draw();
	auto addx = cos((Cnt % 360)*DX_PI / 180) * 25;
	auto addy = sin((Cnt)*DX_PI / 720) * 100;
	auto addr = sin((Cnt)*DX_PI / 180) * 0.1;
	DrawRotaGraph(size.x / 4 - 100+ addx, size.y / 2 + addy, 1+ addr, 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 2 + addx, size.y / 2 - addy, 1 - addr, 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 3 + 100 + addx, size.y / 2 + addy, 1 + addr, 0, bubble, true);

	

	DrawString(size.x / 2 - (float)(GetFontSize()) * 3.0f / 2.0f, size.y / 2 + size.y / 4, "Select", 0xa000f0);

	(*FadeBubble).Draw();
}

void SelectScene::Update(const Input & p)
{
	(*BuckBubble).Create();
	flame++;
	Cnt++;
	shader_time++;
	(this->*_updater)(p);
}
