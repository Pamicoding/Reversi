#pragma once
#include "ReversiDefine.h"

namespace Reversi
{
	/**
	* Implementations of this interface capture player interactions, such as selecting moves.
	*/
	class IReversiInput
	{
	public:
		/**
		* Waits for the player to input a preview position.
		* This function is used to notify the game time to highlight all possible capturable before placing a piece on specified position.
		* 
		* \param callback A callback function that is triggered when the player selects a preview position.
		*                 For example, if the player hovers over (3,4), the callback will be called with (3,4).
		*/
		virtual void WaitPreviewInput(PositionCallback callback) = 0;

		/**
		* Waits for the player to input a move for placing a piece.
		* This function is used to notify the game the player want to make a move.
		* 
		* \param callback A callback function that is triggered when the player selects a position to place a piece.
		*                 For example, if the player clicks on (5,3) to place a piece, the callback will be called with (5,3).
		*/
		virtual void WaitPlacingInput(PositionCallback callback) = 0;
		
		/**
		* Enables or disables input locking.
		* When input is locked, player interactions (such as placing a piece) are ignored.
		* This is useful for preventing input during animations or state transitions.
		* 
		* \param lockState If true, input will be locked; if false, input will be enabled.
		*/
		virtual void SetInputLock(bool lockState) = 0;
	};
}


