#ifndef CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED


#include "ChessPosition.hpp"
#include "ChessMove.hpp"

namespace cyberpawn {
	bool isLegalMove(const ChessPosition & position, const ChessMove & move);
}


#endif // CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
