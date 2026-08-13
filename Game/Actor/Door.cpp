#include "Door.h"
#include <Render/Renderer.h>

using namespace Platformer;

Door::Door(const Vector2& position) : Actor("M",  position, Color::Purple), height(3)
{
    sortingOrder = 1;
}

void Door::Draw(int screenX)
{
	// 비활성 상태이면 종료
	if (!IsActive())
	{
		return;
	}

	// 렌더러에 필요한 데이터 제출
	Vector2 screenPos = position;
	screenPos.x -= screenX;
	Renderer::Get().Submit(image, screenPos - Vector2(-1, -1), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(1, -1), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(-1, 0), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(1, 0), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(-1, 1), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(0, 1), color, sortingOrder);
	Renderer::Get().Submit(image, screenPos - Vector2(1, 1), color, sortingOrder);
}

bool Door::CheckActorPosition(const Vector2& position)
{
	Vector2 under = GetPosition();
	under.y -= 1;
    return (GetPosition() == position) || (under == position);
}
