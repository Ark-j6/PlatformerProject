#include "Key.h"

using namespace Platformer;

Key::Key(const Vector2& position) : Actor("K", position, Color::Yellow)
{
	sortingOrder = 3;
}
