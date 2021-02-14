#include "pch.h"

#include "cyberpawn/ChessGameFunctions.hpp"

using namespace cyberpawn;

TEST(ChessGameFunctionsTest, makeMoveIfLegalMakesE4OpeningMove) {
    ChessPosition position;
    ChessMove e4Move = { {4, 1}, {4, 3} };
    auto resultPosition = makeMoveIfLegal(position, e4Move);

    EXPECT_TRUE(resultPosition.has_value());
    EXPECT_EQ(resultPosition.value()[e4Move.from], PieceCode::noPiece);
    EXPECT_EQ(resultPosition.value()[e4Move.to], PieceCode::whitePawn);
}

TEST(ChessGameFunctionsTest, aSeriesOfPawnMovesWorksCorrectly) {
    ChessPosition position;
    std::vector<ChessMove> moves = {
        { {4, 1}, {4, 3} },
        { {4, 6}, {4, 5} },
        { {3, 1}, {3, 3} },
        { {3, 6}, {3, 4} }
    };

    for (const auto & move : moves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    EXPECT_EQ(position[(ChessSquare{ 4, 5 })], PieceCode::blackPawn);
    EXPECT_EQ(position[(ChessSquare{ 3, 3 })], PieceCode::whitePawn);
}
