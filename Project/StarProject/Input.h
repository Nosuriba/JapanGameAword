#pragma once
#include<DxLib.h>
class Input
{
private:
	int _InputKey;
	int _lastInputKey;
	int _mouseInput;
	int _lastMouseInput;

public:
	Input();
	~Input();

	const bool IsPressed(int p) const;
	const bool IsTrigger(int p) const;
	const bool IsMousePressed(int p) const;
	const bool IsMouseTrigger(int p) const;

	void Update();
};

