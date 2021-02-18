#ifndef CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED

#include "cyberpawn/ChessPosition.hpp"
#include "cyberpawn/ChessGame.hpp"
#include "cyberpawn/BetaTreeEngine.hpp"

#include <optional>
#include <string>

namespace cyberpawn {

	// A state machine that takes cli commands as inputs
	class CyberpawnCli {
	private:
		BetaTreeEngine beta_engine_;
		std::optional<ChessPosition> interactive_game_;
		bool confirming_exit = false;
	public:
		CyberpawnCli() {
			beta_engine_.setSearchDepth(6);
			beta_engine_.setAllowMultipleThreads(true);
		}

		// Executes the command, returning true if the cli should stay open for another command
		bool execute(const std::string & command);

	private:

		bool executeStartCommand(const std::vector<std::string> & tokens);
		bool executeSetDepthCommand(const std::string & depth);
		bool executeSetCommand(const std::vector<std::string> & tokens);
	};


}



#endif // CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED
