#pragma once
#include <memory>

class Camera;

class BackGround
{
private:
	std::shared_ptr<Camera> _camera;
	int beach;

public:
	BackGround();
	~BackGround();
	void Draw();
	void Update();
};

