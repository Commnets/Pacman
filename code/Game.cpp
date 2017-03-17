#include "Game.hpp"
#include "EntitiesBuilder.hpp"
#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Timer.hpp"
#include "MapsBuilder.hpp"
#include "Maps.hpp"
#include "WorldsBuilder.hpp"
#include "GameStates.hpp"
#include "InputHandler.hpp"
#include "General.hpp"
#include "Defs.hpp"
#include <SDL/sdlformbuilder.hpp>
#include <SDL/sdlsoundbuilder.hpp>
#include <iostream>
#include <fstream>
#include <math.h>

// ---
PacmanGame::PacmanGame ()
		: QGAMES::ArcadeGame (new GameImplementation (), QGAMES::Worlds ()),
		  _points (NULL),
		  _lives (NULL),
		  _mode (_NORMAL),
		  _level (1),
		  _scores (),
		  _pacman (NULL),
		  _blinky (NULL), _pinky (NULL), _inky (NULL), _clyde (NULL),
		  _monsters (),
		  _counterFrigthening (0),
		  _gameStopped (false),
		  _counterStopped (true),
		  _counterToFruit (0),
		  _counterFruitScreen (0),
		  _secondForFruit (0),
		  _fruitShown (false)
{
	// Nothing else to do so far...
}

// ---
void PacmanGame::processEvent (const QGAMES::Event& e)
{
	QGAMES::ArcadeGame::processEvent (e);
	if (e.code () == __BALLEATED)
		ballEated ((QGAMES::Tile*) e.data ());
	if (e.code () == __SUPERBALLEATED)
		superBallEated ((QGAMES::Tile*) e.data ());
	if (e.code () == __MAZECLEAN)
		mazeClean ((QGAMES::Map*) e.data ()); // This event is not received here eventually...it is treated at World level
	if (e.code () == __ENTERPORTAL1)
		moveArtistToPortal (__PORTALGATE2, (PacmanArtist*) e.data ());
	if (e.code () == __ENTERPORTAL2)
		moveArtistToPortal (__PORTALGATE1, (PacmanArtist*) e.data ());
	if (e.code () == __FRUITEATEN)
		fruitEaten ((QGAMES::Tile*) e.data ());
}

// ---
void PacmanGame::addScoreObjects ()
{
	if (!_scoreObjects.empty ())
		return;

	_points = new ScorePoints ();
	_scoreObjects.insert (QGAMES::Entities::value_type (_points -> id (), _points));
	_lives = new ScoreLives ();
	_scoreObjects.insert (QGAMES::Entities::value_type (_lives -> id (), _lives));
}

// ---
void PacmanGame::removeScoreObjects ()
{
	ArcadeGame::removeScoreObjects ();

	delete _points;
	_points = NULL;
	delete _lives;
	_lives = NULL;
}

// ---
void PacmanGame::initScore ()
{
	addScoreObjects (); // If needed...
	_points -> initialize (); // To 0...
	_lives -> initialize (); // To the top of lives...
}

// ---
void PacmanGame::addScore (const std::string& p, int s)
{
	_scores.insert (std::pair <int, std::string> (s, p));
}

// ---
void PacmanGame::setMode (PacmanGame::Mode m)
{ 
	_mode = m; 
	if (_mode == PacmanGame::_FRIGHTENING)
		_counterFrigthening = 0;
}

// ---
void PacmanGame::setLevel (int l) 
{ 
	_level = l % __MAXNUMBEROFLEVELS; // Between 0 and the maximum...

	// The second for the fruit is selected here and any
	// Time the level is changed...
	_secondForFruit = rand () % 120; // The fruit should be shown during the first 120 seconds...
	_counterToFruit = 0;
	_counterFruitScreen = 0;
	_fruitShown = false;

	// The speed of the monsters depends on the level...
	// The speed of the pacman also depends on the level...
	_pacman -> setSpeed ((int) (__BASESPEED * __PERCENTSPEED * 
			General::_e._variables [_level][__VARPACMANSPEED]));
	_blinky -> setSpeed ((int) (__BASESPEED * __PERCENTSPEED * 
			General::_e._variables [_level][__VARGHOSTSPEED]));
	_pinky -> setSpeed ((int) (__BASESPEED * __PERCENTSPEED * 
			General::_e._variables [_level][__VARGHOSTSPEED]));
	_inky -> setSpeed ((int) (__BASESPEED * __PERCENTSPEED * 
			General::_e._variables [_level][__VARGHOSTSPEED]));
	_clyde -> setSpeed ((int) (__BASESPEED * __PERCENTSPEED * 
			General::_e._variables [_level][__VARGHOSTSPEED]));
}

// ---
void PacmanGame::setJoystick (bool j)
{
	assert (_inputHandler);
	((InputHandler*) _inputHandler) -> activateJoystick (j);
}

// ---
QGAMES::Position PacmanGame::monsterPenPosition () const
{
	QGAMES::Position result = QGAMES::Position::_cero;
	if (_activeWorld)
		result = ((WorldPacman*) _activeWorld) -> monsterPenPosition ();
	return (result);
}

// ---
QGAMES::Position PacmanGame::initialPacmanPosition () const
{
	QGAMES::Position result = QGAMES::Position::_cero;
	if (_activeWorld)
		result = ((WorldPacman*) _activeWorld) -> initialPacmanPosition ();
	return (result);
}

// ---
QGAMES::FormBuilder* PacmanGame::createFormBuilder ()
{ 
	return (new SDL2DSpriteBuilder (std::string (__FORMSFILE__), 
		(SDLScreen*) mainScreen ())); 
}

// ---
QGAMES::ObjectBuilder* PacmanGame::createObjectBuilder ()
{ 
	return (new QGAMES::ObjectBuilder (std::string (__OBJECTSFILE__), formBuilder ())); 
}

// ---
QGAMES::EntityBuilder* PacmanGame::createEntityBuilder ()
{ 
	return (new EntitiesBuilder (__ENTITIESFILE__, 
		formBuilder (), movementBuilder ())); 
}

// ---
QGAMES::MovementBuilder* PacmanGame::createMovementBuilder () 
{ 
	return (new MovementsBuilder (__MOVEMENTSFILE__)); 
}

// ---
QGAMES::SoundBuilder* PacmanGame::createSoundBuilder () 
{ 
	return (new SDLSoundBuilder (__SOUNDSFILE__)); 
}

// ---
QGAMES::Timer* PacmanGame::createTimer ()
{ 
	return (new Timer ()); 
}

// ---
QGAMES::Screens PacmanGame::createScreens ()
{ 
	QGAMES::Screens r;
	QGAMES::Screen* scr = new SDLScreen (__GAMESNAME__, 
		 QGAMES::Position (0,0), __SCREENWIDTH__, __SCREENHEIGHT__, __SCREENXPOS__, __SCREENYPOS__);
	r.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, scr));
	return (r); 
}

// ---
QGAMES::WorldBuilder* PacmanGame::createWorldBuilder ()
{
	return (new WorldsBuilder (std::string (__WORLDSFILE__), mapBuilder ()));
}

// ---
QGAMES::MapBuilder* PacmanGame::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (std::string (__MAPSFILE__));
	result -> addAddsOn (new MapsBuilderAddsOn ((QGAMES::Sprite2DBuilder*) (formBuilder ())));
	return (result);
}

// ---
void PacmanGame::initialize ()
{
	QGAMES::ArcadeGame::initialize ();

	// Sets the icon to the window...
	mainScreen () -> setIcon (formBuilder () -> form (__PACMANICOFORM));

	// Clean up everything...
	_artists.clear ();
	_gameStates.clear ();
	_worlds.clear ();

	// The main artist of the game, if they have not been created and inserted before...
	// The command entity creates the entities and observe them...
	_pacman = (Pacman*) entity (__ENTITYPACMAN);
	_artists.insert (QGAMES::Entities::value_type (__ENTITYPACMAN, _pacman));

	// The monsters...
	// Blinky
	_blinky = (Blinky*) entity (__ENTITYBLINKY);
	_blinky -> setMyTarget (_pacman); // Target?
	_monsters.push_back (_blinky);
	_artists.insert (QGAMES::Entities::value_type (__ENTITYBLINKY, _blinky));

	// Pinky
	_pinky = (Pinky*) entity (__ENTITYPINKY);
	_pinky -> setMyTarget (_pacman);
	_monsters.push_back (_pinky);
	_artists.insert (QGAMES::Entities::value_type (__ENTITYPINKY, _pinky));

	// Inky
	_inky = (Inky*) entity (__ENTITYINKY);
	_inky -> setMyTarget (_pacman);
	_monsters.push_back (_inky);
	_artists.insert (QGAMES::Entities::value_type (__ENTITYINKY, _inky));

	// Clyde
	_clyde = (Clyde*) entity (__ENTITYCLYDE);
	_clyde -> setMyTarget (_pacman);
	_monsters.push_back (_clyde);
	_artists.insert (QGAMES::Entities::value_type (__ENTITYCLYDE, _clyde));

	// Assign the collegues...
	_blinky -> setCollegues (_monsters);
	_pinky -> setCollegues (_monsters);
	_inky -> setCollegues (_monsters);
	_clyde -> setCollegues (_monsters);

	// Load the states...
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATELOADINGNAME), new PacmanGameStateLoading (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEINITIALNAME), new GameStateInitial (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEPRELUDENAME), new GameStatePrelude (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEPRELUDESHORTNAME), new GameStatePreludeShort (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEPLAYINGNAME), new GameStatePlaying (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEMAZECLEANNAME), new GameStateMazeClean (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEPACMANDIESNAME), new GameStatePacmanDies (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEROUNDENDNAME), new GameStateRoundEnd (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEENDNAME), new GameStateEnd (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATEINTROLETTERSNAME), new GameStateIntroLetters (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__GAMESTATESEESCORESNAME), new GameStateSeeScore (this)));

	// When the game starts, the list of the most famous player is loaded
	// The list is something really simple to read
	_scores.clear ();
	std::ifstream scoreFile ("scores.txt");
	if (scoreFile)
	{
		std::string usr, score;
		while (!scoreFile.eof ())
		{
			scoreFile >> score >> usr;
			_scores.insert (std::pair <int, std::string> (atoi (score.c_str ()), usr));
		}

		scoreFile.close ();
	}

	// The initial state...
	setState (std::string (__GAMESTATELOADINGNAME));

	// Load the world...
	_worlds.insert (QGAMES::Worlds::value_type (__WORLDPACMAN,
		_worldBuilder -> world (__WORLDPACMAN)));

	// Sets the first level...
	setLevel (0);
}

// ---
void PacmanGame::inEveryLoop ()
{
	ArcadeGame::inEveryLoop ();

	// Only when debugging...
	// ...and just in case
	#ifndef NDEBUG 
	// TO DO
	#endif
}

// ---
void PacmanGame::updatePositions ()
{
	// If the game has been stopped for a while,
	// Not actualization is done...
	// Normally the game is stopped when a monster has been eaten!
	// It has to be done before any other thing happens...
	if (_gameStopped)
	{
		if (++_counterStopped >= (1 * framesPerSecond ()))
		{
			_gameStopped = false;
			_counterStopped = 0;
		}

		return;
	}

	// Now first do what the parents says to do...
	QGAMES::ArcadeGame::updatePositions ();

	QGAMES::Screen* scr = mainScreen ();
	if (scr -> fix () != NULL)
		scr -> setFix (NULL); 
	// When the status is not playing, there is nothing to follow...
	// Any case, this is just in case...

	// When the game is not going on...
	// Nothing of the rest makes sense...
	if (_currentGameState -> type () != __GAMESTATEPLAYING)
		return;

	// Otherwhise....
	// Has the fruit to be shown?
	++_counterToFruit;
	if (_counterToFruit == (_secondForFruit * framesPerSecond ()))
	{
		// The counter is not set to 0 again because only one fruit is allowed per round...
		// The fruit shown depends on the level...
		((WorldPacman*) _activeWorld) -> 
			showFruit ((int) General::_e._variables [_level][__VARBONUSSYMBOL]);
		_fruitShown = true;
	}

	// If the fruit is on the screen
	// The counter is still counting to remove it...
	// The fruit is 5 seconds on the screen always (independent on the level)
	if (_fruitShown)
	{
		if (_counterToFruit == ((_secondForFruit + 5) * framesPerSecond ()))
		{
			// The counter is not set to 0 again because only one fruit is allowed per round...
			((WorldPacman*) _activeWorld) -> showFruit (__FRUITFRAMEEMPTY);
			_fruitShown = false;
			_counterToFruit = 0;
		}
	}

	// Look after the frightening status...
	// The number of seconds the frightening ststus lasts depends on the level...
	if (_mode == PacmanGame::_FRIGHTENING)
	{
		_counterFrigthening++;
		if (_counterFrigthening >= 
				((int) (General::_e._variables [_level][__VARFRIGHTTIME] * framesPerSecond ())))
		{
			setMode (PacmanGame::_SEMIFRIGHTENING);
			_counterFrigthening = 0;
		}
	}

	// Look after the semifrightening status...
	// The game is turning back to the normal situation...
	// The number of seconds in the new flashing situation also depends on the level...
	if (_mode == PacmanGame::_SEMIFRIGHTENING)
	{
		_counterFrigthening++;
		if (_counterFrigthening >= 
				((int) (General::_e._variables [_level][__VARNUMBERFLASHES] * framesPerSecond ())))
		{
			setMode (PacmanGame::_NORMAL);
			_counterFrigthening = 0;
		}
	}
}

// ---
void PacmanGame::detectCollisions ()
{
	if (_activeWorld == NULL)
		return;

	bool eatenFound = false;
	int lx = (__TM (_activeWorld -> activeScene () -> activeMap ())) -> tileWidth () / 2;
	int ly = (__TM (_activeWorld -> activeScene () -> activeMap ())) -> tileHeight () / 2;
	QGAMES::Position pcPos = _pacman -> position ();
	for (PacmanMonsters::const_iterator i = _monsters.begin ();
		i != _monsters.end () && !eatenFound; i++)
	{
		QGAMES::Position mPos = (*i) -> position ();
		int dx = (int) (pcPos.posX () - mPos.posX ()); if (dx < 0) dx = -dx;
		int dy = (int) (pcPos.posY () - mPos.posY ()); if (dy < 0) dy = -dy;
		// The collision happens when the pacman or the monster is in the middle
		// of the tile occupied by the other...
		if (dx < lx && dy < ly)
		{
			if (_mode == PacmanGame::_NORMAL && 
				(*i) -> state () != PacmanMonster::_DIE) // ...and the monster is still alive...
			{
				eatenByAMonster ((*i));
				eatenFound = true; // When the first is found...nothing else matters...
			}
			else
				if ((*i) -> state () != PacmanMonster::_DIE)
					monsterEaten ((*i)); // Only when the monsters is not already die...
		}
	}
}

// ---
void PacmanGame::finalize ()
{
	ArcadeGame::finalize ();

	// When the game finishes, the list is saved
	std::ofstream scoreFile ("scores.txt");
	if (scoreFile)
	{
		bool f = true;
		for (ScoreList::const_iterator i = _scores.begin ();
				i != _scores.end (); i++)
		{
			if (!f) scoreFile << std::endl;
			scoreFile << (*i).first << std::string ("\t") << (*i).second;
			f = false;
		}

		scoreFile.close ();
	}
}

// ---
void PacmanGame::ballEated (QGAMES::Tile* t)
{
	t -> setForm (t -> form (), __BALLEATEDFRAME); // It is turned off to background...
	_points -> add (__POINTSBALLEATED);
	sound (__SOUNDCHOMP) -> play (-1);
}

// ---
void PacmanGame::superBallEated (QGAMES::Tile* t)
{
	t -> setForm (t -> form (), __SUPERBALLEATEDFRAME); // It is turned off to background...
	_points -> add (__POINTSSUPERBALLEATED);
	sound (__SOUNDCHOMP) -> play (-1);
	setMode (_FRIGHTENING);
}

// ---
void PacmanGame::mazeClean (QGAMES::Map* m)
{
	// When the maze has been clean, the status of the game changes...
	setState (__GAMESTATEMAZECLEANNAME);
}

// ---
void PacmanGame::monsterEaten (PacmanMonster* m)
{
	// It could happend that the collision is 
	// with a die monster.
	// If it is the case, then the collision is no valid...
	if (m -> state () == PacmanArtist::_DIE)
		return;

	// It has to stop the game for a while...
	_gameStopped = true;

	// Otherwhise the coliision, means the pacman has died...
	sound (__SOUNDEATGHOST) -> play (-1);
	_points -> add (m -> pointsWhenEaten ());
	m -> setState (PacmanMonster::_DIE);
}

// ---
void PacmanGame::eatenByAMonster (PacmanMonster* m)
{
	// The state changes...to die
	setState (__GAMESTATEPACMANDIESNAME);
}

// ---
void PacmanGame::moveArtistToPortal (int p, PacmanArtist* a)
{
	// The map and the layer are needed...
	MapMaze* map = (MapMaze*) _activeWorld -> activeScene () -> activeMap ();
	TileLayerMaze* layer = (TileLayerMaze*) map -> layer (std::string (__MAZELAYER));

	// When the collision happens, the artist was in aprt in the new tile...
	// So, this distance in the new tile must be conserved
	// In other to avoid bad positioning of the monster in the maze...
	int tW = map -> tileWidth (); 
	int dX = (int) a -> position ().posX () % tW; 

	// Finally looks for the position...
	QGAMES::Position pos = layer -> positionFirstTile (__TILEGATE, p);
	if (pos != QGAMES::Position::_noPoint)
		a -> setPosition (pos + QGAMES::Vector (__BD dX, __BD 0, __BD 0)); 
		// The orientation doesn't change...
}

// ---
void PacmanGame::fruitEaten (QGAMES::Tile* t)
{
	sound (__SOUNDEATFRUIT) -> play (-1);
	// The points added depends on the level...
	_points -> add ((int) General::_e._variables [_level][__VARBONUSPOINTS]); 
	((WorldPacman*) _activeWorld) -> showFruit (__FRUITFRAMEEMPTY);
	_fruitShown = false;
}
