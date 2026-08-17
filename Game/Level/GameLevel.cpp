#include "GameLevel.h"
#include <Actor/Player.h>
#include <Actor/Terrain.h>
#include <Actor/Platform.h>
#include <Actor/Obstacle.h>
#include <Actor/Key.h>
#include <Actor/Door.h>
//#include <Actor/CheckPoint.h>

#include <cassert>


using namespace Platformer;

GameLevel::GameLevel()
{
}

Actor* GameLevel::GetActorAt(const Vector2& nextPosition)
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		if (actor->CheckActorPosition(nextPosition))
		{
			if (actor->IsTypeOf<Player>())
			{
				continue;
			}

			return actor.get();
		}
	}

	return nullptr;
}

Actor* GameLevel::GetActorByRequestList(const Vector2& nextPosition)
{
	for (std::shared_ptr<Actor>& actor : addRequestedActorList)
	{
		if (actor->CheckActorPosition(nextPosition))
		{
			if (actor->IsTypeOf<Player>())
			{
				continue;
			}

			return actor.get();
		}
	}

	return nullptr;
}

bool GameLevel::CanMove(const Actor* other, Color color)
{
	if (other)
	{
		if (other->IsTypeOf<Terrain>() || other->IsTypeOf<Obstacle>())
		{
			return false;
		}
		if (other->IsTypeOf<Key>() || other->IsTypeOf<Door>())
		{
			return true;
		}
		else if (other->IsTypeOf<Platform>() && other->GetColor() != color)
		{
			return false;
		}
	}

	return true;
}

void GameLevel::HandleInteraction(Actor* target, const Vector2& direction)
{
	assert(target && "No Actor for interaction");

	if (target->IsTypeOf<Platform>())
	{
		player->UpdateStandingPlatform(dynamic_cast<Platform*>(target));
	}
}

void GameLevel::ChangeActorColors()
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		actor->ChangeColor();
	}
}

void GameLevel::LoadMap()
{}
