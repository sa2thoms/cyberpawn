#include "cyberpawn/ChessGameFunctions.hpp"


namespace cyberpawn {

	namespace math {
		template <typename T>
		T square(T a) { return a * a; }
	}


    bool withinBoard(ChessSquare square) {
        if (square.file > 7 || square.rank > 7 || square.file < 0 || square.rank < 0) {
            return false;
        }
        else {
            return true;
        }
    }

	// checks if the piece can move there under normal circumstances (e.g. it's along a bishop's diagonal)
	bool makeMoveIfGeometricallyLegal(ChessPosition & position, const ChessMove & move) {
		ChessSquare from = move.from;
		ChessSquare to = move.to;

		if (to == from) return false;
        if (!withinBoard(to)) return false;
        if (!withinBoard(from)) return false;

		PieceCode pieceToMove = position[from.file][from.rank];
		Color pieceColor = pieceToMove.getColor();
		
		bool isValidSoFar = pieceToMove != PieceCode::noPiece
			&& pieceColor == position.getTurn()
			&& (pieceColor != position[to].getColor() || position[to] == PieceCode::noPiece);
		if (!isValidSoFar) { return false; }


		if (pieceToMove.isPawn()) {
			int8_t forwardDirection = pieceColor == Color::White ? 1 : -1;
			int8_t pawnStartRank = pieceColor == Color::White ? 1 : 6;
			if ((to.rank - from.rank) == (forwardDirection * 2)) {
				if (from.rank != pawnStartRank) {
					return false;
				}
				if (to.file != from.file) {
					return false;
				}
				if (position[from.file][from.rank + forwardDirection] != PieceCode::noPiece) {
					return false;
				}
				if (position[to.file][to.rank] != PieceCode::noPiece) {
					return false;
				}
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(to.file);
				return true;
			}
			else if ((to.rank - from.rank) == forwardDirection) {
				if (to.file == from.file) {
					if (position[to.file][to.rank] != PieceCode::noPiece) {
						return false;
					}
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
					return true;
				}
				else if (std::abs(to.file - from.file) == 1) {
					auto enPassantPossibleFile = position.fileInWhichAPawnMovedTwoSquaresThePreviousMove();
					if (enPassantPossibleFile) {
						int8_t file_to_take_en_passant = enPassantPossibleFile.value();
						if (to.file != file_to_take_en_passant) {
							return false;
						}
						else if (to.rank != pawnStartRank + forwardDirection * 4) {
							return false;
						}
                        else {
                            position[to] = pieceToMove;
                            position[from] = PieceCode::noPiece;
                            position[{enPassantPossibleFile.value(), pawnStartRank + forwardDirection * 3}] = PieceCode::noPiece;
                            position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                            return true;
                        }
					}
					else if (position[to.file][to.rank] == PieceCode::noPiece) {
						return false;
					}
					if (position[to.file][to.rank].getColor() == position.getTurn()) {
						return false;
					}
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if (pieceToMove.isBishop()) {
			int8_t horizontalDisplacement = to.file - from.file;
			int8_t verticalDisplacement = to.rank - from.rank;
			if (horizontalDisplacement == 0 || verticalDisplacement == 0) {
				return false;
			}
			if (std::abs(verticalDisplacement) != std::abs(horizontalDisplacement)) {
				return false;
			}

			int8_t horizontalIncrement = horizontalDisplacement / std::abs(horizontalDisplacement);
			int8_t verticalIncrement = verticalDisplacement / std::abs(verticalDisplacement);

			ChessSquare increment = ChessSquare{ horizontalIncrement, verticalIncrement };

			ChessSquare square_to_check = from + increment;
			while (square_to_check != to) {
				if (position[square_to_check] != PieceCode::noPiece) {
					return false;
				}
				square_to_check = square_to_check + increment;
			}
			if (position[square_to_check] == PieceCode::noPiece) {
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
				return true;
			}
			else if (position[square_to_check].getColor() != position.getTurn()) {
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
				return true;
			}
			else {
				return false;
			}
		}
		else if (pieceToMove.isKnight()) {
			if (math::square(to.file - from.file) + math::square(to.rank - from.rank) == 5) {
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                return true;
			}
			else {
				return false;
			}
		}
		else if (pieceToMove.isRook()) {
			if (to.file == from.file) {
				// a vertical move
				int8_t displacement = to.rank - from.rank;
				int8_t increment = displacement / std::abs(displacement);

				ChessSquare square_to_check = from + ChessSquare{ 0, increment };
				while (square_to_check != to) {
					if (position[square_to_check] != PieceCode::noPiece) {
						return false;
					}
					square_to_check.rank += increment;
				}
				if (position[square_to_check] == PieceCode::noPiece) {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    if (from.file == 0) {
                        position.currentPlayerCannotCastleQueensideAnymore();
                    }
                    if (from.file == 7) {
                        position.currentPlayerCannotCastleKingsideAnymore();
                    }
					return true;
				}
				else if (position[square_to_check].getColor() != position.getTurn()) {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    if (from.file == 0) {
                        position.currentPlayerCannotCastleQueensideAnymore();
                    }
                    if (from.file == 7) {
                        position.currentPlayerCannotCastleKingsideAnymore();
                    }
					return true;
				}
				else return false;
			}
			else if (to.rank == from.rank) {
				// a horizontal move
				int8_t displacement = to.file - from.file;
				int8_t increment = displacement / std::abs(displacement);

				ChessSquare square_to_check = from + ChessSquare{ increment, 0};
				while (square_to_check != to) {
					if (position[square_to_check] != PieceCode::noPiece) {
						return false;
					}
					square_to_check.file += increment;
				}
				if (position[square_to_check] == PieceCode::noPiece) {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    if (from.file == 0) {
                        position.currentPlayerCannotCastleQueensideAnymore();
                    }
                    if (from.file == 7) {
                        position.currentPlayerCannotCastleKingsideAnymore();
                    }
					return true;
				}
				else if (position[square_to_check].getColor() != position.getTurn()) {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    if (from.file == 0) {
                        position.currentPlayerCannotCastleQueensideAnymore();
                    }
                    if (from.file == 7) {
                        position.currentPlayerCannotCastleKingsideAnymore();
                    }
					return true;
				}
				else return false;
			}
			else {
				return false;
			}
		}
		else if (pieceToMove.isQueen()) {
			int8_t horizontalDisplacement = to.file - from.file;
			int8_t verticalDisplacement = to.rank - from.rank;

			int8_t horizontalIncrement = horizontalDisplacement ? (horizontalDisplacement / std::abs(horizontalDisplacement)) : 0;
			int8_t verticalIncrement = verticalDisplacement ? (verticalDisplacement / std::abs(verticalDisplacement)) : 0;
			if (std::abs(horizontalDisplacement) != std::abs(verticalDisplacement) && verticalDisplacement != 0 && horizontalDisplacement != 0) {
				return false;
			}

			ChessSquare increment = { horizontalIncrement, verticalIncrement };
			ChessSquare square_to_check = from + increment;
			while (square_to_check != to) {
				if (position[square_to_check] != PieceCode::noPiece) {
					return false;
				}
				square_to_check = square_to_check + increment;
			}
			if (position[square_to_check] == PieceCode::noPiece) {
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
				return true;
			}
			else if (position[square_to_check].getColor() != position.getTurn()) {
                position[to] = pieceToMove;
                position[from] = PieceCode::noPiece;
                position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
				return true;
			}
			else {
				return false;
			}
		}
		else if (pieceToMove.isKing()) {
			int8_t horizontalDisplacement = to.file - from.file;
			int8_t verticalDisplacement = to.rank - from.rank;

			int8_t castleRank = (position.getTurn() == Color::White) ? 0 : 7;
			ChessSquare castleStartSquare = ChessSquare{ 4, castleRank };
            ChessSquare kingsideCastleRookStartSquare = ChessSquare{ 7, castleRank };
            ChessSquare queensideCastleRookStartSquare = ChessSquare{ 0, castleRank };
			ChessSquare kingsideCastlePassthroughSquare = ChessSquare{ 5, castleRank };
			ChessSquare queensideCastlePassthroughSquare = ChessSquare{ 3, castleRank };
			ChessSquare kingsideCastleEndSquare = ChessSquare{ 6, castleRank };
			ChessSquare queensideCastleEndSquare = ChessSquare{ 2, castleRank };

			// check for castling
			if (position.mayCastleKingside() && from == castleStartSquare && to == kingsideCastleEndSquare) {
				if (position.isSquareAttacked(castleStartSquare) || position.isSquareAttacked(kingsideCastlePassthroughSquare)) {
					return false;
				}
				else if (position[kingsideCastlePassthroughSquare] != PieceCode::noPiece || position[kingsideCastleEndSquare] != PieceCode::noPiece) {
					return false;
				}
				else {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position[kingsideCastlePassthroughSquare] = position[kingsideCastleRookStartSquare];
                    position[kingsideCastleRookStartSquare] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    position.currentPlayerCannotCastleKingsideAnymore();
                    position.currentPlayerCannotCastleQueensideAnymore();
					return true;
				}
			}
			if (position.mayCastleQueenside() && from == castleStartSquare && to == queensideCastleEndSquare) {
				if (position.isSquareAttacked(castleStartSquare) || position.isSquareAttacked(queensideCastlePassthroughSquare)) {
					return false;
				}
				else if (position[queensideCastlePassthroughSquare] != PieceCode::noPiece || position[queensideCastleEndSquare] != PieceCode::noPiece) {
					return false;
				}
				else {
                    position[to] = pieceToMove;
                    position[from] = PieceCode::noPiece;
                    position[queensideCastlePassthroughSquare] = position[queensideCastleRookStartSquare];
                    position[queensideCastleRookStartSquare] = PieceCode::noPiece;
                    position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
                    position.currentPlayerCannotCastleKingsideAnymore();
                    position.currentPlayerCannotCastleQueensideAnymore();
					return true;
				}
			}

			if (std::abs(horizontalDisplacement) > 1) {
				return false;
			}
			if (std::abs(verticalDisplacement) > 1) {
				return false;
			}
			
            position[to] = pieceToMove;
            position[from] = PieceCode::noPiece;
            position.setFileWherePawnJustMovedTwoSpacesForward(std::nullopt);
            position.currentPlayerCannotCastleKingsideAnymore();
            position.currentPlayerCannotCastleQueensideAnymore();
			return true;
		}
        else {
            return false;
        }
	}

    std::optional<ChessPosition> makeMoveIfLegal(const ChessPosition & position, const ChessMove & move) {
        ChessPosition copyOfPosition = position;
        bool gemoetricallyLegal = makeMoveIfGeometricallyLegal(copyOfPosition, move);
        bool kingLeftAttacked = copyOfPosition.isKingAttacked();
        if (gemoetricallyLegal && !kingLeftAttacked) {
            copyOfPosition.swapTurn();
            return copyOfPosition;
        }
        else {
            return std::nullopt;
        }
    }

    /*
    * This function is designed to collect possible moves in an efficient way,
    * balancing how pruned the set of moves is with how quick this function is.
    * This function is required to return a superset of all legal moves. It may not miss any.
    */
    std::vector<ChessMove> collectPotentialMoves(const ChessPosition & position) {
        std::vector<ChessMove> ret = {};

        Color turn = position.getTurn();
        for (int8_t f = 0; f < 8; f++) {
            for (int8_t r = 0; r < 8; r++) {
                PieceCode piece = position[f][r];
                if (piece != 0 && piece.getColor() == turn) {
                    if (piece.isPawn() || piece.isKing()) {
                        for (int8_t x = -2; x < 3; x++) {
                            for (int8_t y = -2; y < 3; y++) {
                                if (std::abs(x) + std::abs(y) <= 2) {
                                    ret.push_back({ {f, r}, {x, y} });
                                }
                            }
                        }
                    }
                    if (piece.isKnight()) {
                        ret.push_back({ {f, r}, {f - 2, r - 1} });
                        ret.push_back({ {f, r}, {f - 2, r + 1} });
                        ret.push_back({ {f, r}, {f + 2, r - 1} });
                        ret.push_back({ {f, r}, {f + 2, r + 1} });
                        ret.push_back({ {f, r}, {f - 1, r - 2} });
                        ret.push_back({ {f, r}, {f - 1, r + 2} });
                        ret.push_back({ {f, r}, {f + 1, r - 2} });
                        ret.push_back({ {f, r}, {f + 1, r + 2} });
                    }
                    if (piece.isBishop() || piece.isQueen()) {
                        std::vector<ChessSquare> directions = {
                            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
                        };
                        for (auto direction : directions) {
                            ChessSquare square_to_check = ChessSquare{ f, r } + direction;
                            while (withinBoard(square_to_check)) {
                                ret.push_back({ { f, r }, square_to_check });
                                if (position[square_to_check] != PieceCode::noPiece) break;
                                square_to_check = square_to_check + direction;
                            }
                        }
                    }
                    if (piece.isRook() || piece.isQueen()) {
                        std::vector<ChessSquare> directions = {
                            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
                        };
                        for (auto direction : directions) {
                            ChessSquare square_to_check = ChessSquare{ f, r } + direction;
                            while (withinBoard(square_to_check)) {
                                ret.push_back({ { f, r }, square_to_check });
                                if (position[square_to_check] != PieceCode::noPiece) break;
                                square_to_check = square_to_check + direction;
                            }
                        }
                    }
                }
            }
        }

        return ret;
    }

}


