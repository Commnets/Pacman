/** \addtogroup Pacman */
/*@{*/

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

/** A class to define text drawn during the presentation. */
class PacmanPresentationText : public QGAMES::ScoreObjectText
{
	public:
	PacmanPresentationText (const std::string& t = std::string (__NULL_STRING__));

	virtual Entity* clone ()
							{ return (new PacmanPresentationText (_text)); }

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__LETTERSFORM); }
};

/** When the game is loading. */
class PacmanGameStateLoading : public QGAMES::GameState 
{
	public:
	PacmanGameStateLoading (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATELOADING, g),
		  _counter (0),
		  _background (NULL),
		  _logo (NULL),
		  _text (NULL),
		  _fadeOut (false),
		  _counterAlpha (255)
							{ }

	/** @see parent. */
	void onEnter ();
	void updatePositions ();
	void drawOn (QGAMES::Screen* s);
	void onExit ();

	private:
	/** To count how long the logo is in the screen... */
	int _counter; 
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo to be shown... */
	QGAMES::Form* _logo; 
	/** The text refering to the author. */
	PacmanPresentationText* _text;
	/** The logo start to fade out after a couple of seconds.. */
	bool _fadeOut; 
	/** The level of fade out... */
	int _counterAlpha; 
};

/** This class represents the state at the beginning of the game.
	It is the state to select options, to see scores,... */
class GameStateInitial : public QGAMES::GameState 
{
	public:
	GameStateInitial (QGAMES::Game* g);

	void nextOption ();
	void previousOption ();
	bool isMovingToAnOption () const
							{ return (_movingToOption != 0); }
	void optionSelected ();
	void optionAt (const QGAMES::Position& pos);

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	// Implementation
	/** Blinky is part of the selection. */
	Blinky* _blinky;
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo form, shown in this state */
	QGAMES::Form* _logo; 
	/** The options to be selected. */
	std::vector <std::string> _options;
	/** The levels potentially selected. */
	std::vector <std::string> _optionLevels;
	/** ...and the entites representing them. */
	std::vector <PacmanPresentationText*> _optionGraphics;
	/** The current option selected. */
	int _currentOption;
	/** The current option related with the level. */
	int _currentOptionLevel;
	/** Counter for the things moving... */
	int _counterMovement;
	/** The selection moves little by little.
		This parameters marks how much is it moving. */
	int _positionOption;
	/** To indicate the type of movement. */
	int _movingToOption;
	/** The figure that is being shown. */
	int _figure;
	/** The type of control selected: 0 keyboard, 1 joystick. */
	int _typeOfControl;
	/** Blinking attribute. */
	int _blinkAttr;
	/** Direction of the blinking. */
	int _blinkDirection;
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
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
		  _aMap (NULL),
		  _wantToExit (false)
							{ }

	bool stateFinishes () const
							{ return (_stateFinishes); }

	void setWantToExit (bool s)
							{ _wantToExit = s; }

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
	bool _wantToExit;
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
	GameStateRoundEnd (QGAMES::Game* g);

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

	// Implementatation
	int _counterState; 
	int _counterForm;
	int _counterToChangeForm;
	bool _runningAway;
	QGAMES::Position _position;
	int _iFPacman, _iFBlinky, _iFPinky, _iFInky, _iFClyde;
	QGAMES::Vector _mov, _inc;
	bool _finishes;
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

/** This class represents the state to introduce the initial of the player
	after the game ends. */
class GameStateIntroLetters : public QGAMES::GameState 
{
	public:
	GameStateIntroLetters (QGAMES::Game* g);

	void nextLetter ();
	void previousLetter ();
	void letterSelected ();

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo form, shown in this state */
	QGAMES::Form* _logo; 
	/** The number of letter that is blinking.
		It goes from 0 to 2. Only three letters can be modified as in the original game. */
	int _nLetter;
	/** The character ascii that is been displyed in that moment. */
	int _cLetter;
	/** Blinking attribute. */
	int _blinkAttr;
	/** Direction of the blinking. */
	int _blinkDirection;
	/** The letters. */
	char _letters [4];
	/** The score to add in number. */
	int _nScore;
	/** Max NUmber of scores. */
	int _nMScores;
	/** Score to add. */
	std::string _sToAdd;

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** A vector with the initial introduced so far. */
	std::vector <PacmanPresentationText*> _initials;
	/** The scores. */
	std::vector <PacmanPresentationText*> _tScores;
	/** The position of the new initials within the list of initials. */
	int _posNewInitials;
	/** A chain of ceros. */
	std::string _ceros;
};

/** This class represents the state to see the scores. */
class GameStateSeeScore  : public QGAMES::GameState 
{
	public:
	GameStateSeeScore (QGAMES::Game* g);

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	void setWantToExit (bool e)
							{ _wantToExit = e; }

	private:
	/** The background to be shown. */
	QGAMES::Form* _background;
	/** The logo form, shown in this state */
	QGAMES::Form* _logo; 

	// Implementation
	/** When the input handler detects an special key has been pressed
		and the game stands in this state this variable is set to true. */
	bool _wantToExit;
	/** A vector with the initial introduced so far. */
	std::vector <PacmanPresentationText*> _initials;
	/** The scores. */
	std::vector <PacmanPresentationText*> _tScores;
	/** A chain of ceros. */
	std::string _ceros;
};

#endif
  
// End of the file
/*@}*/