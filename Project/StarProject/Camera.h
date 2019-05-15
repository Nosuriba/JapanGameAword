#pragma once
#include "Processing/Geometry.h"

class Camera
{
private:
	Vector2 _pos;
	Vector2 _range;

	Vector2 _correction;

public:
	Camera();
	~Camera();

	void Update(const Vector2&);
	void Draw();

	// 表示補正
	const Vector2 CameraCorrection() const;
	// 影表示位置
	const Vector2 GetShadowPos(const float& h);

	const Vector2 GetRange() const;
};

