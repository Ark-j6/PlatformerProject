#include "Wall.h"

Wall::Wall(const Platformer::Vector2& position) : super("#", position)
{
	// block 처리 해야 함으로 Player와 동일하게
	sortingOrder = 3;
}
