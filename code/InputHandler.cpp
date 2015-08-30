#include "InputHandler.hpp"
#include "GameStates.hpp"
#include "Entities.hpp"
#include "General.hpp"
#include "Defs.hpp"
#include <Arcade/arcadegame.hpp>
#include <iostream>

// ---
void InputHandler::onKeyboardEvent (QGAMES::KeyboardEventData* dt)
{  
	int kPressed = -1;
	bool pKey = false;
	if (_lastKey != -1)
	{
		if (!dt -> _on)
		{
			kPressed = _lastKey;
			_lastKey = -1;
			pKey = true;
		}
	}
	else
		_lastKey = dt -> _key;
	if (!pKey)
		return;
	// If no key has been pressed, then it returns...

	// When the state is the initial
	// There are several options that can be selected...
	if (_game -> activeState () -> type () == __GAMESSTATEINITIAL)
	{
		if (kPressed == SDL_SCANCODE_DOWN) // Only the first pressed is taken into account...
			((GameStateInitial*) _game -> activeState ()) -> nextOption ();
		if (kPressed == SDL_SCANCODE_UP)
			((GameStateInitial*) _game -> activeState ()) -> previousOption ();
		if (kPressed == SDL_SCANCODE_RETURN ||
			kPressed == SDL_SCANCODE_RETURN2)
		{
			((GameStateInitial*) _game -> activeState ()) -> optionSelected ();
			clearPendingEvents ();
		}

		return;
	}

	// When the state is the prelude, the music is sound for a while...
	// But if any key is pressed, then the game starts
	if (_game -> activeState () -> type () == __GAMESTATEPRELUDE)
	{
		if (((GameStatePrelude*) (_game -> activeState ())) -> stateFinishes ())
			_game -> setState (__GAMESTATEPLAYINGNAME); // The game is playing...
		return;
	}
	
	// If the game is happening, the keys are needed to move
	// The pacman...
	if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
	{
		if (kPressed == __PACMANLEFT)
			pacmanLeft ();
		if (kPressed == __PACMANRIGHT)
			pacmanRight ();
		if (kPressed == __PACMANUP)
			pacmanUp ();
		if (kPressed == __PACMANDOWN)
			pacmanDown ();

		return;
	}	

	// When the pacman has died, and the state is the end,
	// and any key is pressed the game starts again...
	if (_game -> activeState () -> type () == __GAMESTATEEND)
	{
		if (((GameStateEnd*) (_game -> activeState ())) -> stateFinishes ())
			_game -> setState (__GAMESTATEINITIALNAME); // Start again...
		return;
	}
}

// ---
void InputHandler::pacmanLeft ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._left);
}

// ---
void InputHandler::pacmanRight ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._right);
}

// ---
void InputHandler::pacmanUp ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._up);
}

// ---
void InputHandler::pacmanDown ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._down);
}
