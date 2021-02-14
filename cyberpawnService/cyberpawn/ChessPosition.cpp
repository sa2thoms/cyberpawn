#include "cyberpawn/ChessPosition.hpp"

#include <optional>

namespace cyberpawn {

	void ChessPosition::setUpStandardChessGame() {
		this->setBoardToEmpty();

		board_[0][0] = PieceCode::whiteRook;
        board_[1][0] = PieceCode::whiteKnight;
        board_[2][0] = PieceCode::whiteBishop;
        board_[3][0] = PieceCode::whiteQueen;
        board_[4][0] = PieceCode::whiteKing;
        board_[5][0] = PieceCode::whiteBishop;
        board_[6][0] = PieceCode::whiteKnight;
        board_[7][0] = PieceCode::whiteRook;

		board_[0][1] = PieceCode::whitePawn;
        board_[1][1] = PieceCode::whitePawn;
        board_[2][1] = PieceCode::whitePawn;
        board_[3][1] = PieceCode::whitePawn;
        board_[4][1] = PieceCode::whitePawn;
        board_[5][1] = PieceCode::whitePawn;
        board_[6][1] = PieceCode::whitePawn;
        board_[7][1] = PieceCode::whitePawn;

		board_[0][6] = PieceCode::blackPawn;
        board_[1][6] = PieceCode::blackPawn;
        board_[2][6] = PieceCode::blackPawn;
        board_[3][6] = PieceCode::blackPawn;
        board_[4][6] = PieceCode::blackPawn;
        board_[5][6] = PieceCode::blackPawn;
        board_[6][6] = PieceCode::blackPawn;
        board_[7][6] = PieceCode::blackPawn;

		board_[0][7] = PieceCode::blackRook;
        board_[1][7] = PieceCode::blackKnight;
        board_[2][7] = PieceCode::blackBishop;
        board_[3][7] = PieceCode::blackQueen;
        board_[4][7] = PieceCode::blackKing;
        board_[5][7] = PieceCode::blackBishop;
        board_[6][7] = PieceCode::blackKnight;
        board_[7][7] = PieceCode::blackRook;

		fileInWhichAPawnMovedTwoSquaresThePreviousMove_ = std::nullopt;
		whiteMayStillCastleKingside_ = true;
		blackMayStillCastleKingside_ = true;
		whiteMayStillCastleQueenside_ = true;
		blackMayStillCastleQueenside_ = true;

		turn_ = Color::White;
	}

	bool withinBoard(ChessSquare square) {
		if (square.file > 7 || square.rank > 7 || square.file < 0 || square.rank < 0) {
			return false;
		}
		else {
			return true;
		}
	}

	bool ChessPosition::isSquareAttacked(ChessSquare square) const {
		// Attacks can come from a diagonal, a straight line, or a knight attack.
		// Note that kings and pawns still attack along a diagonal or straight line.

		Color opposingColor = (turn_ == Color::White) ? Color::Black : Color::White;
		
		int8_t colorMask = (opposingColor == Color::White) ? 0b00000000 : 0b00010000;

		// direction of opposing pawns
		int8_t pawnForwardDirection = (opposingColor == Color::White) ? 1 : -1;

		std::vector< std::pair<ChessSquare, std::vector< std::pair <PieceCode, bool> > > > directionsOfAttack = {
			{
				{-1, -1},
				{
					{PieceCode::whiteBishop | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{-1, 0},
				{
					{PieceCode::whiteRook | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{-1, 1},
				{
					{PieceCode::whiteBishop | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{0, -1},
				{
					{PieceCode::whiteRook | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{0, 1},
				{
					{PieceCode::whiteRook | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{1, -1},
				{
					{PieceCode::whiteBishop | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{1, 0},
				{
					{PieceCode::whiteRook | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{1, 1},
				{
					{PieceCode::whiteBishop | colorMask, true},
					{PieceCode::whiteQueen | colorMask, true},
					{PieceCode::whiteKing | colorMask, false}
				}
			},
			{
				{-2, -1},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{-2, 1},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{2, -1},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{2, 1},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{-1, -2},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{-1, 2},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{1, -2},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			},
			{
				{1, 2},
				{
					{PieceCode::whiteKnight | colorMask, false}
				}
			}
		};

		for (auto directionData : directionsOfAttack) {
			ChessSquare direction = directionData.first;

			if (std::abs(direction.file) == 1 && std::abs(direction.rank) == 1) {
				// if the direction is diagonal, it may be a pawn attack
				if (pawnForwardDirection == -direction.rank) {
					directionData.second.push_back({ PieceCode::whitePawn | colorMask, false });
				}
			}

			ChessSquare square_to_check = square + direction;
			bool firstStep = true;
			while (withinBoard(square_to_check)) {
				if (this->operator[](square_to_check) == PieceCode::noPiece) {
					firstStep = false;
                    square_to_check = square_to_check + direction;
					continue;
				}
				else if (this->operator[](square_to_check).getColor() == opposingColor) {

					for (auto capablePiece : directionData.second) {
						if ((firstStep || capablePiece.second) && capablePiece.first == this->operator[](square_to_check)) {
							return true;
						}
					}

					break;
				}
				else {
					break;
				}
			}
		}

		// all attack vectors have been checked, so we can safely say the square is not under attack.
		return false;
	}

    bool ChessPosition::isKingAttacked() const {
        PieceCode kingInQuestion = (turn_ == Color::White) ? PieceCode::whiteKing : PieceCode::blackKing;
        std::optional<ChessSquare> positionOfKing;
        for (int8_t f = 0; f < board_.size(); f++) {
            for (int8_t r = 0; r < 8; r++) {
                if (board_[f][r] == kingInQuestion) {
                    positionOfKing = ChessSquare{ f, r };
                }
            }
        }
        if (positionOfKing) {
            return isSquareAttacked(positionOfKing.value());
        }
        else {
            return false;
        }
    }

}
