#include <iostream>
#include <string>

#include "CyberpawnCli.hpp"

int main() {

	bool continue_looping = false;

	CyberpawnCli cli;

	while (continue_looping) {
		std::string command_received;
		std::cin >> command_received;

		continue_looping = cli.execute(command_received);
	}

	return 0;
}
