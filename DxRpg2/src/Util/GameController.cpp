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
			frameSpdAvg_ = 0;
			for (int i = 0; i < MetricTimes; i++)
			{
				frameSpdAvg_ += fps_[i];
			}
			frameSpdAvg_ /= MetricTimes;
		}
	}

	void GameController::graphFps()
	{
		if (frameSpdAvg_ != 0)	// avoid dividing by zero
		{
			DxLib::DrawFormatString(0, 0, static_cast<int>(DxLib::GetColor(255, 255, 255)),
				"FPS %.1f", 1000.0 / (double)(frameSpdAvg_));
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
		return getKey(input) == static_cast<char>(Util::GameController::KeyState::NotPressed);
	}

	bool GameController::keyPressed(int input) const
	{
		return getKey(input) == static_cast<char>(Util::GameController::KeyState::Pressed)
			|| getKey(input) == static_cast<char>(Util::GameController::KeyState::PressedNow);
	}

	bool GameController::escapeNotPressed() const
	{
		return getKey(KEY_INPUT_ESCAPE) == static_cast<char>(KeyState::NotPressed);
	}

	bool GameController::eNotPressed() const
	{
		return getKey(KEY_INPUT_E) == static_cast<char>(Util::GameController::KeyState::NotPressed);
	}

	bool GameController::bPressed() const
	{
		return getKey(KEY_INPUT_B) == static_cast<char>(Util::GameController::KeyState::Pressed);
	}
	bool GameController::fPressed() const
	{
		return getKey(KEY_INPUT_F) == static_cast<char>(Util::GameController::KeyState::Pressed);
	}


	bool GameController::upPressedNow() const
	{
		return getKey(KEY_INPUT_UP) == static_cast<char>(Util::GameController::KeyState::PressedNow);
	}

	bool GameController::downPressedNow() const
	{
		return getKey(KEY_INPUT_DOWN) == static_cast<char>(Util::GameController::KeyState::PressedNow);
	}

	bool GameController::xPressedNow() const
	{
		return getKey(KEY_INPUT_X) == static_cast<char>(Util::GameController::KeyState::PressedNow);
	}

	bool GameController::zPressedNow() const
	{
		return getKey(KEY_INPUT_Z) == static_cast<char>(Util::GameController::KeyState::PressedNow);
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
			if (prevKey_[i] == static_cast<char>(KeyState::NotPressed)
				&& key_[i] == static_cast<char>(KeyState::Pressed))
			{
				key_[i] = static_cast<char>(KeyState::PressedNow);  // if change state, set pressed state
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