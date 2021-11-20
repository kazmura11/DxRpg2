#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#include <crtdbg.h>
#endif
#include <cassert>  // DEBUG
#include "DxLib.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

// Constant
constexpr double PI = 3.14159265358979;
constexpr int WindWidth = 640;
constexpr int WindHeight= 480;
constexpr int BlockLen = 32;  // block area size (32x32)
constexpr int MapWidth = 640 + 32;  // Window Size X
constexpr int MapHeight = 480;  // Window Size Y
constexpr int XBlock = MapWidth / BlockLen;  // horizon 20
constexpr int YBlock = MapHeight / BlockLen; // vertical 15

constexpr int CharKindMax = 4;  // number of character kind (main + city character)
constexpr int CompCharMax = 8;  // number of city character
constexpr int ImgCharDivNum = 12;  // number of character parts
constexpr int DivisionNum = 4;
constexpr int AtbCntMax = 240;
constexpr int ImgStartBattleDivNum = 16;

constexpr int MessageLenMax = 128;

constexpr int MainCharIndex  = 0;

enum class PassFlag
{
	Through,
	MainCharOnly,
	NoThrough,	
};

enum class Direction
{
	Down = 0,
	Left = 1,
	Right = 2,
	Up = 3,
};

#ifdef _DEBUG
	#define DBGPRINTNUM(VALUE) \
	{ \
		DxLib::printfDx("[DEBUG][Line %05d][Func %s]\n\t>> %s [%d]\n", __LINE__, __FUNCTION__, #VALUE, VALUE); \
	}
	#define DBGPRINTSTR(VALUE) \
	{ \
		DxLib::printfDx("[DEBUG][Line %05d][Func %s]\n\t>>%s [%s]\n", __LINE__, __FUNCTION__, #VALUE, VALUE); \
	}
#else
	#define DBGPRINTNUM(VALUE)
	#define DBGPRINTSTR(VALUE)
#endif