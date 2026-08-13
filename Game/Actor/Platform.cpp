#include "Platform.h"

using namespace Platformer;

Platform::Platform(const Vector2& position, Color color, bool isChangable, bool isMovable) : Actor("-", position, color), isColorChangable(isChangable), isMovable(isMovable)
{
	sortingOrder = 3;
}

bool Platform::CheckActorPosition(const Vector2& position)
{
	int maxX = GetPosition().x + width;
	bool isX = GetPosition().x <= position.x && position.x <= maxX;

	return isX && position.y == GetPosition().y;
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
