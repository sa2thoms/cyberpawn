#ifndef CYBERPAWN_CYBERPAWN_PIECE_CODE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_PIECE_CODE_H_INCLUDED

#include "Color.hpp"

#include <cstdint>

namespace cyberpawn {

    class PieceCode {
    public:
        // each type of piece has an 8 bit binary code
        static constexpr int8_t noPiece     = 0;

        static constexpr int8_t whitePawn   = 0b00000001;
        static constexpr int8_t whiteBishop = 0b00000010;
        static constexpr int8_t whiteKnight = 0b00000011;
        static constexpr int8_t whiteRook   = 0b00000100;
        static constexpr int8_t whiteQueen  = 0b00000110;
        static constexpr int8_t whiteKing   = 0b00001000;

        static constexpr int8_t blackPawn   = 0b00010001;
        static constexpr int8_t blackBishop = 0b00010010;
        static constexpr int8_t blackKnight = 0b00010011;
        static constexpr int8_t blackRook   = 0b00010100;
        static constexpr int8_t blackQueen  = 0b00010110;
        static constexpr int8_t blackKing   = 0b00011000;


    private:
        int8_t code_;

    public:
        PieceCode() : code_(0) {}
        PieceCode(int8_t code) : code_(code) {}

        Color getColor() const {
            return Color(code_ >> 4);
        }

        bool operator==(const PieceCode & other) const {
            return this->code_ == other.code_;
        }
        bool operator!=(const PieceCode & other) const {
            return this->code_ != other.code_;
        }

        PieceCode operator&(const PieceCode & other) const {
            return this->code_ & other.code_;
        }

        bool isKnight() const {
            return (code_ & 0b11101111) == whiteKnight;
        }
        bool isBishop() const {
            return (code_ & 0b11101111) == whiteBishop;
        }
        bool isPawn() const {
            return (code_ & 0b11101111) == whitePawn;
        }
        bool isRook() const {
            return (code_ & 0b11101111) == whiteRook;
        }
        bool isQueen() const {
            return (code_ & 0b11101111) == whiteQueen;
        }
        bool isKing() const {
            return (code_ & 0b11101111) == whiteKing;
        }

        int8_t asChar() const { return code_; }
    };




}


#endif // CYBERPAWN_CYBERPAWN_PIECE_CODE_H_INCLUDED
