#include "cyberpawn/ChessGame.hpp"
#include "cyberpawn/BetaTreeEngine.hpp"
#include "cyberpawn/Exception.hpp"

#include <iostream>
#include <chrono>

using namespace cyberpawn;

std::string pieceCodeAsString(const PieceCode & code) {
    switch (code.asChar()) {
    case PieceCode::whitePawn:
        return "wpa";
    case PieceCode::whiteBishop:
        return "wbi";
    case PieceCode::whiteKnight:
        return "wkn";
    case PieceCode::whiteRook:
        return "wro";
    case PieceCode::whiteQueen:
        return "WQu";
    case PieceCode::whiteKing:
        return "WKi";
    case PieceCode::blackPawn:
        return "bpa";
    case PieceCode::blackBishop:
        return "bbi";
    case PieceCode::blackKnight:
        return "bkn";
    case PieceCode::blackRook:
        return "bro";
    case PieceCode::blackQueen:
        return "BQu";
    case PieceCode::blackKing:
        return "BKi";
    default:
        return "   ";
    }
}

void printPosition(const ChessPosition & position) {
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            std::cout << "+-----";
        }
        std::cout << "+" << std::endl;
        for (int f = 0; f < 8; f++) {
            std::cout << "|     ";
        }
        std::cout << "|" << std::endl;
        for (int f = 0; f < 8; f++) {
            std::cout << "| " << pieceCodeAsString(position[f][r]) << " ";
        }
        std::cout << "|" << std::endl;
        for (int f = 0; f < 8; f++) {
            std::cout << "|     ";
        }
        std::cout << "|" << std::endl;
    }
    for (int f = 0; f < 8; f++) {
        std::cout << "+-----";
    }
    std::cout << "+" << std::endl;
}

ChessGame::State enginesPlayEachother(const Engine & enginePlayingWhite, const Engine & enginePlayingBlack) {
    ChessGame game;

    printPosition(game.getPosition());

    while (!game.isOver()) {
        const Engine & enginePlayingAMove = (game.whoseTurn() == Color::White) ? enginePlayingWhite : enginePlayingBlack;
        auto bestMoves = enginePlayingAMove.findBestMoves(game.getPosition(), 5);
        bool succeeded = game.makeMove(bestMoves[0]);
        
        printPosition(game.getPosition());
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
    BetaTreeEngine whiteEngine(4);
    BetaTreeEngine blackEngine(4);

    auto start = std::chrono::high_resolution_clock::now();

    ChessGame::State outcome = enginesPlayEachother(whiteEngine, blackEngine);

    auto end = std::chrono::high_resolution_clock::now();

    auto game_duration = end - start;

    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(game_duration);
    
    std::cout << "Game ended in " << gameStateToString(outcome) << ", and took " << duration_ms.count() << " milliseconds." << std::endl;

}
