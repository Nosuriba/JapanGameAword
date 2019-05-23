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

	//PAD_INPUT_DOWN�@	// ���`�F�b�N�}�X�N(���L�[ or �e���L�[�̂Q�L�[)
	//PAD_INPUT_LEFT�@	// ���`�F�b�N�}�X�N(���L�[ or �e���L�[�̂S�L�[)
	//PAD_INPUT_RIGHT�@	// ���`�F�b�N�}�X�N(�E�L�[ or �e���L�[�̂U�L�[)
	//PAD_INPUT_UP�@	// ���`�F�b�N�}�X�N(��L�[ or �e���L�[�̂W�L�[)
	//PAD_INPUT_1�@		// 1�{�^���`�F�b�N�}�X�N(�y�L�[)
	//PAD_INPUT_2�@		// 2�{�^���`�F�b�N�}�X�N(�w�L�[)
	//PAD_INPUT_3�@		// 3�{�^���`�F�b�N�}�X�N(�b�L�[)
	//PAD_INPUT_4�@		// 4�{�^���`�F�b�N�}�X�N(�`�L�[)
	//PAD_INPUT_5�@		// 5�{�^���`�F�b�N�}�X�N(�r�L�[)
	//PAD_INPUT_6�@		// 6�{�^���`�F�b�N�}�X�N(�c�L�[)
	//PAD_INPUT_7�@		// 7�{�^���`�F�b�N�}�X�N(�p�L�[)
	//PAD_INPUT_8�@		// 8�{�^���`�F�b�N�}�X�N(�v�L�[)
	//PAD_INPUT_9�@		// 9�{�^���`�F�b�N�}�X�N(�d�r�b�L�[)
	//PAD_INPUT_10�@	// 10�{�^���`�F�b�N�}�X�N(�X�y�[�X�L�[)

	_old = _input;

	// ���͏�Ԃ��擾
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
