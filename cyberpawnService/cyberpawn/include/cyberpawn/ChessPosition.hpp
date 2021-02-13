#ifndef CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_POSITION_H_INCLUDED

#include "PieceCode.hpp"

#include <vector>
#include <optional>

namespace cyberpawn {

	class ChessPosition {
	private:
		// a vector of files, each of which is a vector of pieceCodes
		std::vector< std::vector<PieceCode> > board_;
		std::optional<int8_t> fileInWhichAPawnMovedTwoSquaresThePreviousMove_;
		bool whiteMayStillCastle_ = true;
		bool blackMayStillCastle_ = true;

	public:
		~ChessPosition() = default;

		ChessPosition() {
			// size of the the board is set at the start of the constructor and must never ever change.
			// other member functions will assume 8 by 8 board
			this->setBoardTo8by8();
			this->setUpStandardChessGame();
		}

		const std::vector<std::vector<PieceCode>> & getBoard() const { return board_; }

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
