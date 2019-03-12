#include <DxLib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int hShowCmd)
{
	/// とりあえず、画面の表示をするところまでは修正した（サイズは適当）。
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(640, 640, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("名称未設定だぞ(´･ω･`)");
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