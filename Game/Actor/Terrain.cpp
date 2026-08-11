#include "Terrain.h"

using namespace Platformer;

Terrain::Terrain(const std::string& image, const Platformer::Vector2& position, Platformer::Color color) : super(image, position, color)
{
	// block 처리 해야 함으로 Player와 동일하게
	sortingOrder = 4;
}
