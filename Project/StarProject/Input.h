#pragma once
#include<DxLib.h>
class Input
{
private:
	int _InputKey;
	int _lastInputKey;

public:
	Input();
	~Input();

	const bool IsPressed(int p) const;
	const bool IsTrigger(int p) const;

	void Update();
};

