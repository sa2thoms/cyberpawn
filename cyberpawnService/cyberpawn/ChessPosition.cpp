#include "cyberpawn/ChessPosition.hpp"


namespace cyberpawn {

	void ChessPosition::setUpStandardChessGame() {
		this->setBoardToEmpty();

		board_[0][0] = PieceCode::whiteRook;
		board_[0][1] = PieceCode::whiteKnight;
		board_[0][2] = PieceCode::whiteBishop;
		board_[0][3] = PieceCode::whiteKing;
		board_[0][4] = PieceCode::whiteQueen;
		board_[0][5] = PieceCode::whiteBishop;
		board_[0][6] = PieceCode::whiteKnight;
		board_[0][7] = PieceCode::whiteRook;

		board_[1][0] = PieceCode::whitePawn;
		board_[1][1] = PieceCode::whitePawn;
		board_[1][2] = PieceCode::whitePawn;
		board_[1][3] = PieceCode::whitePawn;
		board_[1][4] = PieceCode::whitePawn;
		board_[1][5] = PieceCode::whitePawn;
		board_[1][6] = PieceCode::whitePawn;
		board_[1][7] = PieceCode::whitePawn;

		board_[6][0] = PieceCode::blackPawn;
		board_[6][1] = PieceCode::blackPawn;
		board_[6][2] = PieceCode::blackPawn;
		board_[6][3] = PieceCode::blackPawn;
		board_[6][4] = PieceCode::blackPawn;
		board_[6][5] = PieceCode::blackPawn;
		board_[6][6] = PieceCode::blackPawn;
		board_[6][7] = PieceCode::blackPawn;

		board_[7][0] = PieceCode::blackRook;
		board_[7][1] = PieceCode::blackKnight;
		board_[7][2] = PieceCode::blackBishop;
		board_[7][3] = PieceCode::blackKing;
		board_[7][4] = PieceCode::blackQueen;
		board_[7][5] = PieceCode::blackBishop;
		board_[7][6] = PieceCode::blackKnight;
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

}
