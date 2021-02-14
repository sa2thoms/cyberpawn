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