#include <iostream>
#include <string>

#include "CyberpawnCli.hpp"

int main() {

	bool continue_looping = true;

	cyberpawn::CyberpawnCli cli;

	while (continue_looping) {
		std::string command_received;
		std::getline(std::cin, command_received);

		continue_looping = cli.execute(command_received);
	}

	return 0;
}
