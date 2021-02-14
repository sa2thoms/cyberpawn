#include "pch.h"

#include "cyberpawn/BetaTreeEngine.hpp"

using namespace cyberpawn;

TEST(BetaTreeEngineTest, finds20PossibleOpeningMovesForWhite) {
	BetaTreeEngine engine(1);

	ChessPosition position;

	std::vector<ChessMove> bestMoves = engine.findBestMoves(position, 100);

	EXPECT_EQ(bestMoves.size(), 20);
}
