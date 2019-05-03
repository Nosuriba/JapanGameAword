#pragma once
#include "Geometry.h"

class Camera
{
private:
	int _img;

	Vector2 _pos;
	Vector2 _range;

	Vector2 _correction;

public:
	Camera();
	~Camera();

	void Update(const Vector2&);
	void Draw();

	// �\���␳
	const Vector2 CameraCorrection() const;
};

