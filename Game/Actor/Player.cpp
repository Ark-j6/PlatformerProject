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

	if (!enableInput)
		return;

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
	if (Input::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		isJumping = true;
		velocityY -= jumpSpeed;
		RequestChangeColor();
	}
	      
	velocityY += gravity * deltaTime;
	if (velocityY > 30.f)
	{
		velocityY = 30.f;
	}
	float amount = posY + velocityY * deltaTime;
	int next = static_cast<int>(amount);
	RequestMove(false, next, amount);
}


void Player::Move(float direction, float deltaTime)
{
	int curr = static_cast<int>(posX);
	float amount = direction * moveSpeed * deltaTime;
	posX += amount;
	int next = static_cast<int>(posX);

	if (next != curr)
	{
		RequestMove(true, next, amount);
	}
}

void Player::RequestMove(bool isX, const int next, const float amount)
{
	if (std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		isX ? newPosition.x = next : newPosition.y = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				//isX ? posX += amount : posY += amount;
				if (!isX) posY = amount;
				SetPosition(newPosition);
			}
			else
			{
				if (isX)
				{ 
					posX -= amount;
				}
				else
				{
					isJumping = false;
					velocityY = 0;
				}
			}

			// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
			Vector2 direction = newPosition - GetPosition();
			level->HandleInteraction(actor, direction);
		}
		else
		{
			// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
 			if (isX ? level->CheckValidXPos(next) : level->CheckValidYPos(next))
			{
				//isX ? posX += amount : posY += amount;
				if (!isX) posY = amount;
				SetPosition(newPosition);
			}
			else
			{
				if (isX)
				{
					posX -= amount;
				}
				else
				{
					isJumping = false;
					velocityY = 0;
				}
			}
		}
	}
}

void Player::RequestChangeColor()
{
	if (std::shared_ptr<TestLevel> level = Cast<TestLevel>(GetOwner()))
	{
		level->ChangeActorColors();
	}
}
