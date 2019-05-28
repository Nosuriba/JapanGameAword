#include "BackGround.h"
#include <DxLib.h>
#include "Camera.h"
#include "Game.h"
#include "ResourceManager.h"



BackGround::BackGround()
{
	auto &manager = ResourceManager::GetInstance();

	_camera.reset(new Camera());
	beach = manager.LoadImg("../img/»•l.png");
}


BackGround::~BackGround()
{
}

void BackGround::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();

	DrawExtendGraph(0 - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y,
		size.x - _camera->CameraCorrection().x, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x, 0 - _camera->CameraCorrection().y,
		0 - _camera->CameraCorrection().x + size.x, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x + size.x * 2, 0 - _camera->CameraCorrection().y,
		size.x - _camera->CameraCorrection().x + size.x * 2, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x * 3, 0 - _camera->CameraCorrection().y,
		0 - _camera->CameraCorrection().x + size.x * 3, size.y - _camera->CameraCorrection().y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x, size.y - _camera->CameraCorrection().y + size.y,
		size.x - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x, size.y - _camera->CameraCorrection().y + size.y,
		0 - _camera->CameraCorrection().x + size.x, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(0 - _camera->CameraCorrection().x + size.x * 2, size.y - _camera->CameraCorrection().y + size.y,
		size.x - _camera->CameraCorrection().x + size.x * 2, 0 - _camera->CameraCorrection().y + size.y, beach, true);

	DrawExtendGraph(size.x - _camera->CameraCorrection().x + size.x * 3, size.y - _camera->CameraCorrection().y + size.y,
		0 - _camera->CameraCorrection().x + size.x * 3, 0 - _camera->CameraCorrection().y + size.y, beach, true);
}

void BackGround::Update()
{
}
