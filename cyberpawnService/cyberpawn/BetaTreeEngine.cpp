#include "cyberpawn/BetaTreeEngine.hpp"

#include "cyberpawn/ChessGameFunctions.hpp"

#include <functional>
#include <algorithm>
#include <iterator>
#include <thread>

namespace cyberpawn {

    float standalonePieceScore(const PieceCode & pieceCode) {
        if (pieceCode == PieceCode::whitePawn) {
            return 0.87f;
        }
        else if (pieceCode == PieceCode::whiteBishop) {
            return 3.04f;
        }
        else if (pieceCode == PieceCode::whiteKnight) {
            return 3.0f;
        }
        else if (pieceCode == PieceCode::whiteRook) {
            return 4.7f;
        }
        else if (pieceCode == PieceCode::whiteQueen) {
            return 9.2f;
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

        bool white_odd_bishop_exists = false;
        bool white_even_bishop_exists = false;
        bool black_odd_bishop_exists = false;
        bool black_even_bishop_exists = false;


        const auto & board = position.getBoard();
        for (int8_t f = 0; f < 8; f++) {
            for (int8_t r = 0; r < 8; r++) {
                PieceCode pieceCode = position[f][r];
                score_total += standalonePieceScore(pieceCode);

                // track bishop pairs
                if (pieceCode == PieceCode::whiteBishop) {
                    if (((std::abs(f) + std::abs(r)) % 2) == 0) {
                        white_even_bishop_exists = true;
                    }
                    else {
                        white_odd_bishop_exists = true;
                    }
                }
                else if (pieceCode == PieceCode::blackBishop) {
                    if (((std::abs(f) + std::abs(r)) % 2) == 0) {
                        black_even_bishop_exists = true;
                    }
                    else {
                        black_odd_bishop_exists = true;
                    }
                }

                // pawns are worth more further up the board
                if (pieceCode == PieceCode::whitePawn) {
                    score_total += (float(r * r) * 0.02f);
                }
                else if (pieceCode == PieceCode::blackPawn) {
                    score_total -= (float(r * r) * 0.02f);
                }

                float sqr_dist_from_center = math::square(float(r) - 3.5f) + math::square(float(f) - 3.5f);
                if (pieceCode == PieceCode::whiteKnight) {
                    score_total += (1.1f - 0.05f * sqr_dist_from_center);
                }
                if (pieceCode == PieceCode::blackKnight) {
                    score_total -= (1.1f - 0.05f * sqr_dist_from_center);
                }
            }
        }

        if (white_even_bishop_exists && white_odd_bishop_exists) {
            score_total += 0.32f;
        }
        if (black_even_bishop_exists && black_odd_bishop_exists) {
            score_total -= 0.32f;
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
        
        if (this->useMultipleThreads_) {
            std::vector<std::optional<std::pair<ChessMove, float> > > testedMoves;
            testedMoves.resize(possibleMoves.size());

            std::function<void(ChessMove, int)> evaluateMoveAndPlaceInTestedMoves = [&](ChessMove move, int index) {
                auto newPosition = makeMoveIfLegal(position, move);
                if (newPosition) {
                    float score = findBestMove(newPosition.value(), searchDepth_ - 1, (position.getTurn() == Color::White) ? -INFINITY : INFINITY).second;
                    testedMoves[index] = { move, score };
                }
            };

            std::vector<std::thread> threads;
            for (int i = 0; i < possibleMoves.size(); i++) {
                threads.push_back(std::thread(evaluateMoveAndPlaceInTestedMoves, possibleMoves[i], i));
            }
            for (auto & thread : threads) {
                thread.join();
            }

            for (auto & pair : testedMoves) {
                if (pair) {
                    validMoves.push_back(pair.value());
                }
            }
        }
        else {
            for (auto move : possibleMoves) {
                auto newPosition = makeMoveIfLegal(position, move);
                if (newPosition) {
                    float score = findBestMove(newPosition.value(), searchDepth_ - 1, (position.getTurn() == Color::White) ? -INFINITY : INFINITY).second;
                    validMoves.push_back({ move, score });
                }
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

