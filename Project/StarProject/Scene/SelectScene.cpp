#include "SelectScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "GameScene.h"
#include "../ResourceManager.h"
#include "../Particle/Bubble.h"


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
	if (p.TriggerTrigger(TRIGGER::RIGHT)) {
		Select = Select<2?++Select:Select;
	}

	if (p.TriggerTrigger(TRIGGER::LEFT)) {
		Select = Select>0?--Select:Select;
	}
}

SelectScene::SelectScene()
{
	auto size = Game::GetInstance().GetScreenSize();

	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	bubble = ResourceManager::GetInstance().LoadImg("../img/Bubble.png");
	background = ResourceManager::GetInstance().LoadImg("../img/selectback.png");

	//泡のシェーダー頂点
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
	Select = 0; 

	CoralBubble.push_back(std::make_unique<Bubble>(size.x /5*2, size.y / 5 * 2,150,true,1));
	CoralBubble.push_back(std::make_unique<Bubble>(50, size.y / 5 * 4, 100, true, 1));
	CoralBubble.push_back(std::make_unique<Bubble>(size.x / 5 * 4, size.y / 5 * 4, 125, true, 1));
}

SelectScene::~SelectScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectScene::Draw()
{
	auto& game = Game::GetInstance();
	auto size = game.GetScreenSize();

	DrawExtendGraph(0, 0, size.x, size.y, background, true);

	for (auto &b:CoralBubble)
	{
		(*b).Draw();
	}

	auto addx = cos((Cnt % 360)*DX_PI / 180) * 25;
	auto addy = sin((Cnt)*DX_PI / 720) * 100;
	auto addr = sin((Cnt)*DX_PI / 180) * 0.1;

	DrawRotaGraph(size.x / 4 - 100 + ((Select != 0) ? addx : 0), size.y / 2 + ((Select != 0) ? addy : 0), 1 + ((Select != 0) ? addr : addr +0.5), 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 2 + ((Select != 1) ? addx : 0), size.y / 2 - ((Select != 1) ? addy : 0), 1 - ((Select != 1) ? addr: addr -0.5), 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 3 + 100 + ((Select != 2) ? addx : 0), size.y / 2 + ((Select != 2) ? addy : 0), 1 +((Select != 2) ? addr : addr +0.5), 0, bubble, true);

	

	DrawString(size.x / 2 - (float)(GetFontSize()) * 3.0f / 2.0f, size.y / 2 + size.y / 4, "Select", 0xa000f0);

	(*FadeBubble).Draw();
}

void SelectScene::Update(const Input & p)
{
	for (auto &b : CoralBubble)
	{
		(*b).Create();
	}
	flame++;
	Cnt++;
	shader_time++;
	(this->*_updater)(p);
}
