#include "GameScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Fish.h"
#include "../Enemy/Diodon.h"
#include "../Processing/Collision.h"
#include "../Camera.h"
#include "../Object/Obstacle.h"
#include "../Object/DestroyableObject.h"
#include "../Object/PredatoryObject.h"
#include "../Object/ImmortalObject.h"

const int shader_offset = 50;

void GameScene::FadeIn(const Input & p)
{
	if (wait >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &GameScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)wait / WAITFRAME);
	Draw();
}

void GameScene::FadeOut(const Input & p)
{
	if (wait >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		(*FadeBubble).Draw();
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	else {
		(*FadeBubble).Create();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)wait / WAITFRAME);
		Draw();
	}
}

void GameScene::Wait(const Input & p)
{
	Draw();
	_updater = &GameScene::Run;
}

void GameScene::Run(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
	else if (totaltime == 0) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
}

GameScene::GameScene()
{
	auto& size = Game::GetInstance().GetScreenSize();
	_camera.reset(new Camera());

	_pl.reset(new Player(_camera));

	_col.reset(new Collision());

	_destroy.reset(new DestroyableObject(_camera));

	_predatory.reset(new PredatoryObject(_camera));

	_immortal.reset(new ImmortalObject(_camera));

	firstscreen = MakeScreen(size.x, size.y);
	secondscreen = MakeScreen(size.x - 1 , size.y - 1);
	thirdscreen = MakeScreen(size.x - 1, size.y - 1);
	_4thscreen = MakeScreen(size.x, size.y);

	for (int i = 0; i < 2; ++i)
	{
		if (i <= 0)
		{
			_enemies.push_back(std::make_shared<Fish>(_camera));
		}
		else
		{
			_enemies.push_back(std::make_shared<Diodon>(_camera));
		}
	}

	//�t�H���g�̃��[�h
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "���s", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	//�摜�̓ǂݍ���
	auto& manager = ResourceManager::GetInstance();
	sea			= manager.LoadImg("../img/sea.png");
	sea_effect	= manager.LoadImg("../img/sea2.png");
	beach		= manager.LoadImg("../img/���l.png");

	//�g�̃V�F�[�_�[���_
	for (int i = 0; i < 4; i++)
	{
		wave_vertex[i].pos = VGet((i % 2)* size.x - 1, (i / 2)*size.y -1, 0);
		wave_vertex[i].rhw = 1.0f;
		wave_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		wave_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		wave_vertex[i].u = wave_vertex[i].su = (float)(i % 2);
		wave_vertex[i].v = wave_vertex[i].sv = (float)(i / 2);
	}

	//�e�̃V�F�[�_�[
	for (int i = 0; i < 4; i++)
	{
		shadow_vertex[i].pos = VGet((i % 2)* size.x -1, (i / 2)*size.y -1, 0);
		shadow_vertex[i].rhw = 1.0f;
		shadow_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		shadow_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		shadow_vertex[i].u = shadow_vertex[i].su = (float)(i % 2);
		shadow_vertex[i].v = shadow_vertex[i].sv = (float)(i / 2);
	}

	//�I�u�W�F�N�g�̐���
	_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera));
	_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera));
	_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera));

	flame = 0;
	wait = 0;
	time = 60;
	totaltime = 60;

	shader_time = 0;

	_updater = &GameScene::FadeIn;
}

GameScene::~GameScene()
{
	
}

void GameScene::Draw()
{
	int sizex, sizey;
	DxLib::GetWindowSize(&sizex, &sizey);


	//first�X�N���[��(���l)
	SetDrawScreen(firstscreen);

	ClearDrawScreen();

	DrawExtendGraph(0 - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y,
		_camera->GetRange().x - _camera->CameraCorrection().x, _camera->GetRange().y - _camera->CameraCorrection().y, beach, true);

	auto one = totaltime % 10;
	auto ten = totaltime / 10;

	DrawFormatString(sizex / 2, GetFontSize() / 2, 0xff00ff, "%d", one);
	DrawFormatString(sizex / 2 - GetFontSize(), GetFontSize() / 2, 0xff00ff, "%d", ten);




	//second�X�N���[��(�e)
	SetDrawScreen(secondscreen);

	ClearDrawScreen();

	for (auto itr : _enemies)
	{
		itr->Draw();
	}

	//�V�F�[�_�Ŏg���e�N�X�`���͐�قǍ�����`��\�摜
	SetUseTextureToShader(0, secondscreen);

	//�s�N�Z���V�F�[�_�̃Z�b�g
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[1]);

	DrawPrimitive2DToShader(shadow_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);
	




	//third�X�N���[��(�g�V�F�[�_�[)
	SetDrawScreen(thirdscreen);

	ClearDrawScreen();

	DrawExtendGraph(sizex, sizey, 0, 0, sea_effect, true);

	//�V�F�[�_�Ŏg���e�N�X�`���͐�قǍ�����`��\�摜
	SetUseTextureToShader(0, thirdscreen);

	//�V�F�[�_�[�ɏ���n��
	SetPSConstSF(0, shader_time / 100.0f);

	//�s�N�Z���V�F�[�_�̃Z�b�g
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[0]);

	DrawPrimitive2DToShader(wave_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);

	



	//_4th�X�N���[��(�g)
	SetDrawScreen(_4thscreen);

	ClearDrawScreen();

	DrawExtendGraph(0, 0, sizex, sizey, sea, true);

	//�V�F�[�_�Ŏg���e�N�X�`���͐�قǍ�����`��\�摜
	SetUseTextureToShader(0, thirdscreen);

	//�V�F�[�_�[�ɏ���n��
	SetPSConstSF(0, shader_time / 100.0f);

	//�s�N�Z���V�F�[�_�̃Z�b�g
	SetUsePixelShader(Game::GetInstance().GetShaderHandle()[0]);

	DrawPrimitive2DToShader(wave_vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);





	//�o�b�N�`��
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawGraph(0, 0, firstscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);

	DrawGraph(-15, 15, secondscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	_pl->Draw();

	for (auto itr : _enemies)
	{
		itr->Draw();
	}

	for (auto &destroy : _destroyObj) {
		destroy->Draw();
	}
	for (auto &predatory : _predatoryObj) {
		predatory->Draw();
	}
	for (auto &immortal : _immortalObj) {
		immortal->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);

	DrawExtendGraph(0 - shader_offset, 0 - shader_offset, sizex + shader_offset, sizey + shader_offset, thirdscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);

	//DrawExtendGraph(0 - 30, 0, sizex + 50, sizey, _4thscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(*FadeBubble).Draw();

}

void GameScene::Update(const Input & p)
{
	flame++; wait++; shader_time++;

	_pl->Update(p);

	for (auto itr : _enemies)
	{
		itr->Update();
	}

	for (int i = 0; i < _enemies.size(); ++i)
	{
		/// �G�̎��S����
		if (_enemies[i]->GetInfo()._dieFlag)
		{
			_enemies.erase(_enemies.begin() + i);
			continue;
		}

		auto sVec = _pl->GetShot();
		/// ��ڲ԰���ĂƓG�̓����蔻��
		for (int p = 0; p < _pl->GetShot().size(); ++p)
		{
			if (_col->WaterToSqr(_pl->GetInfo().legs[p].tip, sVec[p],_enemies[i]->GetInfo()._rect))
			{
				auto vec = _enemies[i]->GetInfo()._pos - _pl->GetInfo().legs[p].tip;
				vec.Normalize();

				_enemies[i]->CalEscapeDir(vec);
				break;
			}
			_enemies[i]->CalTrackVel(_pl->GetInfo().center, _col->TriToTri(_pl->GetInfo().legs, _enemies[i]->GetInfo()._searchVert));
		}
		
		/// ��ڲ԰�ƓG���Ă̓����蔻��
		for (int s = 0; s < _enemies[i]->GetShotInfo().size(); ++s)
		{
			if (_col->TriToSqr(_pl->GetInfo().legs, _enemies[i]->GetShotInfo()[s]._pos, _enemies[i]->GetShotInfo()[s]._size))
			{
				_enemies[i]->ShotDelete(s);		/// ��ڲ԰�ɓ��������e�̐F��ς��Ă���B
			}
		}
	}

	//�j��\�I�u�W�F�N�g
	for (auto &destroy : _destroyObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], destroy->GetInfo()._rect))
			{
				destroy->Break();
			}
		}
	}

	//�j��s�I�u�W�F�N�g
	for (auto &predatry : _predatoryObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], predatry->GetInfo()._rect))
			{
				predatry->Break();
			}
		}
	}

	//�ߐH�Ώ�
	for (auto &immortal : _immortalObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], immortal->GetInfo()._rect))
			{
				immortal->Break();
			}
		}
	}

	if (_enemies.size() <= 0)
	{
		DrawString(0, 0, "Not Enemy", 0xffffff);
	}

	totaltime = time - (flame / 60);

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);
}