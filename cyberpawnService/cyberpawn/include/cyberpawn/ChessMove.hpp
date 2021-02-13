#ifndef CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED

#include <cstdint>

namespace cyberpawn {

	struct ChessSquare {
		int8_t file;
		int8_t rank;
	};

	struct ChessMove {
		ChessSquare from;
		ChessSquare to;
	};

}


#endif // CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED
