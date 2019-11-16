#include "GameController.h"
#include "StdCommon.h"

namespace Util
{
	GameController &GameController::getInstance()
	{
		static GameController instance;
		return instance;
	}

	void GameController::calcFps()
	{
		fps_[gCount_ % MetricTimes] = waitTime_;
		// if gCount_ reached to metric times... update avarage speed;
		if ((gCount_ % MetricTimes) == (MetricTimes - 1))
		{
			frameSpdAvg = 0;
			for (int i = 0; i < MetricTimes; i++)
			{
				frameSpdAvg += fps_[i];
			}
			frameSpdAvg /= MetricTimes;
		}
	}

	void GameController::graphFps()
	{
		if (frameSpdAvg != 0)	// avoid dividing by zero
		{
			DxLib::DrawFormatString(0, 0, static_cast<int>(DxLib::GetColor(255, 255, 255)),
				"FPS %.1f", 1000.0 / (double)(frameSpdAvg));
		}
	}

	void GameController::controlFps()
	{
		waitTime_ = DxLib::GetNowCount() - prevTime_; // calculate 1 loop time
		if (prevTime_ == 0)
		{
			waitTime_ = OneFrameMillsec;
		}
		prevTime_ = DxLib::GetNowCount();  // set current time
		// adjust to 60 fps
		if (OneFrameMillsec > waitTime_)
		{
			Sleep(static_cast<DWORD>(OneFrameMillsec - waitTime_));
		}
	}

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
		return getKey(input) == Util::GameController::Pressed
			|| getKey(input) == Util::GameController::PressedNow;
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

	// adjust moving speed by keybord
	void GameController::adjustKeyState()
	{
		for (int i = 0; i < KeyKindNum; i++)
		{
			// compare previous state with current state
			if (prevKey_[i] == NotPressed && key_[i] == Pressed)
			{
				key_[i] = PressedNow;  // if change state, set pressed state
			}
			prevKey_[i] = key_[i];
		}
	}

	// call from main loop
	void GameController::control()
	{
		controlFps();  // keep 60 fps
#ifdef _DEBUG
		calcFps();
		graphFps();
#endif
		increaseGCount();
	}
}