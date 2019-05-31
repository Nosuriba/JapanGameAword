#pragma once
#include "Processing/Geometry.h"

class Camera
{
private:
	Vector2 _pos;
	Vector2 _target;

	Vector2 _range;

	Vector2 _correction;

public:
	Camera();
	~Camera();

	void Init(const Vector2& p);
	void Update();
	void SetFocus(const Vector2& p);

	// �\���␳
	const Vector2 CameraCorrection() const;
	// �e�\���ʒu
	const Vector2 GetShadowPos(const float& h);

	const Vector2 GetRange() const;

	const void SetRange(const Vector2& range);
};

