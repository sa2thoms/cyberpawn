#ifndef CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED


#include "ChessPosition.hpp"
#include "ChessMove.hpp"

#include <optional>

namespace cyberpawn {

    bool withinBoard(ChessSquare square);

    // if the move is legal, a new position with the move made is returned. If the
    // move is illegal, then std::nullopt is returned.
    std::optional<ChessPosition> makeMoveIfLegal(const ChessPosition & position, const ChessMove & move);


    // Returns a vector of moves that may or may not be legal
    std::vector<ChessMove> collectPotentialMoves(const ChessPosition & position);

    // Returns the exact set of legal moves
    std::vector<ChessMove> allLegalMoves(const ChessPosition & position);
}


#endif // CYBERPAWN_CYBERPAWN_CHESS_GAME_FUNCTIONS_INCLUDED
