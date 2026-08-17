#include <Game/GameManager.h>
#include <Windows.h>

int main()
{
	SetConsoleTitleA("COLOR FORMER");

	GameManager manager;
	manager.Run();
}