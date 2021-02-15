#ifndef CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED

#include "Engine.hpp"

namespace cyberpawn {

    class BetaTreeEngine : public Engine {
    private:
        int searchDepth_ = 4;

        bool useMultipleThreads_ = false;
    public:
        BetaTreeEngine() = default;
        BetaTreeEngine(int searchDepth) : searchDepth_(searchDepth) {}

        void setSearchDepth(int depth) { searchDepth_ = depth; }

        void setAllowMultipleThreads(bool allowMultipleThreads) { useMultipleThreads_ = allowMultipleThreads; }

        // returns the best moves in order of descending quality (best move is first in the vector)
        std::vector<ChessMove> findBestMoves(const ChessPosition & position, int maxMoves) const override;
    };

}


#endif // CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
