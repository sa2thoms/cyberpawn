#include "cyberpawn/BetaTreeEngine.hpp"


namespace cyberpawn {

	float standalonePieceScore(const PieceCode & pieceCode) {
		if (pieceCode == PieceCode::whitePawn) {
			return 1.0f;
		}
		else if (pieceCode == PieceCode::whiteBishop) {
			return 3.14f;
		}
		else if (pieceCode == PieceCode::whiteKnight) {
			return 3.0f;
		}
		else if (pieceCode == PieceCode::whiteRook) {
			return 5.0f;
		}
		else if (pieceCode == PieceCode::whiteQueen) {
			return 9.0f;
		}
		else if (pieceCode == PieceCode::whiteKing) {
			// the king needs to be worth more than anything else possibly could be combined
			return 16.f * 10.0f;
		}
		else if (pieceCode.getColor() == Color::Black) {
			// black score is the negative of the white score
			return -standalonePieceScore(pieceCode & int8_t(0b11101111));
		}
		else {
			return 0.f;
		}
	}

	float calculateStaticPositionScore(const ChessPosition & position) {
		// the naive implementation of this could just be a point total of the number of pieces still on the board
		
		float score_total = 0.f;

		const auto & board = position.getBoard();
		for (const auto & file : board) {
			for (const auto & pieceCode : file) {
				score_total += standalonePieceScore(pieceCode);
			}
		}

		return score_total;
	}

	std::vector<ChessMove> BetaTreeEngine::findBestMoves(const ChessPosition & position, int maxMoves) const {
        return {};
	}

}

