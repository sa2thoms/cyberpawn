#ifndef CYBERPAWN_CYBERPAWN_ENGINE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_ENGINE_H_INCLUDED

#include "ChessPosition.hpp"
#include "ChessMove.hpp"


namespace cyberpawn {

	class Engine {
	public:
		virtual std::vector<ChessMove> findBestMoves(const ChessPosition & position, int maxMoves) const = 0;
	};

}


#endif // CYBERPAWN_CYBERPAWN_ENGINE_H_INCLUDED
