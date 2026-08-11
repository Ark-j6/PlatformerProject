#include "Platform.h"

using namespace Platformer;

Platform::Platform(const Vector2& position, Color color, bool isChangable, bool isMovable) : Terrain("-", position, color), isColorChangable(isChangable), isMovable(isMovable)
{
	sortingOrder = 3;
}

void Platform::ChangeColor()
{
	if (isColorChangable)
	{
		switch (color)
		{
		case Color::White:
			color = Color::Red;
			break;
		case Color::Red:
			color = Color::Green;
			break;
		case Color::Green:
			color = Color::Blue;
			break;
		case Color::Blue:
			color = Color::White;
			break;
		default:
			break;
		}
	}
}
