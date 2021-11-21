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
		if (countForFPS_ == 0) {
			prevTime_ = DxLib::GetNowCount();
		}
		if (countForFPS_ == MetricTimes) {
			int t = DxLib::GetNowCount();
			framePerSecond_ = 1000.f / ((t - prevTime_) / static_cast<float>(MetricTimes));
			countForFPS_ = 0;
			prevTime_ = t;
		}
		countForFPS_++;
	}

	void GameController::drawFps()
	{
		if (framePerSecond_ != 0)	// avoid dividing by zero
		{
			DxLib::DrawFormatString(0, 0, static_cast<int>(DxLib::GetColor(255, 255, 255)),
				"FPS %.1f", framePerSecond_);
		}
	}

	void GameController::controlFps()
	{
		int elapsedTime = DxLib::GetNowCount() - prevTime_; // calculate 1 loop time
		int waitTime = countForFPS_ * 1000 / FPS - elapsedTime;
		if (waitTime > 0) {
			Sleep(waitTime);
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
		calcFps();
#ifdef _DEBUG
		drawFps();
#endif
		controlFps();  // keep 60 fps

		gCount_++;
		if (gCount_ >= GCountMax)
		{
			gCount_ = 0;
		}
	}
}