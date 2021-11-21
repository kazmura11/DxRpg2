#pragma once
#include <climits>
namespace
{
	constexpr int	FPS = 60;   // FPS
	constexpr int	MetricTimes = 30;   // times to calculate avg
	constexpr int	GCountMax = INT_MAX;   // global counter
	constexpr int	KeyKindNum = 256;
}

namespace Util
{
	class GameController
	{
	public:
		// for singleton
		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;
		GameController(GameController&&) = delete;
		GameController& operator=(GameController&&) = delete;

		enum class KeyState
		{
			NotPressed = 0,
			Pressed = 1,
			PressedNow = 2
		};
		static GameController &getInstance();

		bool keyNotPressed(int input) const;
		bool escapeNotPressed() const;
		bool eNotPressed() const;

		bool keyPressed(int input) const;
		bool bPressed() const;
		bool fPressed() const;

		bool upPressedNow() const;
		bool downPressedNow() const;
		bool xPressedNow() const;
		bool zPressedNow() const;
		
		int getAllKeyPressed();

		int getGCount() const;

		void adjustKeyState();
		void control();

	private:
		GameController() : framePerSecond_(0), gCount_(0), key_{ 0 }, prevKey_{ 0 } {};
		~GameController() {};
		
		char getKey(int input) const;
		void calcFps();
		void drawFps();
		void controlFps();

		int gCount_;
		int countForFPS_;
		float framePerSecond_;  // avarage frame speed
		//int elapsedTime_;  // for adjusting fps
		int prevTime_;
		char key_[KeyKindNum];
		char prevKey_[KeyKindNum];
	};
}

