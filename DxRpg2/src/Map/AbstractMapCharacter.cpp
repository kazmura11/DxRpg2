#include "Map/AbstractMapCharacter.h"

namespace Map
{
	AbstractMapCharacter::AbstractMapCharacter()
		: x_(0), y_(0), walkPixel_(0), walkFlag_(false), dir_(Direction::Down),
		flag_(0), img_(0), imgFlag_(0), kind_(0),
		animePicPos_(0), walkSpeed_(DefWalkSpeed)
	{
	}

	int AbstractMapCharacter::getX() const
	{
		return x_;
	}

	int AbstractMapCharacter::getY() const
	{
		return y_;
	}

	int AbstractMapCharacter::getImg() const
	{
		return img_;
	}

	bool AbstractMapCharacter::getImgFlag() const
	{
		return imgFlag_;
	}

	Direction AbstractMapCharacter::getDir() const
	{
		return dir_;
	}

	int AbstractMapCharacter::getKind() const
	{
		return kind_;
	}

	int AbstractMapCharacter::getAnimePicPos() const
	{
		return animePicPos_;
	}

	void AbstractMapCharacter::stop()
	{
		walkPixel_ = 0;
		walkFlag_ = false;
	}
	void AbstractMapCharacter::walk()
	{
		walkPixel_++;
	}

	void AbstractMapCharacter::setImg(int img)
	{
		img_ = img;
	}
	void AbstractMapCharacter::setImgFlag(bool imgFlag)
	{
		imgFlag_ = imgFlag;
	}

	void AbstractMapCharacter::setPosition()
	{
		if (walkFlag_)
		{
			// walkSpeed分だけ方向に対してキャラの座標を増減する。
			x_ = x_ + OffsetX[static_cast<int>(dir_)] * walkSpeed_;
			y_ = y_ + OffsetY[static_cast<int>(dir_)] * walkSpeed_;
		}
	}

	int AbstractMapCharacter::decideAnimePic(int walkPixel, Direction dir)
	{
		int axisX;
		int axisY;
		// walkPixelは1ピクセル歩くと1増加
		// 1区画のどのあたりにいるのかというのを、
		// 1区画を4分割した値でわってもとまるのがその絵となる。
		// アニメーションのどれかを特定（画像のx方向[1次元配列]）
		// 0-31を想定していて32px / 8をすると0-3に均等にばらつき
		// 8でわれば0-3に収まる
		axisX = (walkPixel % BlockLen) / 8;
		// ここでツクールMVの仕様に合わせて変更(横3x横4)
		//真ん中が静止画像なので補正する(ここで0は静止扱いである)
		axisX = axisX == 0 ? 1 : axisX - 1;
		// アニメーションの向きを特定(画像のy方向[1次元配列])
		axisY = static_cast<int>(dir) * 3;
		// 0～2は下向き画像、3～5は左向き画像、
		// 6～8は右向き画像、9～11は上向き画像･･･と数値を変換
		return axisX + axisY;
	}

	bool AbstractMapCharacter::canMove(PassFlag isPassable)
	{
		// 主人公
		if (kind_ == MainCharIndex)
		{
			return !(isPassable == PassFlag::NoThrough);
		}
		// 町キャラ
		else
		{
			return !(isPassable >= PassFlag::MainCharOnly);
		}
	}

	void AbstractMapCharacter::setX(const int x)
	{
		x_ = x;
	}
	void AbstractMapCharacter::setY(const int y)
	{
		y_ = y;
	}
}