#include "cyberpawn/ChessGameFunctions.hpp"


namespace cyberpawn {
	// checks if the piece can move there under normal circumstances (e.g. it's along a bishop's diagonal)
	bool isGeometricallyLegalMove(const ChessPosition & position, const ChessMove & move) {
		ChessSquare from = move.from;
		ChessSquare to = move.to;

		PieceCode pieceToMove = position[from.file][from.rank];

		bool isValidSoFar = position[from.file][from.rank] != PieceCode::noPiece
			&& position[from.file][from.rank].getColor() == position.getTurn();

		if (pieceToMove.isPawn()) {

		}
	}
	bool isLegalMove(const ChessPosition & position, const ChessMove & move) {


	}
}


