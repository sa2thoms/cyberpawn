#ifndef CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED

#include "Engine.hpp"

namespace cyberpawn {

	class BetaTreeEngine : public Engine {
	public:
		std::vector<ChessMove> findBestMoves(const ChessPosition & position, int maxMoves) const override;
	};

}


#endif // CYBERPAWN_CYBERPAWN_BETA_TREE_ENGINE_H_INCLUDED
