#pragma once
#include <utility>
#include <functional>
#include <vector>

namespace Reversi
{
	enum class PLAYER_COLOR
	{
		BLACK = -1,
		WHITE = 1
	};

	enum class SLOT_STATUS
	{
		EMPTY,
		BLACK,
		WHITE
	};

	enum class GAME_STATE
	{
		BLACK_TURN,
		WHITE_TURN,
		BLACK_WON,
		WHITE_WON,
		DRAW
	};

	typedef std::pair<int, int> BoardPosition;

	typedef std::vector<BoardPosition> BoardPositionVector;

	typedef std::function<void()> Callback;

	typedef std::function<void(BoardPosition)> PositionCallback;
}
