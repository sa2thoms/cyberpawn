#include "cyberpawn/BetaTreeEngine.hpp"

#include "cyberpawn/ChessGameFunctions.hpp"

#include <algorithm>
#include <iterator>
#include <limits>
#include <thread>
#include <vector>

namespace cyberpawn {

    float calculateStaticPositionScore(const ChessPosition& position);

    namespace {

        constexpr float kInfinity = std::numeric_limits<float>::infinity();

        float evaluateSideToMove(const ChessPosition& position) {
            const float score = calculateStaticPositionScore(position);
            return position.getTurn() == Color::White ? score : -score;
        }

        float alphaBeta(ChessPosition& position, int depth, float alpha, float beta) {
            if (depth <= 0) {
                return evaluateSideToMove(position);
            }

            float bestScore = -kInfinity;

            for (const ChessMove& move : collectPotentialMoves(position)) {
                MoveUndo undo;
                if (!tryMakeMove(position, move, undo)) {
                    continue;
                }

                const float score = -alphaBeta(position, depth - 1, -beta, -alpha);
                unmakeMove(position, undo);
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
                if (alpha >= beta) {
                    return beta;
                }
            }

            return bestScore;
        }

    } // namespace

    float standalonePieceScore(const PieceCode & pieceCode) {
        //switch (pieceCode.asChar()) {
        //case PieceCode::whitePawn:
        //    return 0.87;
        //case PieceCode::whiteBishop:
        //    return 3.04;
        //case PieceCode::whiteKnight:
        //    return 3.0f;
        //case PieceCode::whiteRook:
        //    return 4.7;
        //case PieceCode::whiteQueen:
        //    return 9.2f;
        //case PieceCode::whiteKing:
        //    return 16.f * 10.0f;
        //default:
        //    if (pieceCode.getColor() == Color::Black) {
        //        // black score is the negative of the white score
        //        return -standalonePieceScore(pieceCode & int8_t(0b11101111));
        //    }
        //    else {
        //        return 0.f;
        //    }
        //}

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

    std::vector<ChessMove> BetaTreeEngine::findBestMoves(const ChessPosition & position, int maxMoves) const {
        const std::vector<ChessMove> possibleMoves = collectPotentialMoves(position);
        std::vector<std::pair<ChessMove, float>> validMoves;

        if (this->useMultipleThreads_) {
            std::vector<std::optional<std::pair<ChessMove, float>>> testedMoves(possibleMoves.size());

            std::vector<std::thread> threads;
            for (int i = 0; i < static_cast<int>(possibleMoves.size()); i++) {
                threads.emplace_back([&, i]() {
                    ChessPosition searchPosition = position;
                    MoveUndo undo;
                    if (tryMakeMove(searchPosition, possibleMoves[i], undo)) {
                        const float score = -alphaBeta(
                            searchPosition,
                            searchDepth_ - 1,
                            -kInfinity,
                            kInfinity
                        );
                        testedMoves[i] = { possibleMoves[i], score };
                    }
                });
            }
            for (auto& thread : threads) {
                thread.join();
            }

            for (auto& pair : testedMoves) {
                if (pair) {
                    validMoves.push_back(pair.value());
                }
            }
        }
        else {
            ChessPosition searchPosition = position;
            for (const ChessMove& move : possibleMoves) {
                MoveUndo undo;
                if (tryMakeMove(searchPosition, move, undo)) {
                    const float score = -alphaBeta(
                        searchPosition,
                        searchDepth_ - 1,
                        -kInfinity,
                        kInfinity
                    );
                    unmakeMove(searchPosition, undo);
                    validMoves.push_back({ move, score });
                }
            }
        }

        std::sort(
            validMoves.begin(),
            validMoves.end(),
            [](const std::pair<ChessMove, float>& a, const std::pair<ChessMove, float>& b) {
                return a.second > b.second;
            }
        );

        std::vector<ChessMove> sortedMoves;
        sortedMoves.reserve(validMoves.size());

        std::transform(
            validMoves.begin(),
            validMoves.end(),
            std::back_inserter(sortedMoves),
            [](const std::pair<ChessMove, float>& pair) {
                return pair.first;
            }
        );

        if (static_cast<int>(sortedMoves.size()) > maxMoves) {
            sortedMoves.resize(maxMoves);
        }

        return sortedMoves;
    }

}
