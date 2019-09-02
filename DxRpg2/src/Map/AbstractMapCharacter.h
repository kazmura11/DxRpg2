#pragma once
#include "StdCommon.h"

namespace Map
{
	const int	DefWalkSpeed	= 1;
	// 町の人間が歩き回る度合い -> 多いほどたまにしか動かない
	const int	WalkRand	= 100;
	// int型「dir」の値をがx,yそれぞれの方向の相対位置としてに分解したもの
	// x,yはそれぞれ、下、左、右、上
	const int OffsetX[] = {  0, -1,  1,  0 };
	const int OffsetY[] = {  1,  0,  0, -1 }; 
	const int DirNum = 4;

	class AbstractMapCharacter
	{
	public:
		AbstractMapCharacter();
		virtual ~AbstractMapCharacter(){};

		int getX() const;
		int getY() const;
		int getImg() const;
		bool getImgFlag() const;
		int getDir() const;
		int getKind() const;
		int getAnimePicPos() const;
		void setImg(int img);
		void setImgFlag(bool imgFlag);
		void setX(const int x);
		void setY(const int y);

		virtual void initMapState(int tx, int ty, int dir, int kind) = 0;
		virtual bool move(int *isPassable) = 0;

	protected:
		int x_;  // キャラのＸ座標
		int y_;  // キャラのＹ座標
		int walkPixel_;  // アニメーションの絵でどの絵を使うかの判断のための歩数
		bool walkFlag_;  // 歩いているか歩いていないか
		int dir_;  // ４方向を管理
		bool flag_;  // キャラクタの存在フラグ
		int img_;  // キャラクタの絵
		bool imgFlag_;  // キャラクタの絵の描画フラグ
		int kind_;  // 主人公とか町の人の種類とか
		int animePicPos_;  // 16分割画像のキャラ絵位置  0-15
		int walkSpeed_;  // キャラの動く速さ

		void stop();
		void walk();
		void setPosition();
		int decideAnimePic(int WalkPixel, int dir);
		bool canMove(int isPassable);
	};
}