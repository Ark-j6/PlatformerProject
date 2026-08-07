#include "Platform.h"

using namespace Platformer;

Platform::Platform(const Vector2& position) : super("-", position)
{
	sortingOrder = 3;
}
