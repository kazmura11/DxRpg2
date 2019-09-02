#include "StdCommon.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"

// ---------------------------------------------------------------
// エントリポイント
// ---------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  // メモリリーク検出
#endif
	DxLib::SetWindowTextA("DX RPG");  // タイトルをセット
	// ウィンドウモードにセット、マウス非表示
	if ( DxLib::ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK
		|| DxLib::DxLib_Init() == -1
		|| DxLib::SetWindowSizeExtendRate(1.0) == -1
		|| DxLib::SetMouseDispFlag(FALSE) == -1)
	{
		return -1;
	}
	// 裏画面描画にする
	if ( DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1 )
	{
		return -1;
	}
	// ユーティリティクラスのインスタンスを取得
	Util::ResourceLoader &rl = Util::ResourceLoader::getInstance();
	Util::GameController &gc = Util::GameController::getInstance();
	Sequence::Parent *parent = Sequence::Parent::getInstance();
	if ( rl.load() == -1 )  // 画像読込み
	{
		return -1;
	}
	
	// メッセージループ
	while( DxLib::ProcessMessage() == 0
		&& DxLib::ClearDrawScreen() == 0
		&& gc.getAllKeyPressed() == 0
		&& gc.escapeNotPressed())
	{
		gc.adjustKeyState();
		parent->update();
		gc.control();
		DxLib::ScreenFlip();  // 裏画面を反映
	}
	DxLib::DxLib_End();
	
	return 0;
}
