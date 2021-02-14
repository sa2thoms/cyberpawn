#ifndef CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED


#include "ChessPosition.hpp"
#include "ChessMove.hpp"

#include <optional>

namespace cyberpawn {
    std::optional<ChessPosition> makeMoveIfLegal(const ChessPosition & position, const ChessMove & move);
}


#endif // CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
