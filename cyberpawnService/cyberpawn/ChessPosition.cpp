#include "cyberpawn/ChessPosition.hpp"

#include "cyberpawn/ChessGameFunctions.hpp"

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

    namespace attackDirectionLookUpTables {

        const std::vector< std::pair<ChessSquare, std::vector< std::pair <PieceCode, bool> > > > directionsOfAttack_white = {
            {
                {-1, -1},
                {
                    {PieceCode::whiteBishop, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false},
                    {PieceCode::whitePawn, false}
                }
            },
            {
                {-1, 0},
                {
                    {PieceCode::whiteRook, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {-1, 1},
                {
                    {PieceCode::whiteBishop, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {0, -1},
                {
                    {PieceCode::whiteRook, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {0, 1},
                {
                    {PieceCode::whiteRook, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {1, -1},
                {
                    {PieceCode::whiteBishop, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false},
                    {PieceCode::whitePawn, false}
                }
            },
            {
                {1, 0},
                {
                    {PieceCode::whiteRook, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {1, 1},
                {
                    {PieceCode::whiteBishop, true},
                    {PieceCode::whiteQueen, true},
                    {PieceCode::whiteKing, false}
                }
            },
            {
                {-2, -1},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {-2, 1},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {2, -1},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {2, 1},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {-1, -2},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {-1, 2},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {1, -2},
                {
                    {PieceCode::whiteKnight, false}
                }
            },
            {
                {1, 2},
                {
                    {PieceCode::whiteKnight, false}
                }
            }
        };
        const std::vector< std::pair<ChessSquare, std::vector< std::pair <PieceCode, bool> > > > directionsOfAttack_black = {
            {
                {-1, -1},
                {
                    {PieceCode::blackBishop, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {-1, 0},
                {
                    {PieceCode::blackRook, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {-1, 1},
                {
                    {PieceCode::blackBishop, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false},
                    {PieceCode::blackPawn, false}
                }
            },
            {
                {0, -1},
                {
                    {PieceCode::blackRook, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {0, 1},
                {
                    {PieceCode::blackRook, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {1, -1},
                {
                    {PieceCode::blackBishop, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {1, 0},
                {
                    {PieceCode::blackRook, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false}
                }
            },
            {
                {1, 1},
                {
                    {PieceCode::blackBishop, true},
                    {PieceCode::blackQueen, true},
                    {PieceCode::blackKing, false},
                    {PieceCode::blackPawn, false}
                }
            },
            {
                {-2, -1},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {-2, 1},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {2, -1},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {2, 1},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {-1, -2},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {-1, 2},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {1, -2},
                {
                    {PieceCode::blackKnight, false}
                }
            },
            {
                {1, 2},
                {
                    {PieceCode::blackKnight, false}
                }
            }
        };
    }


    bool ChessPosition::isSquareAttacked_directionalSearch(const ChessSquare & square) const {
        // Attacks can come from a diagonal, a straight line, or a knight attack.
        // Note that kings and pawns still attack along a diagonal or straight line.

        Color opposingColor = (turn_ == Color::White) ? Color::Black : Color::White;

        const std::vector< std::pair<ChessSquare, std::vector< std::pair <PieceCode, bool> > > > & directionsOfAttack =
            ((opposingColor == Color::White)) ? attackDirectionLookUpTables::directionsOfAttack_white
            : attackDirectionLookUpTables::directionsOfAttack_black;

        for (const auto & directionData : directionsOfAttack) {
            const ChessSquare & direction = directionData.first;

            ChessSquare square_to_check = square + direction;
            bool firstStep = true;
            while (withinBoard(square_to_check)) {
                if (this->operator[](square_to_check) == PieceCode::noPiece) {
                    firstStep = false;
                    square_to_check = square_to_check + direction;
                    continue;
                }
                if (this->operator[](square_to_check).getColor() == opposingColor) {
                    for (const auto & capablePiece : directionData.second) {
                        if ((capablePiece.first == this->operator[](square_to_check)) && (firstStep || capablePiece.second)) {
                            return true;
                        }
                    }
                }
                break;
            }
        }

        // all attack vectors have been checked, so we can safely say the square is not under attack.
        return false;
    }

    bool ChessPosition::isSquareAttacked(const ChessSquare & square) const {
        return isSquareAttacked_directionalSearch(square);
    }

    bool ChessPosition::isKingAttacked() const {
        PieceCode kingInQuestion = (turn_ == Color::White) ? PieceCode::whiteKing : PieceCode::blackKing;
        for (int8_t f = 0; f < 8; f++) {
            for (int8_t r = 0; r < 8; r++) {
                if (board_[f][r] == kingInQuestion) {
                    return isSquareAttacked({ f, r });
                }
            }
        }
        return false;
    }

    bool ChessPosition::isInCheckmate() const {
        if (isKingAttacked()) {
            if (allLegalMoves(*this).size() == 0) {
                return true;
            }
            else {
                return false;
            }
        }
        else return false;
    }

    bool ChessPosition::isInStalemate() const {
        if (isKingAttacked()) {
            return false;
        }
        else {
            if (allLegalMoves(*this).size() == 0) {
                return true;
            }
            else return false;
        }
    }

}
