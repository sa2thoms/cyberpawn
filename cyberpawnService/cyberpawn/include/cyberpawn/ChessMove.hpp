#ifndef CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED

#include <cstdint>

#include <optional>
#include <string>

namespace cyberpawn {

	struct ChessSquare {
		int8_t file;
		int8_t rank;

		ChessSquare operator+(const ChessSquare & other) const { return { file + other.file, rank + other.rank }; }
		ChessSquare operator-(const ChessSquare & other) const { return { file - other.file, rank - other.rank }; }

		bool operator==(const ChessSquare & other) const {
			return (file == other.file) && (rank == other.rank);
		}
		bool operator!=(const ChessSquare & other) const {
			return !(*this == other);
		}
	};

	struct ChessMove {
		ChessSquare from;
		ChessSquare to;

		bool operator==(const ChessMove & other) const {
			return (from == other.from) && (to == other.to);
		}
		bool operator!=(const ChessMove & other) const {
			return !(*this == other);
		}
	};

	inline std::string moveToString(const ChessMove & move) {
		std::string ret = "";
		ret.push_back(move.from.file + 'A');
		ret.push_back(move.from.rank + '1');
		ret.append(" to ");
		ret.push_back(move.to.file + 'A');
		ret.push_back(move.to.rank + '1');
		return ret;
	}

	inline std::optional<ChessSquare> squareFromString(const std::string & s) {
		if (s.size() != 2 || s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[2] > '8') {
			return std::nullopt;
		}
		else {
			return ChessSquare{ s[0] - 'a', s[1] - '1' };
		}
	}

}


#endif // CYBERPAWN_CYBERPAWN_CHESS_MOVE_H_INCLUDED
