#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main(int argc, char* argv[])
{
	std::cout << "Game!" << std::endl;

	Game game("./Config.txt");
	game.run();

	return 0;
}