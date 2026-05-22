#include "pch.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "OiiAiGame.h"


int main()
{
	OiiAGame game;
	if (false == game.Initialize())
	{
		std::cout << "Failed to initialize the game. Error: " << GetLastError() << std::endl;
		return -1;
	}

	game.Run();

	game.Finalize();

	return 0;

}




















