#pragma once
#include <Level\Level.h>

class Player;

class GameLevel : public Platformer::Level
{
	TYPE_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	virtual ~GameLevel() = default;


	virtual Platformer::Actor* GetActorAt(const Platformer::Vector2& nextPosition);
	virtual Platformer::Actor* GetActorByRequestList(const Platformer::Vector2& nextPosition);
	virtual bool CanMove(const Platformer::Actor* other, Platformer::Color color);
	virtual void HandleInteraction(Platformer::Actor* target, const Platformer::Vector2& direction);

	inline bool CheckValidXPos(const int posX) const { return !(posX < 0 || screenWidth < 0); }
	inline bool CheckValidYPos(const int posY) const { return !(posY < 0 || posY > levelHeight); }
	virtual void ChangeActorColors();

protected:
	virtual void LoadMap();

protected:
	int levelHeight = 0;
	int screenWidth = 0;

	std::shared_ptr<Player> player;
};

