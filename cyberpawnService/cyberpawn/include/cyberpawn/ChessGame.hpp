#ifndef CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED

#include "ChessPosition.hpp"
#include "ChessGameFunctions.hpp"

namespace cyberpawn {


    // a class that holds the state of an entire chess game
    class ChessGame {
    public:
        enum class State {
            Ongoing,
            WhiteWinsByCheckmate,
            BlackWinsByCheckmate,
            Stalemate,
            DrawByRepetition
        };
    private:
        State state_ = State::Ongoing;
        std::vector<ChessMove> move_log_;
        ChessPosition position_;
    public:
        bool makeMove(const ChessMove & move) {
            auto newPosition = makeMoveIfLegal(position_, move);
            if (newPosition) {
                position_ = newPosition.value();
                move_log_.push_back(move);
                if (position_.isInCheckmate()) {
                    state_ = (position_.getTurn() == Color::White) ? State::BlackWinsByCheckmate : State::WhiteWinsByCheckmate;
                }
                else if (position_.isInStalemate()) {
                    state_ = State::Stalemate;
                }

                // TODO: be more complete about how we check for draw by repetition; this only catches the simple cases
                if (move_log_.size() > 10) {
                    size_t last_move = move_log_.size() - 1;
                    if (move_log_[last_move] == move_log_[last_move - 4] && move_log_[last_move] == move_log_[last_move - 8]) {
                        if (move_log_[last_move - 1] == move_log_[last_move - 5] && move_log_[last_move - 1] == move_log_[last_move - 9]) {
                            state_ = State::DrawByRepetition;
                        }
                    }
                }

                return true;
            }
            else {
                return false;
            }
        }

        bool isOver() const {
            return state_ != State::Ongoing;
        }

        State getState() const { return this->state_; }

        Color whoseTurn() const { return position_.getTurn(); }

        const ChessPosition & getPosition() const { return position_; }
    };





}


#endif //CYBERPAWN_CYBERPAWN_CLI_CYBERPAWN_CLI_H_INCLUDED
