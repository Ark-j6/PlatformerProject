#include "Player.h"
#include <Input/Input.h>
#include <Level/TestLevel.h>

using namespace Platformer;

Player::Player(const Vector2& position) : super("P", position, Color::Green)
{
	sortingOrder = 3;
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
}

void Player::ChangeColor()
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
	case Color::BrightWhite:
	default:
		break;
	}
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

	// 이동 처리
	if (Input::Get().GetKey(VK_RIGHT))
	{
		Move(1.f, deltaTime);
	}

	if (Input::Get().GetKey(VK_LEFT))
	{
		Move(-1.f, deltaTime);
	}

	// 점프
	if (Input::Get().GetKeyDown(VK_SPACE) && !isFirstJumping)
	{
		isFirstJumping = true;
		velocityY -= jumpSpeed;
		RequestChangeColor();
	}
	      
	velocityY += gravity * deltaTime;
	if (velocityY > 30.f)
	{
		velocityY = 30.f;
	}
	float d = posY + velocityY * deltaTime;

	std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner());
	Vector2 newPosition = GetPosition();
	newPosition.y = static_cast<int>(d);
	if (level && level->CanMove(newPosition, color))
	{
		posY = d;
		SetPosition(newPosition);
	}
	else
	{
		isFirstJumping = false;
		isJumping = false;
		velocityY = 0;
	}
}


void Player::Move(float direction, float deltaTime)
{
	int curr = static_cast<int>(posX);
	float amount = direction * moveSpeed * deltaTime;
	posX += amount;
	int next = static_cast<int>(posX);

	if (next != curr)
	{
		std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner());
		Vector2 newPosition = GetPosition();
		newPosition.x = next;
		if (level && level->CanMove(newPosition, color))
		{
			SetPosition(newPosition);
			CheckCameraView();
		}
		else
		{
			posX -= amount;
		}
	}
}

void Player::CheckCameraView()
{
	if (std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner()))
	{
		level->CheckPlayerXPos(GetPosition().x);
	}
}

void Player::RequestChangeColor()
{
	if (std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner()))
	{
		level->ChangeActorColors();
	}
}
