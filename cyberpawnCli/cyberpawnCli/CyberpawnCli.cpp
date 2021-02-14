#include "CyberpawnCli.hpp"
#include "StringUtilities.hpp"
#include "cyberpawn/ChessGameFunctions.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

namespace cyberpawn {
	bool CyberpawnCli::executeStartCommand(const std::vector<std::string> & tokens) {

		if (tokens.size() == 3 && tokens[1] == "interactive" && tokens[2] == "game") {
			interactive_game_ = ChessPosition();
			return true;
		}

		std::cout << "Unkown start command. What are you trying to start?" << std::endl;
		return true;
	}

	bool isAffirmative(const std::string & s) {
		const std::vector<std::string> affirmatives = {
			"yes",
			"y",
			"ye",
			"yeah",
			"yup",
			"yuppers",
			"absolutely"
		};
		return std::any_of(affirmatives.begin(), affirmatives.end(), [&s](const std::string & aff) -> bool { return s == aff; });
	}


	bool CyberpawnCli::execute(const std::string & command) {
		std::vector<std::string> tokens = string_utilities::split(command, ' ');

		for (auto & token : tokens) {
			token = string_utilities::toLowerCase(token);
		}

		if (tokens.empty()) {
			return true;
		}

		if (confirming_exit) {
			if ((tokens[0] == "exit" || tokens[0] == "quit" || isAffirmative(tokens[0])) && tokens.size() == 1) {
				return false;
			}
			else {
				confirming_exit = false;
				return true;
			}
		}

		// exit command
		if ((tokens[0] == "exit" || tokens[0] == "quit") && tokens.size() == 1) {
			confirming_exit = true;
			std::cout << "Are you sure you want to quit?" << std::endl;
			return true;
		}

		if (tokens[0] == "start") {
			return executeStartCommand(tokens);
		}

		if (interactive_game_) {
			if (tokens.size() == 3 && tokens[1] == "to") {
				auto from = squareFromString(tokens[0]);
				auto to = squareFromString(tokens[2]);
				if (!from || !to) {
					std::cout << "We think you are trying to make a move, but the syntax is incorrect. Try, for example \"E2 to E4\"." << std::endl;
					return true;
				}
				ChessMove yourMove = { from.value(), to.value() };
				auto newPosition = makeMoveIfLegal(interactive_game_.value(), yourMove);
				if (newPosition) {
					std::cout << "Your move: " << moveToString(yourMove) << std::endl;

					std::vector<ChessMove> engineMoves = beta_engine_.findBestMoves(newPosition.value(), 1);

					if (engineMoves.empty()) {
						std::cout << "Cyberpawn could not find any legal moves! Is it checkmate or stalemate or is cyberpawn broken?" << std::endl;
						return true;
					}

					auto secondNewPosition = makeMoveIfLegal(newPosition.value(), engineMoves[0]);
					if (secondNewPosition) {
						std::cout << "Cyberpawn responded with: " << moveToString(engineMoves[0]) << std::endl;
						interactive_game_ = secondNewPosition;
						return true;
					}
					else {
						std::cout << "Cyberpawn tried to make an illegal move (" << moveToString(engineMoves[0]) << ") and should probably see a doctor." << std::endl;
						return true;
					}
				}
				else {
					std::cout << "That move is illegal. Try again." << std::endl;
					return true;
				}
			}
		}

		std::cout << "Unknown Command." << std::endl;
		return true;
	}
}

