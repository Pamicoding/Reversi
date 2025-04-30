#pragma once
#include "ReversiDefine.h"
#include "IReversiContext.h"

namespace Reversi
{
	/**
	* Implementations of this interface display the game visually to the player.
	*/
	class IReversiView
	{
	public:
		/**
		* Initialize your UI API
		*/
		virtual void Initialize() = 0;

		/**
		* Displays the game start sequence.
		* This function is called at the beginning of the game to show any introductory animations or messages.
		* 
		* \param context context A pointer to the game context, which provides the initial board state and other relevant data.
		* \param endCallback A callback function that is triggered when the game start sequence is completed.
		*/
		virtual void ShowGameStarting(IReversiContext* context, Callback endCallback) = 0;
		
		/**
		* Displays the animation or effect for placing a disk on the board.
		* This function is called when a player makes a valid move and places a piece.
		* 
		* \param color The color of the player's disk being placed (BLACK or WHITE).
		* \param position The board position where the disk is being placed.
		* \param endCallback A callback function that is triggered when the placement animation or effect is completed.
		*/
		virtual void ShowDiskPlacing(PLAYER_COLOR color, BoardPosition position, Callback endCallback) = 0;

		/**
		* Displays the animation or effect for flipping disks on the board.
		* This function is called after a player places a disk and the affected opponent's pieces are flipped.
		* 
		* \param positions A vector of board positions where disks will be flipped.
		* \param endCallback A callback function that is triggered when the flipping animation or effect is completed.
		*/
		virtual void ShowDiskFlipping(const BoardPositionVector& positions, Callback endCallback) = 0;
		
		/**
		* Displays the transition effect when the game state changes.
		* This function is called whenever the game moves to a new state, such as switching turns or reaching the endgame.
		* 
		* \param state The new game state after the transition.
		* \param prevState The previous game state before the transition.
		* \param endCallback A callback function that is triggered when the state transition animation or effect is completed.
		*/
		virtual void ShowStateChanging(GAME_STATE state, GAME_STATE prevState, Callback endCallback) = 0;

		/**
		* Displays a preview of all valid move positions for a player.
		* This function is typically called at the start of a player's turn to highlight possible placements.
		* 
		* \param targetColor The color of the player whose valid moves are being displayed (BLACK or WHITE).
		* \param positions A vector of board positions where the player can legally place a disk.
		*/
		virtual void ShowPlaceablePreview(PLAYER_COLOR targetColor, const BoardPositionVector& positions) = 0;

		/**
		* Clear all placeable preview being displayed.
		*/
		virtual void ClearPlaceablePreview() = 0;

		/**
		* Displays a preview of all capturable peices of a move.
		* This function is typically called when the player hover on a board positions before placement.
		* 
		* \param positions A vector of board positions where all capturable peices is.
		*/
		virtual void ShowCapturablePreview(const BoardPositionVector& positions) = 0;

		/**
		* Clear all capturable preview being displayed.
		*/
		virtual void ClearCapturablePreview() = 0;
		virtual void ShowWinner(GAME_STATE state) = 0;
		virtual void SetRestartCallback(Callback cb) = 0;
		virtual void ResetWindow(IReversiContext* context) = 0;
		virtual void SetTimeoutCallback(std::function<void()> cb) = 0;
		virtual const std::vector<BoardPosition>& getPlaceablePreviewPositions() const = 0;
	};
}