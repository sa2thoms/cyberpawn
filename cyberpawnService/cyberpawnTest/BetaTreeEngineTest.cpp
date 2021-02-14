#include "pch.h"

#include "cyberpawn/BetaTreeEngine.hpp"
#include "cyberpawn/ChessGameFunctions.hpp"

using namespace cyberpawn;

TEST(BetaTreeEngineTest, finds20PossibleOpeningMovesForWhite) {
	BetaTreeEngine engine(1);

	ChessPosition position;

	std::vector<ChessMove> bestMoves = engine.findBestMoves(position, 100);

	EXPECT_EQ(bestMoves.size(), 20);
}

TEST(BetaTreeEngineTest, recognizesPuttingTheOpponentInCheckmateAsBeingTheBestMove) {
	BetaTreeEngine engine(2);

	ChessPosition position;
    std::vector<ChessMove> legalMoves = {
    { {4, 1}, {4, 3} },
    { {5, 6}, {5, 4} },
    { {0, 1}, {0, 2} },
    { {6, 6}, {6, 4} }
    };

    for (const auto & move : legalMoves) {
        auto newPosition = makeMoveIfLegal(position, move);
        position = newPosition.value();
    }

    std::vector<ChessMove> bestMoves = engine.findBestMoves(position, 10);

    EXPECT_EQ(bestMoves[0], (ChessMove{ { 3, 0 }, { 7, 4 } }));
}
