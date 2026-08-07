#include "Player.h"
#include <Input/Input.h>
#include <Level/TestLevel.h>

using namespace Platformer;

Player::Player(const Vector2& position) : super("P", position, Color::Green)
{
	sortingOrder = 3;
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 종료 처리
		QuitGame();
		return;
	}

	// 이동 처리를 위해 GameLevel 객체 얻어오기
	std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner());

	// 이동 처리
	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		// 이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

		// 이동 가능 여부 확인
		if (level && level->CanMove(newPosition))
		{
			//새로운 위치 설정
			SetPosition(newPosition);
		}
	}
	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		// 이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

		// 이동 가능 여부 확인
		if (level && level->CanMove(newPosition))
		{
			//새로운 위치 설정
			SetPosition(newPosition);
		}
	}

	if (isActive)
	{
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		if (level && level->CanMove(newPosition))
		{
			//새로운 위치 설정
			SetPosition(newPosition);
		}
	}
}
