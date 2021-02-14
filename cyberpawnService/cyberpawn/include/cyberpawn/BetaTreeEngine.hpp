#ifndef CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED

#include "Engine.hpp"

namespace cyberpawn {

	class BetaTreeEngine : public Engine {
    private:
        int searchDepth_ = 4;
	public:

        // returns the best moves in order of descending quality (best move is first in the vector)
		std::vector<ChessMove> findBestMoves(const ChessPosition & position, int maxMoves) const override;
	};

}


#endif // CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
