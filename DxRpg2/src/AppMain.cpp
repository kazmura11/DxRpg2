#include "StdCommon.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"

// ---------------------------------------------------------------
// Entry Point
// ---------------------------------------------------------------
int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  // for cheking memory leak
#endif
	{
		DxLib::SetWindowTextA("DX RPG");
		if (DxLib::ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK
			|| DxLib::DxLib_Init() == -1
			|| DxLib::SetWindowSizeExtendRate(1.0) == -1
			|| DxLib::SetMouseDispFlag(FALSE) == -1
			|| DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1)
		{
			return -1;
		}

		Util::ResourceLoader& rl = Util::ResourceLoader::getInstance();
		Util::GameController& gc = Util::GameController::getInstance();
		Sequence::Parent& parent = Sequence::Parent::getInstance();
		if (rl.load() == -1)  // load images
		{
			return -1;
		}

		// Message loop
		while (DxLib::ProcessMessage() == 0
			&& DxLib::ClearDrawScreen() == 0
			&& gc.getAllKeyPressed() == 0
			&& gc.escapeNotPressed())
		{
			gc.adjustKeyState();
			parent.update();
			gc.control();
			DxLib::ScreenFlip();  // draw from the background to the frontground
		}
		DxLib::DxLib_End();
	}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
