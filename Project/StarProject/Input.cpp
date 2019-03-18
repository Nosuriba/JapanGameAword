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

void Input::Update()
{
	_lastInputKey = _InputKey;

	_InputKey = DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1);

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
}
