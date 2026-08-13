#pragma once
#include <Actor\Terrain.h>

class Platform : public Platformer::Actor
{
	TYPE_DECLARATIONS(Platform, Platformer::Actor)

public:
	Platform(const Platformer::Vector2& position, Platformer::Color color, bool isChangable = false, bool isMovable = false);

	virtual bool CheckActorPosition(const Platformer::Vector2& position) override;

	inline void ChangeWidth(unsigned short newWidth)
	{
		width = newWidth;
		image = "";
		for (int i = 0; i < newWidth; ++i)
		{
			image += '-';
		}
	}

	virtual void ChangeColor() override;

	void Moving();

public:
	bool isColorChangable = false;
	bool isMovable = false;
};

