#include "CheckPoint.h"

using namespace Platformer;

CheckPoint::CheckPoint(const Vector2& position) : Actor("C", position, Color::CheckPoint)
{
	sortingOrder = 1;
}
