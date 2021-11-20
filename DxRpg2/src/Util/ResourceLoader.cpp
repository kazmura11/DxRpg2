#include "ResourceLoader.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Util
{
	constexpr float  LoadNum = 32.0;  // number of calling dispProgress()

	ResourceLoader &ResourceLoader::getInstance()
	{
		static ResourceLoader instance;
		return instance;
	}

	void ResourceLoader::dispProgress()
	{
		static int currentLoadNum = 0;
		DxLib::ProcessMessage();
		DxLib::ClearDrawScreen();
		currentLoadNum++;
		DxLib::DrawFormatString(WindWidth / 2 - 60, WindHeight / 2 + 160,
			GetColor(255, 255, 255), "Now Loading....");
		DxLib::DrawFormatString(WindWidth / 2 - 20, WindHeight / 2 + 200,
			GetColor(255, 255, 255), "%.0f％", currentLoadNum / LoadNum * static_cast<double>(100));
		DxLib::ScreenFlip();
	}

	void ResourceLoader::loadMapchip(char *resourcePath, int *handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			MapTipNum, 4, 1, 32, 32, handle);
		THROWS_EX(ret, resourcePath);
		dispProgress();
	}

	void ResourceLoader::loadCharacterTip(char *resourcePath, int *handle) {
#ifdef CHARCHIP32X48  // 32x48 charchip
		int ret = DxLib::LoadDivGraph(resourcePath, ImgCharDivNum, 3, 4, 32, 48, handle);
#else  // 32x32 charchip
		int ret = DxLib::LoadDivGraph(resourcePath, ImgCharDivNum, 3, 4, 32, 32, handle);
#endif
		THROWS_EX(ret, resourcePath);
		dispProgress();
	}

	void ResourceLoader::loadImage(char *resourcePath, int *handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
		dispProgress();
	}

	// データのロード
	int ResourceLoader::load()   // TODO: voidでよい…
	{
		int ret = 0;
		try {
			// mapchip
			loadMapchip("resources/img/maptip/field.png", hdlImgMapchip_);

			// character image
			loadCharacterTip("resources/img/character/1.png", hdlImgChar_[0]);
			loadCharacterTip("resources/img/character/2.png", hdlImgChar_[1]);
			loadCharacterTip("resources/img/character/3.png", hdlImgChar_[2]);
			loadCharacterTip("resources/img/character/4.png", hdlImgChar_[3]);

			// backbround image
			loadImage("resources/img/back/kaidou0331_800b.jpg", &hdlImgBackGround_[0]);
			// message board in battle mode
			loadImage("resources/img/battle/0.png", &hdlImgEtc_[1]);
			loadImage("resources/img/battle/10.png", &hdlImgBackGround_[10]);
			loadImage("resources/img/battle/11.png", &hdlImgBackGround_[11]);
			
			// monster image
			hdlImgMonster_[0] = DxLib::LoadGraph("resources/img/monster/enemy_popm003.png");
			THROWS_EX(hdlImgMonster_[0], "resources/img/monster/enemy_popm003.png");
			dispProgress();

			// number
			ret = DxLib::LoadDivGraph("resources/img/num/suuji12x24_06.png", ImgNumberDivNum, 10, 1, 12, 24, hdlImgNumber_[0]);
			THROWS_EX(ret, "resources/img/num/0.png");
			dispProgress();
			ret = DxLib::LoadDivGraph("resources/img/num/suuji12x24_04.png", ImgNumberDivNum, 10, 1, 12, 24, hdlImgNumber_[1]);
			THROWS_EX(ret, "resources/img/num/1.png");
			dispProgress();

			// animation
			// magic: cure
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect016.png", 8, 8, 1, 120, 120, hdlImgAnimation_[0]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect045.png");
			dispProgress();

			// physical attack
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect001.png", 5, 5, 1, 120, 120, hdlImgAnimation_[1]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect001.png");
			dispProgress();

			// magic: wind
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect039.png", 8, 8, 1, 120, 120, hdlImgAnimation_[2]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect039.png");
			dispProgress();
			// magic: fire
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect037.png", 8, 8, 1, 120, 120, hdlImgAnimation_[3]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect037.png");
			dispProgress();
			// magic: ice
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect038.png", 8, 8, 1, 120, 120, hdlImgAnimation_[4]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect038.png");
			dispProgress();
			// magic: thunder
			ret = DxLib::LoadDivGraph("resources/img/animation/pipo-btleffect040.png", 8, 8, 1, 120, 120, hdlImgAnimation_[5]);
			THROWS_EX(ret, "resources/img/animation/pipo-btleffect040.png");
			dispProgress();

			// cursor image
			loadImage("resources/img/icon/cursor.png", &hdlImgCursor_);

			// Consolas
			hdlFont_[0] = CreateFontToHandle("Consolas", 18, 3, DX_FONTTYPE_ANTIALIASING_EDGE);  // 日本語不可
			hdlFont_[1] = CreateFontToHandle("メイリオ", 12, 2, DX_FONTTYPE_ANTIALIASING_EDGE);

		}
		catch (const char* ex)
		{
			DxLib::printfDx("%s ERROR LoadFailed %s", __FILE__, ex);
			exit(1);
		}

		return ret;
	}

	void ResourceLoader::captureImgMap()
	{
		for (int i = 0; i < DivisionNum; i++)
		{
			for (int j = 0; j < DivisionNum; j++)
			{
				// create 16 divided image
				hdlImgStartBattle_[i * DivisionNum + j]
					= DxLib::MakeGraph(
						WindWidth / DivisionNum,
						WindHeight / DivisionNum
					);
				DxLib::GetDrawScreenGraph(
					0 + (WindWidth / DivisionNum) * j,
					0 + (WindHeight / DivisionNum) * i,
					0 + (WindWidth / DivisionNum) * (j + 1),
					0 + (WindHeight / DivisionNum) * (i + 1),
					hdlImgStartBattle_[i * DivisionNum + j]
				);
			}
		}
	}

	int  ResourceLoader::getHdlImgChar(int kind, int pos) const
	{
		if (kind >= 0 && kind <= 3)
		{
			if (0 <= pos && pos <= 11)
			{
				return hdlImgChar_[kind][pos];
			}
		}
		DxLib::printfDx("ERROR getHdlImgChar()\n");
		return 0;
	}

	int  ResourceLoader::getHdlImgMapchip(int kind) const
	{
		return hdlImgMapchip_[kind];
	}

	int  ResourceLoader::getHdlImgBackGround(int kind) const
	{
		return hdlImgBackGround_[kind];
	}

	int  ResourceLoader::getHdlImgMonster(int kind) const
	{
		return hdlImgMonster_[kind];
	}

	int  ResourceLoader::getHdlImgStartBattle(int kind) const
	{
		return hdlImgStartBattle_[kind];
	}

	void ResourceLoader::setImageStartBattle(int idx, int width, int height)
	{
		hdlImgStartBattle_[idx] = DxLib::MakeGraph(width, height);
	}

	int ResourceLoader::getHdlImgNumber(int kind, int pos) const
	{
		return hdlImgNumber_[kind][pos];
	}

	int ResourceLoader::getCureImage(int frame) const {
		return hdlImgAnimation_[0][frame];
	}

	int ResourceLoader::getSlashImage(int frame) const {
		return hdlImgAnimation_[1][frame];
	}

	int ResourceLoader::getWindImage(int frame) const {
		return hdlImgAnimation_[2][frame];
	}

	int ResourceLoader::getFireImage(int frame) const {
		return hdlImgAnimation_[3][frame];
	}

	int ResourceLoader::getIceImage(int frame) const {
		return hdlImgAnimation_[4][frame];
	}

	int ResourceLoader::getThunderImage(int frame) const {
		return hdlImgAnimation_[5][frame];
	}

	int  ResourceLoader::getHdlImgEtc(int kind) const
	{
		return hdlImgEtc_[kind];
	}

	int  ResourceLoader::getHdlImgCursor() const
	{
		return hdlImgCursor_;
	}

	int  ResourceLoader::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}

	int ResourceLoader::getHdlNullImg() const
	{
		return 0;
	}
}