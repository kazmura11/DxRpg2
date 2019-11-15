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
const double PI = 3.14159265358979;
const int WindWidth = 640;
const int WindHeight= 480;
const int BlockLen = 32;  // block area size (32x32)
const int MapWidth = 640 + 32;  // Window Size X
const int MapHeight = 480;  // Window Size Y
const int XBlock = MapWidth / BlockLen;  // horizon 20
const int YBlock = MapHeight / BlockLen; // vertical 15

const int CharKindMax = 4;  // number of character kind (main + city character)
const int CompCharMax = 8;  // number of city character
const int ImgCharDivNum = 12;  // number of character parts
const int DivisionNum = 4;
const int AtbCntMax = 240;
const int ImgStartBattleDivNum = 16;

const int MessageLenMax = 128;

const int MainCharIndex  = 0;

enum PassFlag {
	Through = 0,
	MainCharOnly = 1,
	NoThrough = 2	
};

enum Direction
{
	Down = 0,
	Left = 1,
	Right = 2,
	Up = 3
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