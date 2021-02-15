#include "cyberpawn/ChessGame.hpp"
#include "cyberpawn/BetaTreeEngine.hpp"
#include "cyberpawn/Exception.hpp"

#include <iostream>
#include <chrono>

using namespace cyberpawn;

ChessGame::State enginesPlayEachother(const Engine & enginePlayingWhite, const Engine & enginePlayingBlack) {
    ChessGame game;

    while (!game.isOver()) {
        const Engine & enginePlayingAMove = (game.whoseTurn() == Color::White) ? enginePlayingWhite : enginePlayingBlack;
        auto bestMoves = enginePlayingAMove.findBestMoves(game.getPosition(), 5);
        bool succeeded = game.makeMove(bestMoves[0]);
        if (!succeeded) {
            throw cyberpawn::Exception("someone tried to make an illegal move.");
        }
    }

    return game.getState();
}

std::string gameStateToString(ChessGame::State state) {
    switch (state) {
    case ChessGame::State::Ongoing:
        return "Ongoing";
    case ChessGame::State::WhiteWinsByCheckmate:
        return "White wins by checkmate";
    case ChessGame::State::BlackWinsByCheckmate:
        return "Black wins by checkmate";
    case ChessGame::State::Stalemate:
        return "Stalemate";
    case ChessGame::State::DrawByRepetition:
        return "Draw by repetition";
    default:
        return "Unknown";
    }
}

int main() {
    BetaTreeEngine whiteEngine(3);
    BetaTreeEngine blackEngine(3);

    auto start = std::chrono::high_resolution_clock::now();

    ChessGame::State outcome = enginesPlayEachother(whiteEngine, blackEngine);

    auto end = std::chrono::high_resolution_clock::now();

    auto game_duration = end - start;

    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(game_duration);
    
    std::cout << "Game ended in " << gameStateToString(outcome) << ", and took " << duration_ms.count() << " milliseconds." << std::endl;

}
