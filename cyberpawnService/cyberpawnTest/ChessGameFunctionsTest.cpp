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

TEST(ChessGameFunctionsTest, pawnCannotTakeForward) {
    ChessPosition position;
    std::vector<ChessMove> moves = {
        { {4, 1}, {4, 3} },
        { {4, 6}, {4, 4} },
        { {4, 3}, {4, 4} }
    };

    
    auto newPosition = makeMoveIfLegal(position, moves[0]);
    EXPECT_TRUE(newPosition.has_value());
    newPosition = makeMoveIfLegal(newPosition.value(), moves[1]);
    EXPECT_TRUE(newPosition.has_value());
    position = newPosition.value();
    newPosition = makeMoveIfLegal(newPosition.value(), moves[2]);
    EXPECT_FALSE(newPosition.has_value());
    

    EXPECT_EQ(position[(ChessSquare{ 4, 4 })], PieceCode::blackPawn);
    EXPECT_EQ(position[(ChessSquare{ 4, 3 })], PieceCode::whitePawn);
}

TEST(ChessGameFunctionsTest, pawnCanTakeEnPassant) {
    ChessPosition position;
    std::vector<ChessMove> moves = {
        { {4, 1}, {4, 3} },
        { {3, 6}, {3, 4} },
        { {4, 3}, {4, 4} },
        { {5, 6}, {5, 4} },
        { {4, 4}, {5, 5} }
    };

    for (const auto & move : moves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    EXPECT_EQ(position[(ChessSquare{ 4, 4 })], PieceCode::noPiece);
    EXPECT_EQ(position[(ChessSquare{ 5, 5 })], PieceCode::whitePawn);
    EXPECT_EQ(position[(ChessSquare{ 5, 4 })], PieceCode::noPiece);
}

TEST(ChessGameFunctionsTest, pawnCannotTakeEnPassantDelayed) {
    ChessPosition position;
    std::vector<ChessMove> legalMoves = {
        { {4, 1}, {4, 3} },
        { {3, 6}, {3, 4} },
        { {4, 3}, {4, 4} },
        { {5, 6}, {5, 4} }
    };

    ChessMove illegalMove = { {4, 4}, {3, 5} };

    for (const auto & move : legalMoves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    auto newPosition = makeMoveIfLegal(position, illegalMove);
    EXPECT_FALSE(newPosition.has_value());

    EXPECT_EQ(position[(ChessSquare{ 4, 4 })], PieceCode::whitePawn);
    EXPECT_EQ(position[(ChessSquare{ 3, 4 })], PieceCode::blackPawn);
    EXPECT_EQ(position[(ChessSquare{ 3, 5 })], PieceCode::noPiece);
}

TEST(ChessGameFunctionsTest, blackCannotMoveLeavingHisKingInCheck) {
    ChessPosition position;
    std::vector<ChessMove> legalMoves = {
        { {4, 1}, {4, 3} },
        { {5, 6}, {5, 4} },
        { {3, 0}, {7, 4} }
    };

    ChessMove illegalMove = { {0, 6}, {0, 5} };

    for (const auto & move : legalMoves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    auto newPosition = makeMoveIfLegal(position, illegalMove);
    EXPECT_FALSE(newPosition.has_value());

    EXPECT_EQ(position[(ChessSquare{ 3, 0 })], PieceCode::noPiece);
    EXPECT_EQ(position[(ChessSquare{ 7, 4 })], PieceCode::whiteQueen);
}

TEST(ChessGameFunctionsTest, canTakeAnotherPieceWithAPawnWhenEnPassantIsAvailable) {
    ChessPosition position;
    std::vector<ChessMove> legalMoves = {
        { {4, 1}, {4, 3} },
        { {3, 6}, {3, 4} },
        { {4, 3}, {3, 4} },
        { {0, 6}, {0, 5} },
        { {7, 1}, {7, 2} },
        { {4, 6}, {4, 5} },
        { {3, 1}, {3, 3} },
        { {4, 5}, {3, 4} }
    };

    for (const auto & move : legalMoves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    EXPECT_EQ(position[(ChessSquare{ 3, 3 })], PieceCode::whitePawn);
    EXPECT_EQ(position[(ChessSquare{ 3, 4 })], PieceCode::blackPawn);
}

TEST(ChessGameFunctionsTest, canPromotePawnToAQueen) {
    ChessPosition position;
    std::vector<ChessMove> legalMoves = {
        { {6, 1}, {6, 3} },
        { {0, 6}, {0, 5} },
        { {6, 3}, {6, 4} },
        { {0, 5}, {0, 4} },
        { {6, 4}, {6, 5} },
        { {0, 4}, {0, 3} },
        { {6, 5}, {7, 6} },
        { {0, 3}, {0, 2} },
        { {7, 6}, {6, 7} }
    };

    for (const auto & move : legalMoves) {
        auto newPosition = makeMoveIfLegal(position, move);
        EXPECT_TRUE(newPosition.has_value());
        position = newPosition.value();
    }

    EXPECT_EQ(position[(ChessSquare{ 6, 7 })], PieceCode::whiteQueen);
}

