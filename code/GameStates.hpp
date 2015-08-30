#ifndef __GAMESTATES_HPP__
#define __GAMESTATES_HPP__

#include <Common/gamestate.hpp>
#include "Worlds.hpp"
#include "Maps.hpp"
#include "Defs.hpp"

class Pacman;
class Blinky;
class Pinky;
class Inky;
class Clyde;

/** This class represents the state at the beginning of the game.
	It is the state to select options, to see scores,... */
class GameStateInitial : public QGAMES::GameState 
{
	public:
	GameStateInitial (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESSTATEINITIAL, g),
		  _options (),
		  _blinky (NULL),
		  _counter (0),
		  _optionHighlighted (0),
		  _aWorld (NULL)
						{ }

	/** Moving the cursor (blinky) to the next option. */
	void nextOption ();
	/** Moving the cursor (blinky) to the previous option. */
	void previousOption ();
	/** An option has been selected. */
	void optionSelected ();

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	Blinky* _blinky;
	std::vector <QGAMES::Form*> _options;
	QGAMES::World* _aWorld;

	// Implementatation
	int _counter;
	int _optionHighlighted;
};

/** This class represents the state before the game starts.
	The initial music sounds, the maze is plenty of balls, etc.. */
class GameStatePrelude : public QGAMES::GameState 
{
	public:
	GameStatePrelude (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPRELUDE, g),
		  _pacman (NULL), _blinky (NULL), _pinky (NULL), _inky (NULL), _clyde (NULL),
		  _counter (0),
		  _direction (1),
		  _counterState (0),
		  _stateFinishes (false),
		  _aWorld (NULL),
		  _aMap (NULL)
							{ }

	bool stateFinishes () const
							{ return (_stateFinishes); }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	Blinky* _blinky;
	Pinky*	_pinky;
	Inky*	_inky;
	Clyde*	_clyde;
	QGAMES::Form* _textReady;
	QGAMES::World* _aWorld;
	MapMaze* _aMap; 

	// Implementatation
	/** The ghosts are movement are the center, 
		and the pacman is opening and closing the mouth. */
	int _counter; 
	int _direction;
	int _counterState;
	bool _stateFinishes;
};

/** This class represents the state before the play starts....but very short
	Usually the precious state is once, and this one several times. */
class GameStatePreludeShort : public QGAMES::GameState 
{
	public:
	GameStatePreludeShort (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPRELUDESHORT, g),
		  _pacman (NULL), _blinky (NULL), _pinky (NULL), _inky (NULL), _clyde (NULL),
		  _counter (0),
		  _direction (1),
		  _counterState (0),
		  _aWorld (NULL),
		  _aMap (NULL)
							{ }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	Blinky* _blinky;
	Pinky*	_pinky;
	Inky*	_inky;
	Clyde*	_clyde;
	QGAMES::Form* _textReady;
	QGAMES::World* _aWorld;
	MapMaze* _aMap;

	// Implementatation
	/** The ghosts are movement are the center, 
		and the pacman is opening and closing the mouth. */
	int _counter; 
	int _direction;
	int _counterState;
};

/** The central state of the game.
	Everybody is moving... */
class GameStatePlaying : public QGAMES::GameState 
{
	public:
	GameStatePlaying (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPLAYING, g),
		  _pacman (NULL), _blinky (NULL), _pinky (NULL), _inky (NULL), _clyde (NULL),
		  _counter (0),
		  _direction (1)
							{ }

	virtual void onEnter ();
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	Blinky* _blinky;
	Pinky*	_pinky;
	Inky*	_inky;
	Clyde*	_clyde;

	// Implementatation
	/** The ghosts are movement are the center, 
		and the pacman is opening and closing the mouth. */
	int _counter; 
	int _direction;
};

/** The state when the maze has been clean.
	The maze blinks, and the pacman remains at its last position. */
class GameStateMazeClean : public QGAMES::GameState
{
	public:
	GameStateMazeClean (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEMAZECLEAN, g),
		  _pacman (NULL),
		  _counterBlink (0),
		  _counterState (0),
		  _aWorld (NULL),
		  _aMap (NULL)
							{ }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	QGAMES::World* _aWorld;
	MapMaze* _aMap;

	// Implementatation
	int _counterBlink;
	int _counterState; 
};

/** The state when the pacman has been killed. */
class GameStatePacmanDies : public QGAMES::GameState
{
	public:
	GameStatePacmanDies (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPACMANDIES, g),
		  _pacman (NULL), 
		  _counterState (0),
		  _counterDie (0),
		  _aWorld (NULL),
		  _aMap (NULL)
							{ }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	QGAMES::World* _aWorld;
	MapMaze* _aMap;

	// Implementatation
	int _counterDie;
	int _counterState; 
};

/** The state when a round has been finished. */
class GameStateRoundEnd : public QGAMES::GameState
{
	public:
	GameStateRoundEnd (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEROUNDEND, g),
		  _pacman (NULL), 
		  _counterState (0),
		  _aWorld (NULL)
							{ }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	QGAMES::World* _aWorld;

	// Implementatation
	int _counterState; 
};

/** The state when evrything has finishes. */
class GameStateEnd : public QGAMES::GameState 
{
	public:
	GameStateEnd (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEEND, g),
		  _pacman (NULL), _blinky (NULL), _pinky (NULL), _inky (NULL), _clyde (NULL),
		  _counter (0),
		  _direction (1),
		  _counterState (0),
		  _stateFinishes (false),
		  _aWorld (NULL),
		  _aMap (NULL)
							{ }

	bool stateFinishes () const
							{ return (_stateFinishes); }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// The main elements of the game...
	Pacman*	_pacman;
	Blinky* _blinky;
	Pinky*	_pinky;
	Inky*	_inky;
	Clyde*	_clyde;
	QGAMES::World* _aWorld;
	MapMaze* _aMap;

	// Implementatation
	/** The ghosts are movement are the center, 
		and the pacman is opening and closing the mouth. */
	int _counter; 
	int _direction; 
	int _counterState;
	bool _stateFinishes;
};

#endif
