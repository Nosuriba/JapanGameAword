#include <DxLib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int hShowCmd)
{
	/// �Ƃ肠�����A��ʂ̕\��������Ƃ���܂ł͏C�������i�T�C�Y�͓K���j�B
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(640, 640, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("���̖��ݒ肾��(�L��֥`)");
	if (DxLib_Init() == -1)
	{
		return false;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

	}

	DxLib_End();
	return 0;
}