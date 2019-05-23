#include "Input.h"

Input::Input()
{
}


Input::~Input()
{
}

const bool Input::IsPressed(int p) const
{
	return _InputKey & p;
}

const bool Input::IsTrigger(int p) const
{
	return (_InputKey & p) && !(_lastInputKey);
}

const bool Input::IsMousePressed(int p) const
{
	return _mouseInput & p;
}

const bool Input::IsMouseTrigger(int p) const
{
	return (_mouseInput & p) && !(_lastMouseInput);
}

void Input::Update()
{
	_lastInputKey   = _InputKey;
	_lastMouseInput = _mouseInput;

	_InputKey   = DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1);
	_mouseInput = DxLib::GetMouseInput();

	//PAD_INPUT_DOWN　	// ↓チェックマスク(下キー or テンキーの２キー)
	//PAD_INPUT_LEFT　	// ←チェックマスク(左キー or テンキーの４キー)
	//PAD_INPUT_RIGHT　	// →チェックマスク(右キー or テンキーの６キー)
	//PAD_INPUT_UP　	// ↑チェックマスク(上キー or テンキーの８キー)
	//PAD_INPUT_1　		// 1ボタンチェックマスク(Ｚキー)
	//PAD_INPUT_2　		// 2ボタンチェックマスク(Ｘキー)
	//PAD_INPUT_3　		// 3ボタンチェックマスク(Ｃキー)
	//PAD_INPUT_4　		// 4ボタンチェックマスク(Ａキー)
	//PAD_INPUT_5　		// 5ボタンチェックマスク(Ｓキー)
	//PAD_INPUT_6　		// 6ボタンチェックマスク(Ｄキー)
	//PAD_INPUT_7　		// 7ボタンチェックマスク(Ｑキー)
	//PAD_INPUT_8　		// 8ボタンチェックマスク(Ｗキー)
	//PAD_INPUT_9　		// 9ボタンチェックマスク(ＥＳＣキー)
	//PAD_INPUT_10　	// 10ボタンチェックマスク(スペースキー)

	_old = _input;

	// 入力状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &_input);
}

const bool Input::Push(BUTTON b) const
{
	if (_input.Buttons[(int)b])
		return true;
	return false;
}

const bool Input::Trigger(BUTTON b) const
{
	if (_input.Buttons[(int)b] && _old.Buttons[(int)b] == 0)
		return true;
	return false;
}

const int Input::PushTrigger(TRIGGER t) const
{
	if (t == TRIGGER::LEFT) return _input.LeftTrigger;
	if (t == TRIGGER::RIGHT) return _input.RightTrigger;
	return 0;
}

const int Input::TriggerTrigger(TRIGGER t) const
{
	if (t == TRIGGER::LEFT && _old.LeftTrigger == 0) return _input.LeftTrigger;
	if (t == TRIGGER::RIGHT && _old.RightTrigger == 0) return _input.RightTrigger;
	return 0;
}

const bool Input::ReleaseTrigger(TRIGGER t) const
{
	if (t == TRIGGER::LEFT && _old.LeftTrigger > 0 && _input.LeftTrigger == 0) return true;
	if (t == TRIGGER::RIGHT && _old.RightTrigger > 0 && _input.RightTrigger == 0) return true;

	return false;
}

const Vector2 Input::Stick(STICK s) const
{
	if (s == STICK::LEFT)return Vector2(_input.ThumbLX / 10000, -_input.ThumbLY / 10000);
	if (s == STICK::RIGHT)return Vector2(_input.ThumbRX / 10000, -_input.ThumbRY / 10000);
	return Vector2();
}
