#include "GameStates.hpp"
#include "Game.hpp"
#include "Entities.hpp"
#include "General.hpp"

// ---
void GameStateInitial::nextOption ()
{
	if (_optionHighlighted < 3)
		_optionHighlighted++;
}

// ---
void GameStateInitial::previousOption ()
{
	if (_optionHighlighted > 0)
		_optionHighlighted--;
}

// ---
void GameStateInitial::optionSelected ()
{
	if (_optionHighlighted == 2)
	{
		((PacmanGame*) _game) -> setWorld (__WORLDPACMAN);
		((PacmanGame*) _game) -> activeWorld () -> initialize ();
		((PacmanGame*) _game) -> observe (((PacmanGame*) _game) -> activeWorld ());
		_game -> setState (__GAMESTATEPRELUDENAME);
	}

	if (_optionHighlighted == 3)
		_game -> exitGame ();
}

// ---
void GameStateInitial::onEnter ()
{
	// The score objects are not valid in this stage...
	_game -> removeScoreObjects ();

	// Entering, there is not still world alive...
	// Just to avoid entities to be managed by the main game loop...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// Fix the screen atthe beginning...
	_game -> mainScreen () -> setPosition (QGAMES::Vector::_cero);

	// Create an array with the forms...
	_options.clear ();
	_options.resize (4);
	_options [0] = _game -> form (__OPTIONSFORM);
	_options [1] = _game -> form (__SCORESFORM);
	_options [2] = _game -> form (__PLAYGAMEFORM);
	_options [3] = _game -> form (__EXITFORM);

	// Blinky is the selector
	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> setCurrentForm (__SPRITESFORM);
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME);
	_blinky -> setPosition (QGAMES::Position (__BD 50, 
		__BD (150 + (_optionHighlighted * 75)), __BD 0));
}

// ---
void GameStateInitial::updatePositions ()
{
	// Two times per second...
	// Totally relaxed..the music is sound...no problems in mind...yet!
	if (_counter++ >= (_game -> framesPerSecond () / 2)) 
	{
		_counter = 0;
		_blinky -> setPosition (QGAMES::Position (__BD 50, 
			__BD (150 + (_optionHighlighted * 75)), __BD 0));
		_blinky -> setCurrentAspect ((_blinky -> currentAspect () == __BLINKYRIGHTMAXFRAME)
			? __BLINKYRIGHTMINFRAME : _blinky -> currentAspect () + 1);
	}
}

// ---
void GameStateInitial::drawOn (QGAMES::Screen* s)
{
	QGAMES::Screen* scr = _game -> mainScreen ();
	for (int i = 0; i < 4; i++)
		_options [i] -> drawOn (s, 0, 
			QGAMES::Position (__BD ((scr -> visualWidth () - _options [i] -> width ()) / 2), 
				__BD (150 + (i * 75)), __BD 0));

	_blinky -> drawOn (s);
}

// ---
void GameStateInitial::onExit ()
{
	// Reestablish the world...
	// so the arcade game takes back the control of the game
	if (_aWorld)
		((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
}

// ---
void GameStatePrelude::onEnter ()
{
	_counter = 0;
	_counterState = 0;
	_direction = 1;
	_stateFinishes = false;

	// Entering, there is not still world alive...
	// Just to avoid entities to be managed by the main game loop...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	_aMap = (MapMaze*) (_aWorld -> activeScene () -> activeMap ());
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// Gets the name of the current map...
	// and some data from it, usefull to draw the initial position of the monsters...
	QGAMES::Position tPos = _aMap -> centerTilePenPosition ();
	int tW = _aMap -> tileWidth (); int tH = _aMap -> tileHeight ();

	// The map must to be initialized...
	// That is the points to be set to "fill" form, just in case
	_aMap -> initialize ();
	// Put the score objects to 0, ready to start...
	((PacmanGame*) _game) -> initScore ();

	// The begging sound starts....
	// and fix the screen...
	_game -> sound (__SOUNDBEGINNING) -> play (-1);
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD _aMap -> tileWidth () * General::_e._right);
	
	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	_pacman -> initialize ();
	_pacman -> setCurrentForm (__SPRITESFORM);
	_pacman -> setCurrentAspect (__PACMANLEFTMINFRAME);
	_pacman -> setCurrentMovement (__MININT__);
	_pacman -> setMove (QGAMES::Vector::_cero); // Don't move...
	_pacman -> setPosition (_aMap -> initialPacmanPosition ());
	/** The orientation makes no sense. */

	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> initialize ();
	_blinky -> setCurrentForm (__SPRITESFORM);
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME);
	_blinky -> setCurrentMovement (__MININT__);
	_blinky -> setPosition (tPos + tW * _blinky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_pinky = (Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY);
	_pinky -> initialize ();
	_pinky -> setCurrentForm (__SPRITESFORM);
	_pinky -> setCurrentAspect (__PINKYRIGHTMINFRAME);
	_pinky -> setCurrentMovement (__MININT__);
	_pinky -> setPosition (tPos + tW * _pinky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_inky = (Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY);
	_inky -> initialize ();
	_inky -> setCurrentForm (__SPRITESFORM);
	_inky -> setCurrentAspect (__INKYRIGHTMINFRAME);
	_inky -> setCurrentMovement (__MININT__);
	_inky -> setPosition (tPos + tW * _inky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_clyde = (Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE);
	_clyde -> initialize ();
	_clyde -> setCurrentForm (__SPRITESFORM);
	_clyde -> setCurrentAspect (__CLYDERIGHTMINFRAME);
	_clyde -> setCurrentMovement (__MININT__);
	_clyde -> setPosition (tPos + tW * _clyde -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	// Text indicating everything is ready...
	_textReady = _game -> form (__READYFORM);
}

// ---
void GameStatePrelude::updatePositions ()
{
	// Six times per second...
	// They are still relaxed...the palyer is still getting ready!
	if (_counter++ >= (_game -> framesPerSecond () / 6)) 
	{
		_counter = 0;
		_direction = -_direction;
	
		/** Pacman's aspect doesn't change. */

		_blinky -> setCurrentAspect ((_blinky -> currentAspect () >= __BLINKYRIGHTMAXFRAME)
			? __BLINKYRIGHTMINFRAME : _blinky -> currentAspect () + 1);
		_pinky -> setCurrentAspect ((_pinky -> currentAspect () >= __PINKYRIGHTMAXFRAME)
			? __PINKYRIGHTMINFRAME : _pinky -> currentAspect () + 1);
		_inky -> setCurrentAspect ((_inky -> currentAspect () >= __INKYRIGHTMAXFRAME)
			? __INKYRIGHTMINFRAME : _inky -> currentAspect () + 1);
		_clyde -> setCurrentAspect ((_clyde -> currentAspect () >= __CLYDERIGHTMAXFRAME)
			? __CLYDERIGHTMINFRAME : _clyde -> currentAspect () + 1);
	}

	// The monsters move up and down, out of one specific movement...
	_blinky -> setPosition (_blinky -> position () - 
		(QGAMES::Vector (__BD 0, __BD -0.5 * _direction, __BD 0)));
	_blinky -> setOrientation (QGAMES::Vector (__BD 0, __BD -_direction, __BD 0));
	_pinky -> setPosition (_pinky -> position () - 
		(QGAMES::Vector (__BD 0, __BD 0.5 * _direction, __BD 0)));
	_pinky -> setOrientation (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));
	_inky -> setPosition (_inky -> position () - 
		(QGAMES::Vector (__BD 0, __BD -0.5 * _direction, __BD 0)));
	_inky -> setOrientation (QGAMES::Vector (__BD 0, __BD -_direction, __BD 0));
	_clyde -> setPosition (_clyde -> position () - 
		(QGAMES::Vector (__BD 0, __BD 0.5 * _direction, __BD 0)));
	_clyde -> setOrientation (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));

	_counterState++;
	// At the second 2, the game is ready to start...
	if (_counterState == (2 * _game -> framesPerSecond ()))
		_stateFinishes = true; 
	// At the second five (when music finishes), the game starts automatically...
	if (_counterState == (5 * _game -> framesPerSecond ()))
		_game -> setState (__GAMESTATEPLAYINGNAME); 
}

// ---
void GameStatePrelude::drawOn (QGAMES::Screen* s)
{
	// Draws the current map...
	_aMap -> drawOn (s, QGAMES::Position::_cero);
	
	// The pacman appears at the second three...
	if (_counterState > (3 * _game -> framesPerSecond ()))
		_pacman -> drawOn (s);
	_blinky -> drawOn (s);
	_pinky -> drawOn (s);
	_inky -> drawOn (s);
	_clyde -> drawOn (s);

	// The text ready is on the screen for three seconds...
	if (_counterState <= (3 * _game -> framesPerSecond ()))
		_textReady -> drawOn (s, 0, _aMap -> fruitPosition () - 
			__BD ((_textReady -> frameWidth () / 2) - (_aMap -> tileWidth () / 2)) * General::_e._right);
}

// ---
void GameStatePrelude::onExit ()
{
	// Reestablish the world...
	// so the arcade game takes back the control of the game
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
	// Fix the number of lives...
	((PacmanGame*) _game) -> setLives (_MAXNUMBEROFLIVES);
}

// ---
void GameStatePreludeShort::onEnter ()
{
	_counter = 0;
	_counterState = 0;
	_direction = 1;

	// The map is not initialized,
	// Becasue this state can be used both when the maze has been clean up
	// or when the pacman dies and there is still lives remaining...

	// Entering, there is not still world alive...
	// Just to avoid entities to be managed by the main game loop...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	_aMap = (MapMaze*) (_aWorld -> activeScene () -> activeMap ());
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// Gets the name of the current map...
	// and some data from it, usefull to draw the initial position of the monsters...
	QGAMES::Position tPos = _aMap -> centerTilePenPosition ();
	int tW = _aMap -> tileWidth (); int tH = _aMap -> tileHeight ();

	// Fix the main screen...just in case....
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD _aMap -> tileWidth () * General::_e._right);
	
	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	// The initialization here makes no sense, because
	// It has to converserve the behaviour of the last scenario...
	_pacman -> setCurrentForm (__SPRITESFORM);
	_pacman -> setCurrentAspect (__PACMANLEFTMINFRAME);
	_pacman -> setCurrentMovement (__MININT__);
	_pacman -> setMove (QGAMES::Vector::_cero); // Don't move...
	_pacman -> setPosition (_aMap -> initialPacmanPosition ());
	/** The orientation makes no sense. */

	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	// The initialization here makes no sense, because
	// It has to converserve the behaviour of the last scenario...
	_blinky -> setCurrentForm (__SPRITESFORM);
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME);
	_blinky -> setCurrentMovement (__MININT__);
	_blinky -> setPosition (tPos + tW * _blinky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_pinky = (Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY);
	// The initialization here makes no sense, because
	// It has to converserve the behaviour of the last scenario...
	_pinky -> setCurrentForm (__SPRITESFORM);
	_pinky -> setCurrentAspect (__PINKYRIGHTMINFRAME);
	_pinky -> setCurrentMovement (__MININT__);
	_pinky -> setPosition (tPos + tW * _pinky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_inky = (Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY);
	// The initialization here makes no sense, because
	// It has to converserve the behaviour of the last scenario...
	_inky -> setCurrentForm (__SPRITESFORM);
	_inky -> setCurrentAspect (__INKYRIGHTMINFRAME);
	_inky -> setCurrentMovement (__MININT__);
	_inky -> setPosition (tPos + tW * _inky -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	_clyde = (Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE);
	// The initialization here makes no sense, because
	// It has to converserve the behaviour of the last scenario...
	_clyde -> setCurrentForm (__SPRITESFORM);
	_clyde -> setCurrentAspect (__CLYDERIGHTMINFRAME);
	_clyde -> setCurrentMovement (__MININT__);
	_clyde -> setPosition (tPos + tW * _clyde -> tilesAtBeginning () * General::_e._right);
	/** The orientation will be determinated later on by the movement. */

	// Text indicating everything is ready...
	_textReady = _game -> form (__READYFORM);
}

// ---
void GameStatePreludeShort::updatePositions ()
{
	// Ten times per second...
	// The monsters are excited for scattering pacman!
	// so they moves quickly
	if (_counter++ >= (_game -> framesPerSecond () / 6)) 
	{
		_counter = 0;
		_direction = -_direction;

		/** Pacman aspect doesn't change */

		_blinky -> setCurrentAspect ((_blinky -> currentAspect () >= __BLINKYRIGHTMAXFRAME)
			? __BLINKYRIGHTMINFRAME : _blinky -> currentAspect () + 1);
		_pinky -> setCurrentAspect ((_pinky -> currentAspect () >= __PINKYRIGHTMAXFRAME)
			? __PINKYRIGHTMINFRAME : _pinky -> currentAspect () + 1);
		_inky -> setCurrentAspect ((_inky -> currentAspect () >= __INKYRIGHTMAXFRAME)
			? __INKYRIGHTMINFRAME : _inky -> currentAspect () + 1);
		_clyde -> setCurrentAspect ((_clyde -> currentAspect () >= __CLYDERIGHTMAXFRAME)
			? __CLYDERIGHTMINFRAME : _clyde -> currentAspect () + 1);
	}

	// The monsters move up and down...
	_blinky -> setPosition (_blinky -> position () - 
		(QGAMES::Vector (__BD 0, __BD -0.5 * _direction, __BD 0)));
	_blinky -> setOrientation (QGAMES::Vector (__BD 0, __BD -_direction, __BD 0));
	_pinky -> setPosition (_pinky -> position () - 
		(QGAMES::Vector (__BD 0, __BD 0.5 * _direction, __BD 0)));
	_pinky -> setOrientation (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));
	_inky -> setPosition (_inky -> position () - 
		(QGAMES::Vector (__BD 0, __BD -0.5 * _direction, __BD 0)));
	_inky -> setOrientation (QGAMES::Vector (__BD 0, __BD -_direction, __BD 0));
	_clyde -> setPosition (_clyde -> position () - 
		(QGAMES::Vector (__BD 0, __BD 0.5 * _direction, __BD 0)));
	_clyde -> setOrientation (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));

	_counterState++;
	// At the second three, the game continues automatically...
	if (_counterState == (3 * _game -> framesPerSecond ()))
		_game -> setState (__GAMESTATEPLAYINGNAME); 
}

// ---
void GameStatePreludeShort::drawOn (QGAMES::Screen* s)
{
	// Draws the current map...
	_aMap -> drawOn (s, QGAMES::Position::_cero);
	
	// The pacman appears at the second three...
	if (_counterState > (3 * _game -> framesPerSecond ()))
		_pacman -> drawOn (s);
	_blinky -> drawOn (s);
	_pinky -> drawOn (s);
	_inky -> drawOn (s);
	_clyde -> drawOn (s);

	// The text ready is on the screen for three seconds...
	if (_counterState <= (3 * _game -> framesPerSecond ()))
		_textReady -> drawOn (s, 0, _aMap -> fruitPosition () - 
			__BD ((_textReady -> frameWidth () / 2) - (_aMap -> tileWidth () / 2)) * General::_e._right);
}

// ---
void GameStatePreludeShort::onExit ()
{
	// Reestablish the world...
	// so the arcade game takes the control back
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
}

// ---
void GameStatePlaying::onEnter ()
{
	// Just in case...
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD ((__TM (((PacmanGame*) _game) -> activeWorld () -> activeScene () -> activeMap ())) 
			-> tileWidth ()) * General::_e._right);
	// Normal status...alwyas when starting...
	((PacmanGame*) _game) -> setMode (PacmanGame::_NORMAL);

	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	_pacman -> setState (PacmanArtist::_ALIVE); // It is alive
	_pacman -> setCurrentForm (__SPRITESFORM);
	/** As the pacman in not moving initially...
		and aspect should be stayes, and looking to the left has been selected. */
	_pacman -> setCurrentAspect (__PACMANLEFTMINFRAME);
	/** The only movement possible for the pacman is being part of the maze. */
	_pacman -> setCurrentMovement (__MOVMAZEPACMAN);
	/** The position is in the middle of the maze...or the position parametrized */
	_pacman -> setPosition (((MapMaze*) (((PacmanGame*) _game) -> activeWorld () -> 
		activeScene () -> activeMap ())) -> initialPacmanPosition ());
	_pacman -> setOrientation (QGAMES::Vector::_cero);

	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> setState (PacmanArtist::_ALIVE); // It is alive, but in the mode previous...
	_blinky -> setCurrentForm (__SPRITESFORM);
	/** It is not neccesary to set the aspect,
		because it is determinated by the movement, but just to avoid an weak blinking effect... */
	_blinky -> setCurrentAspect (__BLINKYLEFTMINFRAME);
	/** Initially the monster is in the pen. */
	_blinky -> setCurrentMovement (__MOVPENBLINKY);
	/** The coordinates and the orientation are the ones from the previous state.
		So, it is not needed to introduce them again. */

	_pinky = (Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY);
	_pinky -> setState (PacmanArtist::_ALIVE);
	_pinky -> setCurrentForm (__SPRITESFORM);
	/** It is not neccesary to set the aspect,
		because it is determinated by the movement, but just to avoid an weak blinking effect... */
	_pinky -> setCurrentAspect (__PINKYLEFTMINFRAME);
	/** Initially the monster is in the pen. */
	_pinky -> setCurrentMovement (__MOVPENPINKY);
	/** The coordinates and the orientation are the ones from the previous state.
		So, it is not needed to introduce them again. */

	_inky = (Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY);
	_inky -> setState (PacmanArtist::_ALIVE);
	_inky -> setCurrentForm (__SPRITESFORM);
	/** It is not neccesary to set the aspect,
		because it is determinated by the movement, but just to avoid an weak blinking effect... */
	_inky -> setCurrentAspect (__INKYLEFTMINFRAME);
	/** Initially the monster is in the pen. */
	_inky -> setCurrentMovement (__MOVPENINKY);
	/** The coordinates and the orientation are the ones from the previous state.
		So, it is not needed to introduce them again. */

	_clyde = (Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE);
	_clyde -> setState (PacmanArtist::_ALIVE);
	_clyde -> setCurrentForm (__SPRITESFORM);
	/** It is not neccesary to set the aspect,
		because it is determinated by the movement, but just to avoid an weak blinking effect... */
	_clyde -> setCurrentAspect (__CLYDELEFTMINFRAME);
	/** Initially the monster is in the pen. */
	_clyde -> setCurrentMovement (__MOVPENCLYDE);
	/** The coordinates and the orientation are the ones from the previous state.
		So, it is not needed to introduce them again. */

	// The main sound...the siren...forever...
	_game -> sound (__SOUNDSIREN) -> play (-1);
}

// ---
void GameStatePlaying::onExit ()
{
	_game -> sound (__SOUNDSIREN) -> stop (); // The sound of the siren must stop...
}

// ---
void GameStateMazeClean::onEnter ()
{
	_counterBlink = 0;
	_counterState = 0;

	// Keeps the current world, just to avoid
	// The main loop to draw and manage the presentation...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	_aMap = (MapMaze*) _aWorld -> activeScene () -> activeMap ();
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// Fix the screen...
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD _aMap -> tileWidth () * General::_e._right);

	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	/** The position, the aspect and the orientation will be same it has. */
	/** The movement has to be none. */
	_pacman -> setCurrentMovement (__MININT__);
}

// ---
void GameStateMazeClean::updatePositions ()
{
	_counterBlink++;
	if (_counterBlink >= (_game -> framesPerSecond () / 3)) // Three times per second...
	{
		_counterBlink = 0;
		MapMaze* map = 
			(MapMaze*) (_aWorld -> activeScene () -> activeMap ());
		map -> blink (!map -> isBlink ());
	}

	_counterState++;
	// At the second 3, the game starts again...
	if (_counterState == (3 * _game -> framesPerSecond ()))
	{
		// Every three rounds...changes to the state to the end of the round...
		// Otherwhise the maze is clean up and a new round starts again...
		int l = ((PacmanGame*) _game) -> level ();
		_game -> setState ((l % 3 == 0 && l != 0) 
			? __GAMESTATEROUNDENDNAME : __GAMESTATEPRELUDESHORTNAME);
	}
}

// ---
void GameStateMazeClean::drawOn (QGAMES::Screen* s)
{
	_aWorld -> activeScene () -> activeMap () -> drawOn (s, QGAMES::Position::_cero);
	_pacman -> drawOn (s); 
}

// ---
void GameStateMazeClean::onExit ()
{
	// Reestablish the world...
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
	// Clea up the maze...
	_aMap -> initialize ();
	// Increments in one the level...
	((PacmanGame*) _game) -> setLevel (((PacmanGame*) _game) -> level () + 1);

	// The monsters...are in the basic state when the maze has been clean up
	// The counters about which status it is...are also set up
	((Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY)) -> initialize ();
	((Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY)) -> initialize ();
	((Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY)) -> initialize ();
	((Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE)) -> initialize ();
}

// ---
void GameStatePacmanDies::onEnter ()
{
	_counterDie = 0;
	_counterState = 0;

	// Keeps the current world, just to avoid
	// The main loop to draw and manage the presentation...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	((PacmanGame*) _game) -> setWorld (__MININT__);
	_aMap = (MapMaze*) _aWorld -> activeScene () -> activeMap ();

	// Fix the screen...
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD _aMap -> tileWidth () * General::_e._right);

	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	/** The position, and the orientation will be same it has. */
	/** The aspect is the looking right. */
	_pacman -> setCurrentAspect (__PACMANRIGHTMINFRAME);
	/** The movement has to be none. */
	_pacman -> setCurrentMovement (__MININT__);

	// In this case, the monsters are not initialized...

	// The main sound...pacman dieing...
	_game -> sound (__SOUNDDEATH) -> play (-1);
}

// ---
void GameStatePacmanDies::updatePositions ()
{
	_counterDie++;
	// Three times per second...
	// The pacman moves...
	if (_counterDie >= (_game -> framesPerSecond () / 3)) 
	{
		_counterDie = 0;
		_pacman -> setCurrentAspect ((_pacman -> currentAspect () >= __PACMANRIGHTMAXFRAME)
			? __PACMANRIGHTMINFRAME : _pacman -> currentAspect () + 1);
	}

	_counterState++;
	// At the second 3, the game starts again...
	if (_counterState == (3 * _game -> framesPerSecond ()))
		_game -> setState ((((PacmanGame*) _game) -> lives () == 1)
			? __GAMESTATEENDNAME :  __GAMESTATEPRELUDESHORTNAME); // No more lives...
}

// ---
void GameStatePacmanDies::drawOn (QGAMES::Screen* s)
{
	_aMap -> drawOn (s, QGAMES::Position::_cero);
	_pacman -> drawOn (s); 
}

// ---
void GameStatePacmanDies::onExit ()
{
	// The level doesn't change here...
	_game -> sound (__SOUNDDEATH) -> stop (); // If any...

	// Reestablish the world...
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
	// The map has not to be initialize, as the pacman has still to clean the maze...

	// The number of lives is being reduced by one...
	((PacmanGame*) _game) -> setLives (((PacmanGame*) _game) -> lives () - 1);

	// The monsters...are in the basic state when the maze has been clean up
	// but the counters related when the status the monster is , doesn't change...
	((Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY)) -> 
		setMode (PacmanMonster::_WAITING);
	((Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY)) -> 
		setMode (PacmanMonster::_WAITING);
	((Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY)) -> 
		setMode (PacmanMonster::_WAITING);
	((Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE)) -> 
		setMode (PacmanMonster::_WAITING);
}

// ---
void GameStateRoundEnd::onEnter ()
{
	_counterState = 0;

	// Keeps the current world, just to avoid
	// The main loop to draw and manage the presentation...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// The main sound...intermission sound......
	_game -> sound (__SOUNDINTERMISSION) -> play (-1);
}

// ---
void GameStateRoundEnd::updatePositions ()
{
	_counterState++;
	// At the second five, the system moves to the next level...
	if (_counterState == (5 * _game -> framesPerSecond ()))
		_game -> setState (__GAMESTATEPRELUDESHORTNAME);
}

// ---
void GameStateRoundEnd::drawOn (QGAMES::Screen* s)
{
	// TO DO...
}

// ---
void GameStateRoundEnd::onExit ()
{
	// The sounds finishes...if any...
	_game -> sound (__SOUNDINTERMISSION) -> stop ();

	// Reestablish the world...
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
}

// ---
void GameStateEnd::onEnter ()
{
	_counterState = 0;
	_stateFinishes = false;

	// Keeps the current world, just to avoid
	// The main loop to draw and manage the presentation...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	_aMap = (MapMaze*) _aWorld -> activeScene () -> activeMap ();
	((PacmanGame*) _game) -> setWorld (__MININT__);

	// The game ends...and the music for that sounds...
	_game -> sound (__SOUNDINTERMISSION) -> play (-1);

	// Fix the screen...
	_game -> mainScreen () -> setPosition (QGAMES::Position::_cero + 
		__BD _aMap -> tileWidth () * General::_e._right);

	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	_pacman -> setCurrentForm (__SPRITESFORM);
	_pacman -> setCurrentAspect (__PACMANLEFTMINFRAME);
	/** The movement is useless. */
	/** The position is where it was in the previous state.
		So, it is not needed to actualize it. */
	_pacman -> setOrientation (QGAMES::Vector::_cero);

	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> setCurrentForm (__SPRITESFORM);
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME);
	/** The movement is useless. */
	/** The position is where it was in the previous state.
		So, it is not needed to actualize it. */
	_blinky -> setOrientation (QGAMES::Vector::_cero);

	_pinky = (Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY);
	_pinky -> setCurrentForm (__SPRITESFORM);
	_pinky -> setCurrentAspect (__PINKYRIGHTMINFRAME);
	/** The movement is useless. */
	/** The position is where it was in the previous state.
		So, it is not needed to actualize it. */
	_pinky -> setOrientation (QGAMES::Vector::_cero);

	_inky = (Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY);
	_inky -> setCurrentForm (__SPRITESFORM);
	_inky -> setCurrentAspect (__INKYRIGHTMINFRAME);
	/** The movement is useless. */
	/** The position is where it was in the previous state.
		So, it is not needed to actualize it. */
	_inky -> setOrientation (QGAMES::Vector::_cero);

	_clyde = (Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE);
	_clyde -> setCurrentForm (__SPRITESFORM);
	_clyde -> setCurrentAspect (__CLYDERIGHTMINFRAME);
	/** The movement is useless. */
	/** The position is where it was in the previous state.
		So, it is not needed to actualize it. */
	_clyde -> setOrientation (QGAMES::Vector::_cero);
}

// ---
void GameStateEnd::updatePositions ()
{
	// 6 times a second 
	// The monsters changes their aspect
	// This is a little bit more slower than normal game...
	// ...but they are finally relased after scattering pacman!
	if (_counter++ >= _game -> framesPerSecond () / 6) 
	{
		_counter = 0;
		_direction = -_direction;

		// The pacman doesn't move at the end...

		// The monsters move the clothes....
		_blinky -> setCurrentAspect ((_blinky -> currentAspect () == __BLINKYRIGHTMAXFRAME)
			? __BLINKYRIGHTMINFRAME : _blinky -> currentAspect () + 1);
		_pinky -> setCurrentAspect ((_pinky -> currentAspect () == __PINKYRIGHTMAXFRAME)
			? __PINKYRIGHTMINFRAME : _pinky -> currentAspect () + 1);
		_inky -> setCurrentAspect ((_inky -> currentAspect () == __INKYRIGHTMAXFRAME)
			? __INKYRIGHTMINFRAME : _inky -> currentAspect () + 1);
		_clyde -> setCurrentAspect ((_clyde -> currentAspect () == __CLYDERIGHTMAXFRAME)
			? __CLYDERIGHTMINFRAME : _clyde -> currentAspect () + 1);
	}

	_counterState++;
	// At the second 2 the game is ready to go to the next stage...
	if (_counterState == (2 * _game -> framesPerSecond ()))
		_stateFinishes = true; 
	// At the second 5 the game goes automatically to the next stage...
	if (_counterState == (5 * _game -> framesPerSecond ()))
		_game -> setState (__GAMESTATEINITIALNAME);
}

// ---
void GameStateEnd::drawOn (QGAMES::Screen* s)
{
	_aMap -> drawOn (s, QGAMES::Position::_cero);
	
	_pacman -> drawOn (s);
	_blinky -> drawOn (s);
	_pinky -> drawOn (s);
	_inky -> drawOn (s);
	_clyde -> drawOn (s);
}

// ---
void GameStateEnd::onExit ()
{
	// Reestablish the world...
	((PacmanGame*) _game) -> setWorld (_aWorld -> id ());
	// The map has not to be initialize, as the pacman has still to clean the maze...
}
