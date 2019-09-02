#include "GameController.h"
#include "StdCommon.h"

namespace Util
{
	GameController &GameController::getInstance()
	{
		static GameController instance;
		return instance;
	}

	// FPS計測関数
	void GameController::calcFps()
	{
		fps_[gCount_ % MetricTimes] = waitTime_;  // １周の時間を格納
		if ((gCount_ % MetricTimes) == (MetricTimes - 1))  // 計測回数に達したら
		{
			frameSpdAvg = 0;
			for (int i = 0; i < MetricTimes; i++)
			{
				frameSpdAvg += fps_[i];
			}
			frameSpdAvg /= MetricTimes;
		}
	}

	// FPS表示
	void GameController::graphFps()
	{
		if (frameSpdAvg != 0)	// 0割り禁止により、FPS平均が0じゃなかったらFPS表示
		{
			DxLib::DrawFormatString(0, 0, static_cast<int>(DxLib::GetColor(255, 255, 255)),
				"FPS %.1f", 1000.0 / (double)(frameSpdAvg));
		}
	}

	// FPS制御
	void GameController::controlFps()
	{
		waitTime_ = DxLib::GetNowCount() - prevTime_; //１周の処理にかかった時間を計算
		if (prevTime_ == 0)
		{
			// t == 0 つまり一番最初に処理が行われたときは16を代入
			waitTime_ = OneFrameMillsec;
		}
		prevTime_ = DxLib::GetNowCount();  // 現在の時刻を格納
		if (OneFrameMillsec > waitTime_)  // １周かかった時間がFPS60つまり１周16msよりも早く行われたとき
		{
			Sleep(static_cast<DWORD>(OneFrameMillsec - waitTime_));  // FPS60になるように、つまり１周16msまで待つ。
		}
	}

	// Private
	char GameController::getKey(int input) const
	{
		return key_[input];
	}

	bool GameController::keyNotPressed(int input) const
	{
		return getKey(input) == Util::GameController::NotPressed;
	}

	bool GameController::keyPressed(int input) const
	{
		return getKey(input) == Util::GameController::Pressed;
	}

	bool GameController::escapeNotPressed() const
	{
		return getKey(KEY_INPUT_ESCAPE) == NotPressed;
	}

	bool GameController::eNotPressed() const
	{
		return getKey(KEY_INPUT_E) == Util::GameController::NotPressed;
	}


	bool GameController::bPressed() const
	{
		return getKey(KEY_INPUT_B) == Util::GameController::Pressed;
	}
	bool GameController::fPressed() const
	{
		return getKey(KEY_INPUT_F) == Util::GameController::Pressed;
	}


	bool GameController::upPressedNow() const
	{
		return getKey(KEY_INPUT_UP) == Util::GameController::PressedNow;
	}

	bool GameController::downPressedNow() const
	{
		return getKey(KEY_INPUT_DOWN) == Util::GameController::PressedNow;
	}

	bool GameController::xPressedNow() const
	{
		return getKey(KEY_INPUT_X) == Util::GameController::PressedNow;
	}

	bool GameController::zPressedNow() const
	{
		return getKey(KEY_INPUT_Z) == Util::GameController::PressedNow;
	}

	int GameController::getAllKeyPressed()
	{
		return DxLib::GetHitKeyStateAll(key_);
	}

	void GameController::increaseGCount()
	{
		gCount_++;
		if (gCount_ >= GCountMax)
		{
			gCount_ = 0;
		}
	}

	int GameController::getGCount() const
	{
		return gCount_;
	}

	// マップ画面の移動以外で、移動速度が早くなりすぎるのを防ぐための機能
	// メインループの最初に必ず書かなければならない
	void GameController::adjustKeyState()
	{
		for (int i = 0; i < KeyKindNum; i++)
		{
			// 前に押してなくて、今押していたら
			if (prevKey_[i] == NotPressed && key_[i] == Pressed)
			{
				key_[i] = PressedNow;  // 今の瞬間押されたという意味の2を代入。
			}
			prevKey_[i] = key_[i];  // 今の入力状態を過去に入力されたデータとしてコピー
		}
	}

	// 通常はメインループの中でこれだけを呼べばよい
	void GameController::control()
	{
		controlFps();  // FPS制御（FPSを60くらいに保つための関数）
#ifdef _DEBUG
		//calcFps();   // 現在のFPSを計算
		//graphFps();
#endif
		increaseGCount();  // 内部カウンタを1増加
	}
}