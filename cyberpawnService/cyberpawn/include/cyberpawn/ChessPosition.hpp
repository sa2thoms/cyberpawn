#ifndef CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED

#include "PieceCode.hpp"
#include "ChessMove.hpp"

#include <vector>
#include <optional>

namespace cyberpawn {

    class ChessPosition {
    private:
        // row-major ordered
        PieceCode board_[8][8];
        std::optional<int8_t> fileInWhichAPawnMovedTwoSquaresThePreviousMove_;
        bool whiteMayStillCastleKingside_ = true;
        bool blackMayStillCastleKingside_ = true;
        bool whiteMayStillCastleQueenside_ = true;
        bool blackMayStillCastleQueenside_ = true;
        Color turn_ = Color::White;

    public:
        ~ChessPosition() = default;

        ChessPosition() {
            this->setUpStandardChessGame();
        }

        // copy constructor
        ChessPosition(const ChessPosition & other) {
            memcpy(board_, other.board_, sizeof(PieceCode) * 64);
            fileInWhichAPawnMovedTwoSquaresThePreviousMove_ = other.fileInWhichAPawnMovedTwoSquaresThePreviousMove_;
            whiteMayStillCastleKingside_ = other.whiteMayStillCastleKingside_;
            whiteMayStillCastleQueenside_ = other.whiteMayStillCastleQueenside_;
            blackMayStillCastleKingside_ = other.blackMayStillCastleKingside_;
            blackMayStillCastleQueenside_ = other.blackMayStillCastleQueenside_;
            turn_ = other.turn_;
        }

        Color getTurn() const { return turn_; }

        std::optional<int8_t> fileInWhichAPawnMovedTwoSquaresThePreviousMove() const { return fileInWhichAPawnMovedTwoSquaresThePreviousMove_; }
        void setFileWherePawnJustMovedTwoSpacesForward(std::optional<int8_t> file) { fileInWhichAPawnMovedTwoSquaresThePreviousMove_ = file; }

        // returns a row-major order array of 64 squares [A1, A2, ... H7, H8];
        PieceCode const * getBoard() const { return board_[0]; }

        PieceCode const * operator[](int8_t file) const { return board_[file]; }

        PieceCode const & operator[](const ChessSquare & square) const { return board_[square.file][square.rank]; }
        PieceCode & operator[](const ChessSquare & square) { return board_[square.file][square.rank]; }

        // returns true if ther player whose turn it is has not moved his king or kingside rook yet
        bool mayCastleKingside() const { return (turn_ == Color::White) ? whiteMayStillCastleKingside_ : blackMayStillCastleKingside_; }
        bool mayCastleQueenside() const { return (turn_ == Color::White) ? whiteMayStillCastleQueenside_ : blackMayStillCastleQueenside_; }

        void currentPlayerCannotCastleKingsideAnymore() {
            if (turn_ == Color::White) {
                whiteMayStillCastleKingside_ = false;
            }
            else {
                blackMayStillCastleKingside_ = false;
            }
        }
        void currentPlayerCannotCastleQueensideAnymore() {
            if (turn_ == Color::White) {
                whiteMayStillCastleQueenside_ = false;
            }
            else {
                blackMayStillCastleQueenside_ = false;
            }
        }

        void swapTurn() { turn_ = ((turn_ == Color::White) ? Color::Black : Color::White); }

        // returns true if the square is attacked by the opposing player (the player whose turn it is not)
        bool isSquareAttacked(ChessSquare square) const;
        bool isKingAttacked() const;

        bool isInCheckmate() const;
        bool isInStalemate() const;
    private:
        void setBoardToEmpty() {
            for (auto & file : board_) {
                for (auto & pieceCode : file) {
                    pieceCode = PieceCode::noPiece;
                }
            }
        }

        void setUpStandardChessGame();

        // isSquareAttacked implementations
        bool isSquareAttacked_directionalSearch(ChessSquare square) const;
    };


}


#endif // CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
