#include "GameStates.hpp"
#include "Game.hpp"
#include "Entities.hpp"
#include "General.hpp"
#include "InputHandler.hpp"

// ---
PacmanPresentationText::PacmanPresentationText (const std::string& t)
		: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (__LETTERSFORM, 
		QGAMES::Game::game () -> form (__LETTERSFORM)));
	setText (t);
}

// ---
void PacmanGameStateLoading::onEnter ()
{
	// The counter to control how long the 
	// screen is a loading mode...
	_counter = 0;
	// The background used during this state
	_background = _game -> form (__BACKGROUNDINITIALFORM);
	// The logo used during this state...
	_logo = _game -> form (__COMMTYLOGOFORM);
	// The text used...
	_text = new PacmanPresentationText (std::string ("2016 IGNACIO CEA"));
	_text -> setSpace (-5); // Letters are tight!!
	// Not fadding 
	_fadeOut = false;
	// The counter for alpha channel to the begging...
	_counterAlpha = 255;

	// There is no active world...so far
	(__AGM _game) -> setWorld (__MININT__);
	// There is no score objects...
	(__AGM _game) -> removeScoreObjects ();
}

// ---
void PacmanGameStateLoading::updatePositions ()
{
	// Two seconds in this state...
	// Before fading the title out...
	if (_counter++ > (2 * _game -> framesPerSecond ()))
		_fadeOut = true;

	// If fadding out...
	if (_fadeOut)
	{
		// No more than 10 seconds...
		if (_counter++ > (10 * _game -> framesPerSecond ())) 
			_game -> setState (std::string (__GAMESTATEINITIALNAME));

		// ...or until dissapear...
		_counterAlpha = _counterAlpha - 2; 
		if (_counterAlpha < 0)
			_game -> setState (std::string (__GAMESTATEINITIALNAME));
	}
}

// ---
void PacmanGameStateLoading::drawOn (QGAMES::Screen* s)
{
	// Display the background...a very scared one!
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100);
	// During this state the logo of Ceasoftware is displayed at the bootm
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos, _counterAlpha);
	// Write down the text, next to the logo...
	_text -> setAlphaLevel (_counterAlpha);
	_text -> drawOn (s, lPos + QGAMES::Position (__BD (_logo -> frameWidth () + 10), 
		__BD (((_logo -> frameHeight () - _text -> visualHeight ()) / 2) + 10), __BD 0));
}

// ---
void PacmanGameStateLoading::onExit ()
{
	delete (_text);
}

// ---
GameStateInitial::GameStateInitial (QGAMES::Game* g)
	: QGAMES::GameState (__GAMESTATEINITIAL, g),
	  _blinky (NULL),
	  _background (NULL),
	  _logo (NULL),
	  _options (),
	  _optionLevels (),
	  _optionGraphics (),
	  _currentOption (0),
	  _currentOptionLevel (0),
	  _counterMovement (0),
	  _positionOption (0),
	  _movingToOption (0),
	  _figure (0),
	  _typeOfControl (0),
	  _blinkAttr (50),
	  _blinkDirection (2),
	  _wantToExit (false)
{
	// Nothing else to do so far...
}

// ---
void GameStateInitial::nextOption ()
{
	if (_movingToOption != 0)
		return;
	if (_currentOption < (__NUMBEROPTIONS - 1))
		_movingToOption = 1;
}

// ---
void GameStateInitial::previousOption ()
{
	if (_movingToOption != 0)
		return;
	if (_currentOption > 0)
		_movingToOption = -1;
}

// ---
void GameStateInitial::optionSelected ()
{
	if (_movingToOption != 0)
		return;

	if (_currentOption == 0)
		_typeOfControl = 0;
	if (_currentOption == 1)
		_typeOfControl = 1;

	if (_currentOption == 3)
	{
		if (++_currentOptionLevel >= __MAXNUMBEROFDIFFICULTYLEVELS)
			_currentOptionLevel = 0;
		_options [3] = _optionLevels [_currentOptionLevel];
		delete (_optionGraphics [3]);
		_optionGraphics [3] = new PacmanPresentationText (_options [3]);
		_optionGraphics [3] -> setSpace (0);
	}

	if (_currentOption == 4 || _currentOption == 2)
		_wantToExit = true;
}

// ---
void GameStateInitial::optionAt (const QGAMES::Position& pos)
{
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 500), __BD 40, __BD 0);
	for (int i = 0; i < __NUMBEROPTIONS; i++)
	{
		QGAMES::Position p1C = oPos + 
			(__BD (i * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0));
		QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
		QGAMES::Rectangle rOpt (p1C, p2C);
		if (rOpt.hasIn (pos))
			_currentOption = i;
	}
}
// ---
void GameStateInitial::onEnter ()
{
	// The background used during this state
	_background = _game -> form (__BACKGROUNDINITIALFORM);
	// ...and the logo used during this state...
	_logo = _game -> form (__COMMTYLOGOFORM);

	// Blinky is the selector
	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> setCurrentForm (__SPRITESFORM);
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME);
	_blinky -> setPosition (QGAMES::Position (__BD 50, 
		__BD (150 + (_currentOption * 75)), __BD 0));

	// Define the options related with the level...
	_optionLevels.resize (__MAXNUMBEROFDIFFICULTYLEVELS);
	_optionLevels [0] = std::string ("EASY");
	_optionLevels [1] = std::string ("NORMAL");
	_optionLevels [2] = std::string ("DIFFICULT");

	// Define the options...
	_options.resize (__NUMBEROPTIONS);
	_options [0] = std::string ("KEYBOARD");
	_options [1] = std::string ("JOYSTICK");
	_options [2] = std::string ("SCORES");
	_options [3] = _optionLevels [_currentOptionLevel];
	_options [4] = std::string ("START GAME");

	// ...and the equivalent graphics...
	_optionGraphics.resize (__NUMBEROPTIONS);
	for (int i = 0; i < __NUMBEROPTIONS; i++)
	{
		_optionGraphics [i] = new PacmanPresentationText (_options [i]);
		_optionGraphics [i] -> setSpace (0);
	}

	// It doesn't want to exit...so far
	_wantToExit = false;
	// The counter movement is set to 0
	_counterMovement = 0;
	// No movement to none option...
	_movingToOption = 0;
	// No counter in place to execute the movement
	_positionOption = 0;
	// Type of control selected: Keyboard or Joystick?
	// First time it will be keyboard, but after first game who knows?
	_typeOfControl = ((InputHandler*) _game -> inputHandler () -> behaviour ()) -> isJoystickActive () ? 1 : 0; 
	// Blinking attribute
	_blinkAttr = 50;
	// The dircetion is up...
	_blinkDirection = 2;

	// The score objects are not valid in this stage...
	_game -> removeScoreObjects ();
	// No world so far...
	((PacmanGame*) _game) -> setWorld (__MININT__);
	// Fix the screen atthe beginning...
	_game -> mainScreen () -> setPosition (QGAMES::Vector::_cero);
}

// ---
void GameStateInitial::updatePositions ()
{
	// The things move...
	if (++_counterMovement > 6)
	{
		_counterMovement = 0;
		if (++_figure > 1) // Blinky has only one aspect...
			_figure = 0;
	}

	// Moves the rectangle around the option...
	if (_movingToOption != 0)
	{
		_positionOption += 4;
		if (_positionOption > 60)
		{
			_positionOption = 0;
			if (_movingToOption == 1)
				_currentOption++;
			else
				_currentOption--;
			_movingToOption = 0;
		}
	}

	// Blinks...
	_blinkAttr += _blinkDirection;
	if (_blinkAttr > 150) _blinkAttr = 150;
	if (_blinkAttr < 50) _blinkAttr = 50;
	if (_blinkAttr == 150 || _blinkAttr == 50)
		_blinkDirection *= -1;

	// if the right key has been selected,
	// It is time to move to the next state...
	if (_wantToExit)
		_game -> setState ((_currentOption == 2) 
			? __GAMESTATESEESCORESNAME : __GAMESTATEPRELUDENAME);
}

// ---
void GameStateInitial::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos);

	// The original position of the options..
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 500), __BD 40, __BD 0);

	// Draws a full rectangle around the current options selected
	// The color of the rectangle is gray...
	QGAMES::Position p1A = oPos + 
		(__BD ((_typeOfControl == 0) ? 0 : 1) * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	QGAMES::Position p2A = p1A + QGAMES::Vector (__BD 450, __BD 60, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (_blinkAttr, _blinkAttr, _blinkAttr), true);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (255, 255, 255));

	// Draws the options...
	for (int i = 0; i < __NUMBEROPTIONS; i++)
	{
		QGAMES::Vector aPos (__BD 0, __BD (i * 60), __BD 0);
		_optionGraphics [i] -> 
			drawOn (s, oPos + aPos + QGAMES::Vector (__BD 60, __BD 10, __BD 0));
	}
	
	// Draws a while rectangle around the selected option...
	QGAMES::Position p1C = oPos + 
		(__BD (_currentOption * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0)) + 
			(__BD _movingToOption * QGAMES::Vector (__BD 0, __BD _positionOption, __BD 0));
	QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 450, __BD 60, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1C, p2C), QGAMES::Color (255,255,255), false);

	// Draws blinky...
	_blinky -> setPosition (QGAMES::Position (__BD 50, 
		__BD (50 + (_currentOption * 60)), __BD 0));
	_blinky -> setCurrentAspect (__BLINKYRIGHTMINFRAME + _figure);
	_blinky -> drawOn (s);
}

// ---
void GameStateInitial::onExit ()
{
	// Deletes all the reserved things...
	for (int i = 0; i < __NUMBEROPTIONS; i++)
		delete (_optionGraphics [i]);
	_options.clear ();
	_optionGraphics.clear ();

	// It is time to select the level...
	((PacmanGame*) _game) -> setLevel (_currentOptionLevel * 10);
	// Select the type of control: joystick or keyboard?
	((PacmanGame*) _game) -> setJoystick ((_typeOfControl == 0) ? false : true);

	// Stablishes the world and the things around it...
	((PacmanGame*) _game) -> setWorld (__WORLDPACMAN);
	((PacmanGame*) _game) -> activeWorld () -> initialize ();
}

// ---
void GameStatePrelude::onEnter ()
{
	_counter = 0;
	_counterState = 0;
	_direction = 1;
	_stateFinishes = false;
	_wantToExit = false;

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
	if (_counterState == (5 * _game -> framesPerSecond ()) || _wantToExit)
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
	((PacmanGame*) _game) -> setLives (__MAXNUMBEROFLIVES__);
	// Stop the music just in case...
	// If the user leaves the game enough time the music finishes by itself...
	_game -> sound (__SOUNDBEGINNING) -> stop ();
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
		_game -> setState (((l % __PLAYSTOROUND__) == 0 && l != 0) 
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
GameStateRoundEnd::GameStateRoundEnd (QGAMES::Game* g)
	: QGAMES::GameState (__GAMESTATEROUNDEND, g),
	  _pacman (NULL), 
	  _counterState (0),
	  _aWorld (NULL),
	  _counterForm (0),
	  _counterToChangeForm (0),
	  _runningAway (true),
	  _position (QGAMES::Position::_cero),
	  _iFPacman (0), _iFBlinky (0), _iFPinky (0), _iFInky (0), _iFClyde (0),
	  _mov (QGAMES::Vector::_cero), _inc (QGAMES::Vector::_cero),
	  _finishes (false)
{
	// Nothing else to do so far...
}

// ---
void GameStateRoundEnd::onEnter ()
{
	_counterState = 0;
	_counterForm = 0;
	_counterToChangeForm = 0;
	_position = 
		QGAMES::Position (__BD (__SCREENWIDTH__ + 50), __BD (__SCREENHEIGHT__ >> 1), __BD 0);
	_runningAway = true;
	_mov = QGAMES::Vector (__BD 4, __BD 0, __BD 0);
	_inc = QGAMES::Vector (__BD 40, __BD 0, __BD 0);
	_finishes = false;

	// Keeps the current world, just to avoid
	// The main loop to draw and manage the presentation...
	_aWorld = ((PacmanGame*) _game) -> activeWorld (); // It will be reestablished later on...
	((PacmanGame*) _game) -> setWorld (__MININT__);

	_pacman = (Pacman*)	((PacmanGame*) _game) -> entity (__ENTITYPACMAN);
	_pacman -> setCurrentForm (__SPRITESFORM);
	_iFPacman = __PACMANLEFTMINFRAME; _pacman -> setCurrentAspect (_iFPacman);
	_pacman -> setCurrentMovement (__MININT__);
	_pacman -> setPosition (_position);

	_blinky = (Blinky*) ((PacmanGame*) _game) -> entity (__ENTITYBLINKY);
	_blinky -> setCurrentForm (__SPRITESFORM);
	_iFBlinky = __BLINKYLEFTMINFRAME; _blinky -> setCurrentAspect (_iFBlinky);
	_blinky -> setCurrentMovement (__MININT__);
	_blinky -> setPosition (_position + (3 * _inc));

	_pinky = (Pinky*) ((PacmanGame*) _game) -> entity (__ENTITYPINKY);
	_pinky -> setCurrentForm (__SPRITESFORM);
	_iFPinky = __PINKYLEFTMINFRAME; _pinky -> setCurrentAspect (_iFPinky);
	_pinky -> setPosition (_position + (4 * _inc));

	_inky = (Inky*) ((PacmanGame*) _game) -> entity (__ENTITYINKY);
	_inky -> setCurrentForm (__SPRITESFORM);
	_iFInky = __INKYLEFTMINFRAME; _inky -> setCurrentAspect (_iFInky);
	_inky -> setCurrentMovement (__MININT__);
	_inky -> setPosition (_position + (5 * _inc));

	_clyde = (Clyde*) ((PacmanGame*) _game) -> entity (__ENTITYCLYDE);
	_clyde -> setCurrentForm (__SPRITESFORM);
	_iFClyde = __CLYDELEFTMINFRAME; _clyde -> setCurrentAspect (_iFClyde);
	_clyde -> setCurrentMovement (__MININT__);
	_clyde -> setPosition (_position + (6 * _inc));

	// The main sound...intermission sound......
	_game -> sound (__SOUNDINTERMISSION) -> play (-1);
	// Fix the screen atthe beginning...
	_game -> mainScreen () -> setPosition (QGAMES::Vector::_cero);
}

// ---
void GameStateRoundEnd::updatePositions ()
{
	// The counter to change the form...
	if (_counterToChangeForm++ >= 6)
	{
		_counterToChangeForm = 0;
		if (_counterForm++ >= 1)
			_counterForm = 0;
	}

	// Change the movement...
	// ...in the left side?
	if (_clyde -> position ().posX () <= -60)
	{
		_runningAway = false;
		_iFPacman = __GIANTPACMANRIGHTMINFRAME;
		_pacman -> setCurrentForm (__GIANTPACMANFORM__);
		_iFBlinky = __BLINKYRIGHTMINFRAME;
		_iFPinky = __PINKYRIGHTMINFRAME;
		_iFInky = __INKYRIGHTMINFRAME;
		_iFClyde = __CLYDERIGHTMINFRAME;
	}
	// ..in the right side?
	if (_pacman -> position ().posX () >= (__SCREENWIDTH__ + 60))
	{
		_runningAway = true;
		_iFPacman = __PACMANLEFTMINFRAME;
		_pacman -> setCurrentForm (__SPRITESFORM);
		_iFBlinky = __BLINKYLEFTMINFRAME;
		_iFPinky = __PINKYLEFTMINFRAME;
		_iFInky = __INKYLEFTMINFRAME;
		_iFClyde = __CLYDELEFTMINFRAME;
		_finishes = true;
	}

	// Moves the elements...
	if (_runningAway) _position -= _mov;
	else _position += _mov;

	// Assign the form...
	_pacman -> setCurrentAspect (_iFPacman + _counterForm);
	_blinky -> setCurrentAspect (_iFBlinky + _counterForm);
	_pinky -> setCurrentAspect (_iFPinky + _counterForm);
	_inky -> setCurrentAspect (_iFInky + _counterForm);
	_clyde -> setCurrentAspect (_iFClyde + _counterForm);

	// And set again the position...
	_pacman -> setPosition ((_runningAway) 
		? _position : (_position - QGAMES::Vector (__BD 0, __BD 48, __BD 0)));
	_blinky -> setPosition (_position + _mov + (3 * _inc));
	_pinky -> setPosition (_position + _mov + (4 * _inc));
	_inky -> setPosition (_position + _mov + (5 * _inc));
	_clyde -> setPosition (_position + _mov + (6 * _inc));

	_counterState++;
	// At the second five, the system moves to the next level...
	if (_counterState == (20 * _game -> framesPerSecond ()) || _finishes)
		_game -> setState (__GAMESTATEPRELUDESHORTNAME);
}

// ---
void GameStateRoundEnd::drawOn (QGAMES::Screen* s)
{
	_pacman -> drawOn (s);
	_blinky -> drawOn (s);
	_pinky -> drawOn (s);
	_inky -> drawOn (s);
	_clyde -> drawOn (s);
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
		_game -> setState (__GAMESTATEINTROLETTERSNAME);
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

// ---
GameStateIntroLetters::GameStateIntroLetters (QGAMES::Game* g)
	: QGAMES::GameState (__GAMESTATEINTROLETTERS, g),
	  _background (NULL),
	  _logo (NULL),
	  _nLetter (0),
	  _cLetter (32), // First letter is always an space...
	  _blinkAttr (0),
	  _blinkDirection (1),
	  _nScore (0),
	  _nMScores (0),
	  _sToAdd (__NULL_STRING__),
	  _wantToExit (false),
	  _initials (),
	  _tScores (),
	  _posNewInitials (0),
	  _ceros ("000000")
{
	_letters [0] = _letters [1] = _letters [2] = 42; // Asterisc...
	_letters [3] = 0;
}

// ---
void GameStateIntroLetters::nextLetter ()
{
	if (++_cLetter == 94) 
		_cLetter = 33;
	if (_cLetter == 40 || _cLetter == 41) _cLetter = 42;
	_letters [_nLetter] = _cLetter;
}

void GameStateIntroLetters::previousLetter ()
{
	if (--_cLetter == 32) 
		_cLetter = 93;
	if (_cLetter == 40 || _cLetter == 41) _cLetter = 39;
	_letters [_nLetter] = _cLetter;
}

void GameStateIntroLetters::letterSelected ()
{
	if (++_nLetter == 3)
		_wantToExit = true;
}

void GameStateIntroLetters::onEnter ()
{
	// The background used during this state
	_background = _game -> form (__BACKGROUNDINITIALFORM);
	// The logo used during this state...
	_logo = _game -> form (__COMMTYLOGOFORM);
	// The number of the letter to me modified first is the 0
	_nLetter = 0;
	// The first character valid is an space...
	_cLetter = 32;
	// Blinking attribute
	_blinkAttr = 50;
	// The dircetion is up...
	_blinkDirection = 2;
	// The letters are spaces...
	_letters [0] = _letters [1] = _letters [2] = 42; _letters [3] = 0;
	// Not exit so far...
	_wantToExit = false;
	// The score to add...
	_nScore = ((PacmanGame*) _game) -> score ();
	char nST[255]; _itoa_s (_nScore, nST, 10);
	_sToAdd = std::string (nST);
	if (_sToAdd.length () < 6)
		_sToAdd = _ceros.substr (0, 6 - _sToAdd.length ()) + _sToAdd;

	// The world finishes...
	((PacmanGame*) _game) -> setWorld (__MININT__);
	// Fix the screen atthe beginning...
	_game -> mainScreen () -> setPosition (QGAMES::Vector::_cero);
	_game -> removeScoreObjects ();

	// Create the initials...
	_initials.resize (__MAXSCORES); _tScores.resize (__MAXSCORES);
	_posNewInitials = 0; // Means that by default new initials go at the beginning of the list!
	int nI = 0; 
	PacmanGame::ScoreList lst = ((PacmanGame*) _game) -> scores ();
	for (PacmanGame::ScoreList::const_reverse_iterator i = lst.rbegin (); 
			i != lst.rend () && nI < __MAXSCORES; i++, nI++)
	{
		if (_nScore < (*i).first) _posNewInitials = nI + 1;

		_initials [nI] = new PacmanPresentationText ((*i).second);
		_initials [nI] -> setSpace (5);

		char v[255]; _itoa_s ((*i).first, v, 10);
		std::string vS (v); 
		if (vS.length () < 6) 
			vS = _ceros.substr (0, 6 - vS.length ()) + vS;
		_tScores [nI] = new PacmanPresentationText (vS);
		_tScores [nI] -> setSpace (5);
	}

	// The rest of the initial up to __MAXSCORES are empty...
	_nMScores = nI;
	for (int i = nI; i < __MAXSCORES; i++)
	{
		_initials [i] = new PacmanPresentationText (std::string ("AAA"));
		_tScores [i] = new PacmanPresentationText (_ceros);
		_initials [i] -> setSpace (5);
		_tScores [i] -> setSpace (5);
	}
}

void GameStateIntroLetters::updatePositions ()
{
	// Blinks...
	_blinkAttr += _blinkDirection;
	if (_blinkAttr > 150) _blinkAttr = 150;
	if (_blinkAttr < 50) _blinkAttr = 50;
	if (_blinkAttr == 150 || _blinkAttr == 50)
		_blinkDirection *= -1;

	// if the right key has been selected,
	// It is time to move to the next state...
	if (_wantToExit)
		_game -> setState (__GAMESTATEINITIALNAME);
}

void GameStateIntroLetters::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos);

	// The original positions...
	QGAMES::Position iPos (__BD 100, __BD 50, __BD 0);
	QGAMES::Position oPos = iPos;
	QGAMES::Vector incX (__BD 40, __BD 0, __BD 0);
	QGAMES::Vector incY (__BD 0, __BD 50, __BD 0);
	QGAMES::Vector aTxt (__BD 275, __BD 0, __BD 0);

	// Draws a full rectangle around the current options selected
	// The color of the rectangle is gray...
	QGAMES::Position p1A = oPos + (__BD (_posNewInitials + 1) * incY) + (__BD _nLetter * incX) + aTxt;
	QGAMES::Position p2A = p1A + QGAMES::Vector (__BD 40, __BD 5, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (_blinkAttr, _blinkAttr, _blinkAttr), true);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (255, 255, 255));

	// It is time to draw...
	if (_nMScores == 0)
	{
		PacmanPresentationText* nS = new PacmanPresentationText (_sToAdd);
		nS -> setSpace (5); 
		nS -> drawOn (s, oPos);
		delete (nS);

		for (int j = 0; j < 3; j++)
		{
			PacmanPresentationText* nT = new PacmanPresentationText (std::string (1, _letters [j]));
			nT -> setSpace (5); 
			nT -> drawOn (s, oPos + aTxt + (__BD j * incX));
			delete (nT);
		}
	}
	else
	{
		int i = 0;
		for (; i < __MAXSCORES && i < _nMScores; i++)
		{
			if (i == _posNewInitials)
			{
				PacmanPresentationText* nS = new PacmanPresentationText (_sToAdd);
				nS -> setSpace (5); 
				nS -> drawOn (s, oPos);
				delete (nS);

				for (int j = 0; j < 3; j++)
				{
					PacmanPresentationText* nT = new PacmanPresentationText (std::string (1, _letters [j]));
					nT -> setSpace (5); 
					nT -> drawOn (s, oPos + aTxt + (__BD j * incX));
					delete (nT);
				}

				oPos += incY;
			}

			_tScores [i] -> drawOn (s, oPos);
			_initials [i] -> drawOn (s, oPos + aTxt);
			oPos += incY;
		}

		if (i == _posNewInitials)
		{
			PacmanPresentationText* nS = new PacmanPresentationText (_sToAdd);
			nS -> setSpace (5); 
			nS -> drawOn (s, oPos);
			delete (nS);

			for (int j = 0; j < 3; j++)
			{
				PacmanPresentationText* nT = new PacmanPresentationText (std::string (1, _letters [j]));
				nT -> setSpace (5); 
				nT -> drawOn (s, oPos + aTxt + (__BD j * incX));
				delete (nT);
			}
		}
	}
}

void GameStateIntroLetters::onExit ()
{
	// Deletes all the texts...
	for (int i = 0; i < __MAXSCORES; i++)
	{
		delete (_initials [i]);
		delete (_tScores [i]);
	}

	((PacmanGame*) _game) -> addScore (_letters, _nScore);
	_initials.clear ();
	_tScores.clear ();
}


// ---
GameStateSeeScore::GameStateSeeScore (QGAMES::Game* g)
	: QGAMES::GameState (__GAMESTATESEESCORES, g),
	  _background (NULL),
	  _logo (NULL),
	  _wantToExit (false),
	  _initials (),
	  _tScores (),
	  _ceros ("000000")
{
	// Nothing else to do...
}

void GameStateSeeScore::onEnter ()
{
	// The background used during this state
	_background = _game -> form (__BACKGROUNDINITIALFORM);
	// The logo used during this state...
	_logo = _game -> form (__COMMTYLOGOFORM);
	// Not exit so far...
	_wantToExit = false;

	// The world finishes...
	((PacmanGame*) _game) -> setWorld (__MININT__);
	// Fix the screen atthe beginning...
	_game -> mainScreen () -> setPosition (QGAMES::Vector::_cero);
	_game -> removeScoreObjects ();

	// Create the initials...
	int nI = 0;
	_initials.resize (__MAXSCORES); _tScores.resize (__MAXSCORES);
	PacmanGame::ScoreList lst = ((PacmanGame*) _game) -> scores ();
	for (PacmanGame::ScoreList::const_reverse_iterator i = lst.rbegin (); 
			i != lst.rend (); i++, nI++)
	{
		_initials [nI] = new PacmanPresentationText ((*i).second);
		_initials [nI] -> setSpace (5);

		char v[255]; _itoa_s ((*i).first, v, 10);
		std::string vS (v); 
		if (vS.length () < 6) 
			vS = _ceros.substr (0, 6 - vS.length ()) + vS;
		_tScores [nI] = new PacmanPresentationText (vS);
		_tScores [nI] -> setSpace (5);
	}

	for (int i = nI; i < __MAXSCORES; i++)
		_initials [i] = _tScores [i] = NULL; // Nothing...
}

// ---
void GameStateSeeScore::updatePositions ()
{
	if (_wantToExit)
		_game -> setState (__GAMESTATEINITIALNAME);
}

void GameStateSeeScore::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos);

	// The original positions...
	QGAMES::Position iPos (__BD 100, __BD 50, __BD 0);
	QGAMES::Position oPos = iPos;
	QGAMES::Vector incX (__BD 40, __BD 0, __BD 0);
	QGAMES::Vector incY (__BD 0, __BD 50, __BD 0);
	QGAMES::Vector aTxt (__BD 275, __BD 0, __BD 0);

	for (int i = 0; i < __MAXSCORES; i++)
	{
		if (_tScores [i] != NULL)
		{
			_tScores [i] -> drawOn (s, oPos);
			_initials [i] -> drawOn (s, oPos + aTxt);
			oPos += incY;
		}
	}
}

void GameStateSeeScore::onExit ()
{
	// Deletes all the texts...
	for (int i = 0; i < __MAXSCORES; i++)
	{
		delete (_initials [i]);
		delete (_tScores [i]);
	}

	_initials.clear ();
	_tScores.clear ();
}
