#ifndef __INPUTHANDLER_HPP__
#define __INPUTHANDLER_HPP__

#include <SDL/sdlinputhandler.hpp>

/** Class to read the events managed by the game. */
class InputHandler : public SDLInputHandler
{
	public:
	InputHandler (QGAMES::Game* g)
		: SDLInputHandler (g),
		  _lastKey (-1)
							{ }

	virtual void onKeyboardEvent (QGAMES::KeyboardEventData* dt);

	private:
	/** Moves pacman to the left. */
	void pacmanLeft ();
	/** Moves pacman to the right. */
	void pacmanRight ();
	/** Moves pacman up. */
	void pacmanUp ();
	/** Moves pacman down. */
	void pacmanDown ();

	private:
	/** The last key pushes. */
	int _lastKey;
};

#endif