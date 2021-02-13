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
		whiteMayStillCastle_ = true;
		blackMayStillCastle_ = true;
	}

}
