#pragma once
#include<DxLib.h>
#include "Geometry.h"

// XInputƒ{ƒ^ƒ““ü—Í’è‹`
enum class BUTTON {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	BACK,
	L_STICK,
	R_STICK,
	LB,
	RB,
	A = 12,
	B = 13,
	X = 14,
	Y = 15
};

enum class TRIGGER {
	LEFT,
	RIGHT
};

enum class STICK {
	LEFT,
	RIGHT
};

class Input
{
private:
	int _InputKey;
	int _lastInputKey;
	int _mouseInput;
	int _lastMouseInput;

	XINPUT_STATE _input;
	XINPUT_STATE _old;

public:
	Input();
	~Input();

	const bool IsPressed(int p) const;
	const bool IsTrigger(int p) const;
	const bool IsMousePressed(int p) const;
	const bool IsMouseTrigger(int p) const;

	void Update();

	const bool Push(BUTTON) const;
	const bool Trigger(BUTTON) const;

	const int PushTrigger(TRIGGER t) const;
	const int TriggerTrigger(TRIGGER t) const;
	const bool ReleaseTrigger(TRIGGER t) const;

	const Vector2 Stick(STICK s) const;
};

