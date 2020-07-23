#pragma once
#include "StdCommon.h"

namespace
{
	const int	MapTipNum = 4;  // number of mapchip
	const int	BackGroundNum = 20;  // number of background
	const int	MonsterNum = 10;
	const int	ImgNumberKind = 5;
	const int	ImgNumberDivNum = 10;
	const int	ImgAnimationKind = 10;
	const int	ImgAnimationDivNum = 25;
	const int	ImgEtcNum = 5;
}

namespace Util
{
	class ResourceLoader
	{
	public:
		// for singleton
		ResourceLoader(const ResourceLoader&) = delete;
		ResourceLoader& operator=(const ResourceLoader&) = delete;
		ResourceLoader(ResourceLoader&&) = delete;
		ResourceLoader& operator=(ResourceLoader&&) = delete;

		static ResourceLoader &getInstance();

		int load();
		void captureImgMap();
		int getHdlImgChar(int kind, int pos) const;
		int getHdlImgMapchip(int kind) const;
		int getHdlImgBackGround(int kind) const;
		int getHdlImgMonster(int kind) const;
		int getHdlImgStartBattle(int kind) const;
		void setImageStartBattle(int idx, int width, int height);

		int getCureImage(int frame) const;

		int getSlashImage(int frame) const;
		int getWindImage(int frame) const;
		int getFireImage(int frame) const;
		int getIceImage(int frame) const;
		int getThunderImage(int frame) const;

		int getHdlImgNumber(int kind, int pos) const;

		int getHdlImgEtc(int kind) const;
		int getHdlImgCursor() const;
		int getHdlFont(int kind) const;
		int getHdlNullImg() const;

	private:
		ResourceLoader()
			:hdlFont_{}, hdlImgAnimation_{}, hdlImgBackGround_{}, hdlImgChar_{}, hdlImgCursor_{},
			hdlImgEtc_{}, hdlImgMapchip_{}, hdlImgMonster_{}, hdlImgNumber_{}, hdlImgStartBattle_{}
			{};
		~ResourceLoader() {};
		void dispProgress();
		void loadMapchip(char *resourcePath, int *handle);
		void loadCharacterTip(char *resourcePath, int *handle);
		void loadImage(char *resourcePath, int *handle);
		
		int hdlImgMapchip_[MapTipNum];
		int hdlImgChar_[CharKindMax][ImgCharDivNum];
		int hdlImgBackGround_[BackGroundNum];
		int hdlImgMonster_[MonsterNum];
		int hdlImgStartBattle_[ImgStartBattleDivNum];

		int hdlImgNumber_[ImgNumberKind][ImgNumberDivNum];
		int hdlImgAnimation_[ImgAnimationKind][ImgAnimationDivNum];
		int hdlImgEtc_[ImgEtcNum];
		int hdlImgCursor_;
		int hdlFont_[2];
	};
}