#pragma once
#include <cstdlib>
#include <ctime>
#include "IReversiContext.h"
#include "IReversiInput.h"
#include "IReversiView.h"

namespace Reversi
{
	/**
	* Represents the core game logic for Reversi.
	* Manages the game state, processes player moves, and coordinates with the view and input.
	*/
	class ReversiGame
	{
	private:
		IReversiContext* context;
		IReversiInput* input;
		IReversiView* view;
		BoardPositionVector placeablePositions;
		BoardPositionVector positionBuffer;

		void StartGame();
		void WaitForPlayerInput();
		BoardPositionVector GetFlippableDisks(BoardPosition placingPosition);
		//bool ProcessDiskPlacement(BoardPosition placingPosition);
		void ProcessStateChange();
		void RestartGame();
		void HandleTimeout();

	public:
		ReversiGame(IReversiContext*, IReversiInput*, IReversiView*);
		void Execute();
		bool ProcessDiskPlacement(BoardPosition placingPosition);
	};
}
