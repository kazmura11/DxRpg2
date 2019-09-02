#include "Map/MapCityCharacter.h"

namespace Map
{
	void MapCityCharacter::initMapState(int tx, int ty, int dir, int kind)
	{
		x_ = tx;
		y_ = ty;
		flag_ = true; // キャラ生存
		dir_ = dir;  // キャラのむいている向きを下にセット D:0,L:1,R:2,U:3
		imgFlag_ = true;  // 描画する
		kind_ = kind;  // 町の人の種類
		walkPixel_ = 0;
		walkFlag_ = false;
		img_ = 0;
		walkSpeed_ = DefWalkSpeed;
		animePicPos_ = 1;  // 真下
		stop();  // 歩数０
	}

	bool MapCityCharacter::move(int *isPassable)
	{
		setPosition();  // キャラクタ位置の決定
		// xもyも座標が１区画の倍数のとき
		if (x_ % BlockLen == 0 && y_ % BlockLen == 0)
		{
			stop();  // 歩くカウンタを戻す
			if (GetRand(WalkRand) == 0)  // 歩き始める
			{
				dir_ = GetRand(3);  // ランダムに方向を決定 0-3
				int cy = y_ / BlockLen;
				int cx = x_ / BlockLen;
				int ny = y_ / BlockLen + OffsetY[dir_];
				int nx = x_ / BlockLen + OffsetX[dir_];

				if (canMove(isPassable[ny * XBlock + nx]))
				{
					// 現在位置を通れるものにして、向かう位置を通れるないものとする
					walkFlag_ = true;
					isPassable[cy * XBlock + cx] = Through;
					isPassable[ny * XBlock + nx] = NoThrough;
				}
				else  // 向いてる方向が通れない場所なら
				{
					walkFlag_ = false;
				}
			}
		}
		else  // もyも座標が１区画の倍数でないとき
		{
			walk();  // 常にdirの方向へ歩く
		}
		animePicPos_ = decideAnimePic(walkPixel_, dir_);
		return false;  // 常にfalseを返す(エンカウントしない)
	}
}
