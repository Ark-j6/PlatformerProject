#pragma once

#include <Core/Core.h>
#include <Windows.h>

namespace Platformer
{
	// 색상을 열거형으로 정의
	enum class PLATFORMER_API Color : WORD
	{
		Red = FOREGROUND_RED,
		Green = FOREGROUND_GREEN,
		Blue = FOREGROUND_BLUE,
		White = Red | Green | Blue,
		Yellow = Red | Green,
		Cyan = Green | Blue,
		Purple = Red | Blue,
		BrightWhite = White | FOREGROUND_INTENSITY
	};
}