#ifndef CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED

#include "PieceCode.hpp"
#include "ChessMove.hpp"

#include <vector>
#include <optional>

namespace cyberpawn {

    class ChessPosition {
    private:
        // board is ordered such that A1 is accessed as [A][1]. I believe this is technically column-major order
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

        bool mayCastleKingside(Color turn) const {
            return (turn == Color::White) ? whiteMayStillCastleKingside_ : blackMayStillCastleKingside_;
        }
        bool mayCastleQueenside(Color turn) const {
            return (turn == Color::White) ? whiteMayStillCastleQueenside_ : blackMayStillCastleQueenside_;
        }

        // returns true if the player whose turn it is has not moved his king or kingside rook yet
        bool mayCastleKingside() const { return mayCastleKingside(turn_); }
        bool mayCastleQueenside() const { return mayCastleQueenside(turn_); }

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
        bool isSquareAttacked(const ChessSquare & square) const;
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
        bool isSquareAttacked_directionalSearch(const ChessSquare & square) const;
    };



    // a 256-bit representation of a chess position
    class CompressedChessPosition {
    private:
        // column-major order. Each square is 4 bits. E.g. the first uint8_t element stores A1 and A2
        uint8_t data_[32] = { 0 };


        // bit 3 (the most significant bit of the four) is the color bit. 0 == white, 1 == black
        // bits 0 through 2 determine what piece it is, with two special codes for positional metadata.
        //     000: pawn
        //     001: bishop
        //     010: rook
        //     011: queen
        //     100: knight
        //     101: king
        //     110: piece that may be involved in a special move (if in a rook starting spot, may be involved in castling. If on the 4th/5th rank, a pawn that may be taken en passant)
        //     111: empty square. The color of the empty squares indicate whos turn it is. If empty squares are 0111, it's white's turn. If 1111, it's black's turn)

        uint8_t compressedPieceCodeFromRegularPieceCode(PieceCode regularCode, uint8_t compressedEmptyCode = 0b0111) {
            switch (regularCode.asChar())
            {
            case PieceCode::whitePawn:
                return 0b0000;
            case PieceCode::whiteBishop:
                return 0b0001;
            case PieceCode::whiteRook:
                return 0b0010;
            case PieceCode::whiteQueen:
                return 0b0011;
            case PieceCode::whiteKnight:
                return 0b0100;
            case PieceCode::whiteKing:
                return 0b0101;
            case PieceCode::blackPawn:
                return 0b1000;
            case PieceCode::blackBishop:
                return 0b1001;
            case PieceCode::blackRook:
                return 0b1010;
            case PieceCode::blackQueen:
                return 0b1011;
            case PieceCode::blackKnight:
                return 0b1100;
            case PieceCode::blackKing:
                return 0b1101;
            case PieceCode::noPiece:
            default:
                return compressedEmptyCode;
            }
        }

        // the data to write must be in the lower 4 bits of "data"
        void writeSquare(uint8_t file, uint8_t rank, uint8_t data) {
            int accessor = (file << 2) + (rank >> 1);
            uint8_t preserve_mask = 0b11110000 >> (4 * (rank & 0b1));
            uint8_t write_mask = ~preserve_mask;
            uint8_t shifted_write_data = data << (4 * (rank & 0b1));
            data_[accessor] = (data_[accessor] & preserve_mask) | (shifted_write_data & write_mask);
        }

    public:

        explicit CompressedChessPosition(ChessPosition const & position) {
            uint8_t empty_square_code = (position.getTurn() == Color::White) ? 0b0111 : 0b1111;
            for (int file = 0; file < 8; file++) {
                // go up by two ranks at a time for convenience
                for (int rank = 0; rank < 8; rank += 2) {
                    uint8_t code0 = compressedPieceCodeFromRegularPieceCode(position[file][rank + 0], empty_square_code);
                    uint8_t code1 = compressedPieceCodeFromRegularPieceCode(position[file][rank + 1], empty_square_code);
                    data_[(file << 2) + (rank >> 1)] = (code1 << 4) | code0;
                }
            }

            // add en passant info
            if (position.fileInWhichAPawnMovedTwoSquaresThePreviousMove().has_value()) {
                uint8_t file = position.fileInWhichAPawnMovedTwoSquaresThePreviousMove().value();
                uint8_t rank = (position.getTurn() == Color::White) ? 4 : 3;
                int accessor = (file << 2) + (rank >> 1);
                uint8_t preserve_mask = 0b11110000 >> (4 * (rank & 0b1));
                uint8_t write_code = (position.getTurn() == Color::White) ? 0b1110 : 0b0110;
                uint8_t shifted_write_data = write_code << (4 * (rank & 0b1));
                data_[accessor] = (preserve_mask & data_[accessor]) | shifted_write_data;
            }

            // add castling info
            if (position.mayCastleKingside(Color::White)) {
                uint8_t file = 7;
                uint8_t rank = 0;
                uint8_t write_code = 0b0110;
                writeSquare(file, rank, write_code);
            }
            if (position.mayCastleQueenside(Color::White)) {
                uint8_t file = 0;
                uint8_t rank = 0;
                uint8_t write_code = 0b0110;
                writeSquare(file, rank, write_code);
            }
            if (position.mayCastleQueenside(Color::Black)) {
                uint8_t file = 0;
                uint8_t rank = 7;
                uint8_t write_code = 0b1110;
                writeSquare(file, rank, write_code);
            }
            if (position.mayCastleKingside(Color::Black)) {
                uint8_t file = 7;
                uint8_t rank = 7;
                uint8_t write_code = 0b1110;
                writeSquare(file, rank, write_code);
            }
        }

        ChessPosition uncompress() const {
            ChessPosition ret;
            // TODO


            return ret;
        }

        uint8_t const * data() const {
            return data_;
        }

    };


}


#endif // CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
