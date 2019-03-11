#include <DxLib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int hShowCmd)
{
	DxLib_Init();
	/*while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
	{

	}*/
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(640, 640, 32);
	WaitKey();
	DxLib_End();
	return 0;
}