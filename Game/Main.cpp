#include <Engine/Engine.h>
#include <Level/TestLevel.h>

int main()
{
	Platformer::Engine engine;
	engine.AddNewLevel<TestLevel>();
	engine.Run();
}