#include "cyberpawn/BetaTreeEngine.hpp"

#include "cyberpawn/ChessGameFunctions.hpp"

#include <functional>
#include <algorithm>
#include <iterator>

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

    std::pair<ChessMove, float> findBestMove(const ChessPosition & position, int depth, float parentBestScoreReached) {
        std::vector<ChessMove> possibleMoves = collectPotentialMoves(position);
        std::function<bool(float, float)> isScoreBetterThan = (position.getTurn() == Color::White)
            ? std::function<bool(float, float)>([](float scoreA, float scoreB) -> bool { return scoreA > scoreB; })
            : std::function<bool(float, float)>([](float scoreA, float scoreB) -> bool { return scoreA < scoreB; });

        std::pair<ChessMove, float> bestFoundSoFar = {
            {{0, 0}, {0, 0}}, (position.getTurn() == Color::White) ? -INFINITY : INFINITY
        };

        std::function<float(const ChessPosition &)> calculateScore =
            (depth <= 1) ? std::function<float(const ChessPosition &)>([](const ChessPosition & position) -> float {return calculateStaticPositionScore(position); })
            : std::function<float(const ChessPosition &)>([&depth, &bestFoundSoFar](const ChessPosition & position) -> float {return findBestMove(position, depth - 1, bestFoundSoFar.second).second; });

        
        for (const ChessMove & move : possibleMoves) {
            auto newPosition = makeMoveIfLegal(position, move);
            if (newPosition) {
                float score = calculateScore(newPosition.value());
                if (isScoreBetterThan(score, bestFoundSoFar.second)) {
                    bestFoundSoFar = { move, score };

                    // alpha-beta pruning
                    if (isScoreBetterThan(bestFoundSoFar.second, parentBestScoreReached)) {
                        break;
                    }
                }
            }
        }

        return bestFoundSoFar;
    }

	std::vector<ChessMove> BetaTreeEngine::findBestMoves(const ChessPosition & position, int maxMoves) const {
        std::vector<ChessMove> possibleMoves = collectPotentialMoves(position);
        std::function<bool(float, float)> isScoreBetterThan = (position.getTurn() == Color::White)
            ? std::function<bool(float, float)>([](float scoreA, float scoreB) -> bool { return scoreA > scoreB; })
            : std::function<bool(float, float)>([](float scoreA, float scoreB) -> bool { return scoreA < scoreB; });

        std::vector<std::pair<ChessMove, float>> validMoves;

        for (auto move : possibleMoves) {
            auto newPosition = makeMoveIfLegal(position, move);
            if (newPosition) {
                float score = findBestMove(newPosition.value(), searchDepth_ - 1, (position.getTurn() == Color::White) ? -INFINITY : INFINITY).second;
                validMoves.push_back({ move, score });
            }
        }
        
        std::sort(
            validMoves.begin(),
            validMoves.end(),
            [&isScoreBetterThan](const std::pair<ChessMove, float> & a, const std::pair<ChessMove, float> & b) {
                return isScoreBetterThan(a.second, b.second);
            }
        );

        std::vector<ChessMove> sortedMoves;

        std::transform(
            validMoves.begin(),
            validMoves.end(),
            std::back_inserter(sortedMoves),
            [](const std::pair<ChessMove, float> & pair) {
                return pair.first;
            }
        );

        if (sortedMoves.size() > maxMoves) {
            sortedMoves.resize(maxMoves);
        }

        return sortedMoves;
	}

}

