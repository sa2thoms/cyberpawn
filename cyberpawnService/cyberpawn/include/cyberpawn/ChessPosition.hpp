#ifndef CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED

#include "PieceCode.hpp"
#include "ChessMove.hpp"

#include <vector>
#include <optional>

namespace cyberpawn {

    class ChessPosition {
    private:
        // a vector of files, each of which is a vector of pieceCodes
        std::vector< std::vector<PieceCode> > board_;
        std::optional<int8_t> fileInWhichAPawnMovedTwoSquaresThePreviousMove_;
        bool whiteMayStillCastleKingside_ = true;
        bool blackMayStillCastleKingside_ = true;
        bool whiteMayStillCastleQueenside_ = true;
        bool blackMayStillCastleQueenside_ = true;
        Color turn_ = Color::White;

    public:
        ~ChessPosition() = default;

        ChessPosition() {
            // size of the the board is set at the start of the constructor and must never ever change.
            // other member functions will assume 8 by 8 board
            this->setBoardTo8by8();
            this->setUpStandardChessGame();
        }

        Color getTurn() const { return turn_; }

        std::optional<int8_t> fileInWhichAPawnMovedTwoSquaresThePreviousMove() const { return fileInWhichAPawnMovedTwoSquaresThePreviousMove_; }
        void setFileWherePawnJustMovedTwoSpacesForward(std::optional<int8_t> file) { fileInWhichAPawnMovedTwoSquaresThePreviousMove_ = file; }

        const std::vector<std::vector<PieceCode>> & getBoard() const { return board_; }

        const std::vector<PieceCode> & operator[](int8_t file) const { return board_[file]; }

        const PieceCode & operator[](const ChessSquare square) const { return board_[square.file][square.rank]; }
        PieceCode & operator[](const ChessSquare square) { return board_[square.file][square.rank]; }

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
        void setBoardTo8by8() {
            board_ = std::vector< std::vector<PieceCode> >(8);
            for (std::vector<PieceCode> & file : board_) {
                file = std::vector<PieceCode>(8);
            }
        }
        void setBoardToEmpty() {
            for (auto & file : board_) {
                for (auto & pieceCode : file) {
                    pieceCode = PieceCode::noPiece;
                }
            }
        }

        void setUpStandardChessGame();

    };


}


#endif // CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
