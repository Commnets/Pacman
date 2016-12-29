#include "Entities.hpp"
#include "Movements.hpp"
#include "Maps.hpp"
#include "General.hpp"
#include "Game.hpp"
#include <iostream>

// ---
PacmanArtist::PacmanArtist (int id, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
		: QGAMES::ArtistInATiledMap (id, f, d),
		  _counter (0),
		  _changeMovement (false),
		  _movementToChange (__MININT__),
//		  _lastPosition (QGAMES::Position::_cero),
		  _initialize (false)
{ 
	_aspects.resize (7); 
	// Normal aspects are up, down, right, left,...
	// But other 3 aspects can be set up
	// In the case of the pacman those aspects will be: pacman dieing, an no more...
	// In the case of the monsters those aspecs will be: 
	// monster in panic, monster blinking, and monster died.
	for (int i = 0; i < 7; i++)
	{
		_aspects [i].resize (2);
		_aspects [i][0] = _aspects [i][1] = 0;
	}
}

// ---
void PacmanArtist::setState (PacmanArtist::State s)
{ 
	_state = s; 
}

// ---
void PacmanArtist::setSpeed (int s)
{
	// Set the speed for all the movements related with the artist...
	for (QGAMES::Movements::const_iterator i = _data._movements.begin ();
			i != _data._movements.end (); i++)
		((PacmanEntityMovement*) ((*i).second)) -> setSpeed (s);
}

// ---
void PacmanArtist::updatePositions ()
{
	// Sometimw it could be necessary...
	if (_initialize)
	{
		initialize ();
		_initialize = false;
	}

	// Before doing anything, lets be sure there is no any change in the movement
	// pending to be executed...
	if (_changeMovement)
	{
		setCurrentMovement (_movementToChange);
		_changeMovement = false;
		_movementToChange = __MININT__;
	}

	// The artist updatePositions method is responsable for
	// detecting the collisions of the object with other
	// tiles or elements,...and to move the object itself...
	QGAMES::ArtistInATiledMap::updatePositions ();

	// The game passes through this point framesPerSecond times a second
	// So, Ten times a second the counter becomes equal 6
	// So, the entity changed its aspect ten times a second...
	if (_counter++ == 6)
	{
		_counter = 0;

		if (_data._orientation != QGAMES::Vector::_cero)
		{
			_currentAspect++;
			PacmanArtist::MovingTo d = 
					(_data._orientation == General::_e._up) ? PacmanArtist::_DUP :
					((_data._orientation == General::_e._left) ? PacmanArtist::_DLEFT : 
					((_data._orientation == General::_e._down) ? PacmanArtist::_DDOWN : PacmanArtist::_DRIGHT));
			confirmFinalAspect (d);
		}
	}
}

// ---
void PacmanArtist::confirmFinalAspect (PacmanArtist::MovingTo d)
{
	if (_currentAspect < _aspects [d][0] || _currentAspect > _aspects [d][1])
		_currentAspect = _aspects [d][0];
}

// ---
PacmanMonster::PacmanMonster (int id, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PacmanArtist (id, f, d),
		_pacman (NULL),
		_mode (_CHASE),
		_collegues (),
		_counterScatter (0),
		_counterChase (0),
		_counterCycle (0),
		_lastSpeed (__BD 0),
		_whereToGo (QGAMES::Position::_cero),
		_orientations (),
		_sizes ()
{ 
	// Nothing else to do so far...
}

// ---
void PacmanMonster::setState (PacmanArtist::State s)
{
	PacmanArtist::setState (s);
	for (QGAMES::Movements::const_iterator i = _data._movements.begin ();
			i != _data._movements.end (); i++)
		((PacmanEntityMovement*) ((*i).second)) -> 
			accelerate ((s == PacmanArtist::_DIE) ? 3 : 1); // When die..everything happens quicker...
}

// ---
void PacmanMonster::setMode (PacmanMonster::Mode m) 
{ 
	_mode = m;
}

// ---
void PacmanMonster::setMyTarget (Pacman* p)
{
	_pacman = p;
}

// ---
bool PacmanMonster::continueMoving ()
{
	// If the monster is died...
	// ...and it is exactly in the front of the pen, the movement has to stop...
	bool result = true;
	QGAMES::Position nMPos = ((MapMaze*) _map) -> centerTilePenPosition () +
		__BD (((MapMaze*) _map) -> tileHeight () * 2) * General::_e._up;
	if (position () == nMPos && _state == PacmanMonster::_DIE)
		result = false;
	return (result);
}

// ---
QGAMES::Vector PacmanMonster::nextMovementAtLimit (const QGAMES::Vector& d)
{ 
	return (nextMove ()); 
}

// ---
void PacmanMonster::initialize ()
{
	setMode (PacmanMonster::_WAITING);
	setState (PacmanArtist::_ALIVE);

	// Implementation...
	_counterScatter = _counterChase = _counterCycle = 0;
	_whereToGo = QGAMES::Position::_cero;
}

// ---
void PacmanMonster::updatePositions ()
{
	// First of all, it is necessary to manage the general form
	// of the monter. Somethinh that is done is the parent method.
	PacmanArtist::updatePositions ();

	// If the monster is waiting...nothing to do except keep waiting!
	if (_mode == PacmanMonster::_WAITING)
		return;

	// When the monster pass throught the pen position, and it is dead...
	// It is time to restablish the initial status, everything starts again...
	// ANy case, it is something defined by the method continueMoving!
	if (!continueMoving ())
		processEvent (QGAMES::Event (__EYESINFRONTOFPEN, this));
	
	// If the state of the monster is not alive
	// There is no change between the scatter and the chase mode...
	if (_state != PacmanMonster::_ALIVE)
		return;
	else
		setChaseOrScatter (); // Otherwhise there could be...
}

// ---
void PacmanMonster::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	#ifndef NDEBUG
	// Draws a line to indicate where to go...
	// Makes only sense when the game is running...
	s -> drawLine (position (), _whereToGo, __REDCOLOR, 3);
	s -> drawCircle (_whereToGo, QGAMES::Vector::_zNormal, 10, 10, __REDCOLOR);
	s -> drawRectangle (collisionZone (), __GRAYCOLOR);
	#endif
	PacmanArtist::drawOn (s, p);
}

// ---
void PacmanMonster::whenCollisionWith (const QGAMES::Tiles& t, QGAMES::TileLayer* l)
{
	QGAMES::Tile* rT = QGAMES::NullTile::_nullTile;
	if (!t.empty ()) rT = t [(int) t.size () - 1];

	// The collision has been with a portal
	// Then it is necessary to look for the other one...
	if (rT -> type () == __TILEGATE)
	{
		// The collision with portal 1 has to happen while going to the left...
		// Otherhise it would be valid at all...
		if (rT -> numberFrame () == __PORTALGATE1 && orientation () == General::_e._left)
			notify (QGAMES::Event (__ENTERPORTAL1, this));
		else
		// And the collision with portal 2 has to happen while going to the right...
		// Otherhise it would be valid at all...
		if (rT -> numberFrame () == __PORTALGATE2 && orientation () == General::_e._right)
			notify (QGAMES::Event (__ENTERPORTAL2, this));
	}
}

// ---
QGAMES::Vector PacmanMonster::nextMove ()
{
	// This vectors are to help the calculation...
	// The orientations...
	if (_orientations.empty ())
	{
		_orientations.clear ();
		_orientations.resize (4);
		_orientations [PacmanArtist::_DUP] = General::_e._up;
		_orientations [PacmanArtist::_DLEFT] = General::_e._left;
		_orientations [PacmanArtist::_DDOWN] = General::_e._down;
		_orientations [PacmanArtist::_DRIGHT] = General::_e._right;
	}

	// ...and the sizes...
	if (_sizes.empty ())
	{
		_sizes.clear ();
		_sizes.resize (4);
		_sizes [PacmanArtist::_DUP] = (__TM _map) -> tileHeight ();
		_sizes [PacmanArtist::_DLEFT] = (__TM _map) -> tileWidth ();
		_sizes [PacmanArtist::_DDOWN] = (__TM _map) -> tileHeight ();
		_sizes [PacmanArtist::_DRIGHT] = (__TM _map) -> tileWidth ();
	}

	// So it is time to calculate the different possibilities
	// the monster has to move...
	int nPossibilities = 0;
	std::vector <bool> possibilities;
	possibilities.resize (4);
	for (int i = 0; i < 4; i++)
	{
		if (_orientations [i] != -orientation ())
		{
			QGAMES::Vector pTile = position () + (__BD _sizes [i] * _orientations [i]);
			possibilities [i] = 
				((__TM _map) -> tileAt (pTile) -> type () != __TILEMAZE) ? true : false;
		if (possibilities [i])
			nPossibilities++;
		}
	}

	// The only possibility available is to go back...
	if (nPossibilities == 0)
		return (-1 * orientation ()); 

	// There is only one possibility...and it is not to go back...
	if (nPossibilities == 1)
	{
		QGAMES::Vector result = QGAMES::Vector::_cero;
		for (int i = 0; i < 4; i++)
			if (possibilities [i])
				result = _orientations [i];
		return (result);
	}

	// There are more than one possibility,
	// The one to be choosen is the one with the minimum distance to the target position...
	PacmanGame* g = (PacmanGame*) PacmanGame::game ();
	QGAMES::Position tPos = QGAMES::Position::_cero;
	if (_state == PacmanMonster::_ALIVE)
	{
		if (g -> mode () == PacmanGame::_NORMAL)
			tPos = (_mode == PacmanMonster::_CHASE) ? targetPosition () : scatterPosition ();
		else
			tPos = scatterPosition (); // In frightning mode...to the headquarter
	}
	// When the monster is died, the pen position has to be found...
	else
		tPos = g -> monsterPenPosition ();
	_whereToGo = tPos;

	// Once the target position has been determinated,
	// it is time to calculate the distance to each and select the one with the minimal
	int lD = __MAXINT__;
	int o = -1;
	for (int i = 0; i != 4; i++)
	{
		QGAMES::Position pos = position () + (__BD _sizes [i] * _orientations [i]);
		QGAMES::bdata d = pos.distanceTo (tPos);
		if (possibilities [i] && (d < __BD lD))
		{
			lD = (int) d;
			o = i;
		}
	}

	return (_orientations [o]);
}

// ---
void PacmanMonster::confirmFinalAspect (PacmanArtist::MovingTo d)
{
	PacmanGame* game = (PacmanGame*) PacmanGame::game ();

	// The mosnter is alive...
	if (_state == PacmanArtist::_ALIVE)
	{
		// ...and the game is in a normal situation...
		if (game -> mode () == PacmanGame::_NORMAL)
		{
			// so the aspect depends on the position of the monster...
			if (_currentAspect < _aspects [d][0] || _currentAspect > _aspects [d][1])
				_currentAspect = _aspects [d][0];
		}
		else
		// The game is returning to a normal mode...
		if (game -> mode () == PacmanGame::_SEMIFRIGHTENING)
		{
			// ...so the monster is blinking between blue and white...
			// Here is white...
			_counterPanic++;
			if (_counterPanic >= 2 || _counterPanic < 0)
			{
				_counterPanic = 0;
				if (_currentAspect < _aspects [5][0] || _currentAspect > _aspects [5][1])
					_currentAspect = _aspects [5][0];
			}
			// ...here is blue
			else
			{
				if (_currentAspect < _aspects [4][0] || _currentAspect > _aspects [4][1])
					_currentAspect = _aspects [4][0];
			}
		}
		// The game is in the frightening mode...
		else
		{
			if (_currentAspect < _aspects [4][0] || _currentAspect > _aspects [4][1])
				_currentAspect = _aspects [4][0];
		}
	}
	// The monster is died...
	else
	if (_currentAspect < _aspects [6][0] || _currentAspect > _aspects [6][1])
		_currentAspect = _aspects [6][0];
}

// ---
void PacmanMonster::setChaseOrScatter ()
{
	// Four times changing between chase and scatter...
	// ...and the definitive mode is chase
	if (_counterCycle == 4)
	{
		setMode (PacmanMonster::_CHASE);
		return; // From cycle 4, always in chase...
	}

	// In frightening mode, the behaviour is "suspended"...
	// Whe the mode finishes, then the monsters return to their state.
	PacmanGame* g = (PacmanGame*) PacmanGame::game ();
	if (g -> mode () == PacmanGame::_FRIGHTENING)
		return;

	// This is the table related with the time standing at one specific state
	// This table has been published at: 
	// http://home.comcast.net/~jpittman2/pacman/pacmandossier.html#CH2_Scatter_Chase_Repeat
	// as part of the pacman dossier published by Jamey Pitmann
	// It can be parametrized as part of the level...
	// If the level is 1...
	if (g -> level () == 0)
	{
		if (mode () == PacmanMonster::_SCATTER)
		{
			if (_counterCycle < 2)
			{
				// Only 7 seconds at this state, two firsts times...
				if (_counterScatter++ >= (7 * g -> framesPerSecond ())) 
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}

			if (_counterCycle > 2 && _counterCycle < 4)
			{
				// Second time and further, only 5 seconds on the state...
				if (_counterScatter++ >= (5 * g -> framesPerSecond ())) 
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}
		}
		// ...in chasse mode...
		else
		{
			// first 4 times...
			if (_counterCycle < 4)
			{
				// ...20 seconds after the pacman...
				if (_counterChase++ >= (20 * g -> framesPerSecond ()))
				{
					_counterChase = 0;
					setMode (PacmanMonster::_SCATTER);
				}
			}

			_counterCycle++;
		}
	}

	// Apply same rules than above to read and interpret the code...
	// Between levels 2 and 4
	if (g -> level () >= 1 && g -> level () <= 3)
	{
		if (mode () == PacmanMonster::_SCATTER)
		{
			if (_counterCycle < 2)
			{
				if (_counterScatter++ >= (7 * g -> framesPerSecond ()))
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}

			if (_counterCycle == 2)
			{
				if (_counterScatter++ >= (5 * g -> framesPerSecond ()))
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}

			if (_counterCycle == 3)
			{
				if (_counterScatter++ == 1)
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}
		}
		else
		{
			if (_counterCycle < 2)
			{
				if (_counterChase++ >= (20 * g -> framesPerSecond ()))
				{
					_counterChase = 0;
					setMode (PacmanMonster::_SCATTER);
				}
			}

			if (_counterCycle == 3)
			{
				if (_counterChase++ >= (1033 * g -> framesPerSecond ()))
				{
					_counterChase = 0;
					setMode (PacmanMonster::_SCATTER);
				}
			}

			_counterCycle++;
		}
	}

	// Apply same rules than above to read and interpret the code...
	// Beyond level 5...
	if (g -> level () >= 4)
	{
		if (mode () == PacmanMonster::_SCATTER)
		{
			if (_counterCycle < 3)
			{
				if (_counterScatter++ >= (5 * g -> framesPerSecond ()))
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}

			if (_counterCycle == 3)
			{
				if (_counterScatter++ == 1)
				{
					_counterScatter = 0;
					setMode (PacmanMonster::_CHASE);
				}
			}
		}
		else
		{
			if (_counterCycle < 2)
			{
				if (_counterChase++ >= (20 * g -> framesPerSecond ()))
				{
					_counterChase = 0;
					setMode (PacmanMonster::_SCATTER);
				}
			}

			if (_counterCycle == 3)
			{
				if (_counterChase++ >= (1037 * g -> framesPerSecond ()))
				{
					_counterChase = 0;
					setMode (PacmanMonster::_SCATTER);
				}
			}

			_counterCycle++;
		}
	}
}

// ---
void Pacman::initialize ()
{
	_aspects [PacmanArtist::_DUP][0] = __PACMANUPMINFRAME; 
	_aspects [PacmanArtist::_DUP][1] = __PACMANUPMAXFRAME;
	_aspects [PacmanArtist::_DLEFT][0] = __PACMANLEFTMINFRAME; 
	_aspects [PacmanArtist::_DLEFT][1] = __PACMANLEFTMAXFRAME;
	_aspects [PacmanArtist::_DDOWN][0] = __PACMANDOWNMINFRAME; 
	_aspects [PacmanArtist::_DDOWN][1] = __PACMANDOWNMAXFRAME;
	_aspects [PacmanArtist::_DRIGHT][0] = __PACMANRIGHTMINFRAME; 
	_aspects [PacmanArtist::_DRIGHT][1] = __PACMANRIGHTMAXFRAME;
}

// ---
void Pacman::whenCollisionWith (const QGAMES::Tiles& t, QGAMES::TileLayer* l)
{
	QGAMES::Tile* rT = QGAMES::NullTile::_nullTile;
	if (!t.empty ()) rT = t [(int) t.size () - 1];

	// Collision with a ball...
	// It could be a normal one, or a ball with power...
	if (rT -> type () == __TILEBALL)
	{
		if (rT -> numberFrame () == __BALLFRAME) // A normal ball has been eated...
			notify (QGAMES::Event (__BALLEATED, rT));
		if (rT -> numberFrame () == __SUPERBALLFRAME) // The pwer ball has been eated...
			notify (QGAMES::Event (__SUPERBALLEATED, rT));
	}
	else
	// The collision has been with the fruit...
	if (rT -> type () == __TILEFRUIT)
	{
		if (rT -> numberFrame () != __FRUITFRAMEEMPTY)
			notify (QGAMES::Event (__FRUITEATEN, rT));
	}
	// The collision has been with a portal
	// Then it is necessary to look for the other one...
	else
	if (rT -> type () == __TILEGATE)
	{
		// The collision with portal 1 has to happen while going to the left...
		// Otherhise it would be valid at all...
		if (rT -> numberFrame () == __PORTALGATE1 && orientation () == General::_e._left)
			notify (QGAMES::Event (__ENTERPORTAL1, this));
		else
		// And the collision with portal 2 has to happen while going to the right...
		// Otherhise it would be valid at all...
		if (rT -> numberFrame () == __PORTALGATE2 && orientation () == General::_e._right)
			notify (QGAMES::Event (__ENTERPORTAL2, this));
	}
}

// ---
QGAMES::Position Blinky::targetPosition () const
{
	// The target is always where pacman is...
	return (_pacman -> position ());
}

// ---
void Blinky::initialize ()
{
	PacmanMonster::initialize ();

	setCurrentMovement (__MOVPENBLINKY);
	setOrientation (General::_e._down);
	if (_map != NULL)
		setPosition (((MapMaze*) _map) -> centerTilePenPosition () + 
			(tilesAtBeginning () * __BD ((MapMaze*) _map) -> tileWidth ()) * General::_e._right);
	// The first movement should be in the pen...
	// so, the first position is in the pen...

	_aspects [PacmanArtist::_DUP][0] = __BLINKYUPMINFRAME; 
	_aspects [PacmanArtist::_DUP][1] = __BLINKYUPMAXFRAME;
	_aspects [PacmanArtist::_DLEFT][0] = __BLINKYLEFTMINFRAME; 
	_aspects [PacmanArtist::_DLEFT][1] = __BLINKYLEFTMAXFRAME;
	_aspects [PacmanArtist::_DDOWN][0] = __BLINKYDOWNMINFRAME; 
	_aspects [PacmanArtist::_DDOWN][1] = __BLINKYDOWNMAXFRAME;
	_aspects [PacmanArtist::_DRIGHT][0] = __BLINKYRIGHTMINFRAME; 
	_aspects [PacmanArtist::_DRIGHT][1] = __BLINKYRIGHTMAXFRAME;

	_aspects [4][0] = __MONSTERSCAREDMINFRAME;
	_aspects [4][1] = __MONSTERSCAREDMAXFRAME;
	_aspects [5][0] = __MONSTERSEMISCAREDMINFRAME;
	_aspects [5][1] = __MONSTERSEMISCAREDMAXFRAME;
	_aspects [6][0] = __MONSTERDIEMINFRAME;
	_aspects [6][1] = __MONSTERDIEMAXFRAME;
}

// ---
void Blinky::processEvent (const QGAMES::Event& e)
{
	PacmanMonster::processEvent (e);

	// After some time into the pen...the monster exists it
	if (e.code () == __INTHEPEN_MOVCOMPLETED)
		setMovementToChange (__MOVEXITPENBLINKY);

	// After exit the pen, the movement across the maze starts...
	if (e.code () == __EXITINGPEN_COMPLETED)
	{
		setMovementToChange (__MOVMAZEBLINKY);
		setMode (_SCATTER);
	}

	// When die, an the center of the tile has been achieved
	if (e.code () == __EYESINFRONTOFPEN)
		setMovementToChange (__MOVENTERPENBLINKY);

	// After die, the center of the pen has been achieved back...
	if (e.code () == __ENTERINGPEN_COMPLETED)
	{
		_initialize = true;
		setMovementToChange (__MOVPENBLINKY);
	}

	// The movement can not be change directly
	// When a new movement is set (setCurrentMovement) the old
	// movement becomes unObserved, it means it is remove from the 
	// list of notifiers being observed by the entity, and also
	// the entity is removed from the list of entities to be notified
	// But at this point the list is being iterated, so an error will happen...

	// The same situation happens with the initialization
}

// ---
QGAMES::Position Pinky::targetPosition () const
{
	// The target of blinky is 4 tiles ahead where pacman is...
	return (_pacman -> position () + 
		(__BD ((4 * (__TM _map) -> tileWidth ())) * _pacman -> orientation ()));
}

// ---
void Pinky::initialize ()
{
	PacmanMonster::initialize ();

	setCurrentMovement (__MOVPENPINKY);
	setOrientation (General::_e._up);
	if (_map != NULL)
		setPosition (((MapMaze*) _map) -> centerTilePenPosition () + 
			(tilesAtBeginning () * __BD ((MapMaze*) _map) -> tileWidth ()) * General::_e._right);
	// The first movement should be in the pen...
	// so, the first position is in the pen...

	_aspects [PacmanArtist::_DUP][0] = __PINKYUPMINFRAME; 
	_aspects [PacmanArtist::_DUP][1] = __PINKYUPMAXFRAME;
	_aspects [PacmanArtist::_DLEFT][0] = __PINKYLEFTMINFRAME; 
	_aspects [PacmanArtist::_DLEFT][1] = __PINKYLEFTMAXFRAME;
	_aspects [PacmanArtist::_DDOWN][0] = __PINKYDOWNMINFRAME; 
	_aspects [PacmanArtist::_DDOWN][1] = __PINKYDOWNMAXFRAME;
	_aspects [PacmanArtist::_DRIGHT][0] = __PINKYRIGHTMINFRAME; 
	_aspects [PacmanArtist::_DRIGHT][1] = __PINKYRIGHTMAXFRAME;

	_aspects [4][0] = __MONSTERSCAREDMINFRAME;
	_aspects [4][1] = __MONSTERSCAREDMAXFRAME;
	_aspects [5][0] = __MONSTERSEMISCAREDMINFRAME;
	_aspects [5][1] = __MONSTERSEMISCAREDMAXFRAME;
	_aspects [6][0] = __MONSTERDIEMINFRAME;
	_aspects [6][1] = __MONSTERDIEMAXFRAME;
}

// ---
void Pinky::processEvent (const QGAMES::Event& e)
{
	PacmanMonster::processEvent (e);

	// After some time in the pen, the monster starts to exit it...
	if (e.code () == __INTHEPEN_MOVCOMPLETED)
		setMovementToChange (__MOVEXITPENPINKY);

	// After exit the pen, the movement across the maze starts...
	if (e.code () == __EXITINGPEN_COMPLETED)
	{
		setMovementToChange (__MOVMAZEPINKY);
		setMode (_SCATTER);
	}

	// When die, an the center of the tile has been achieved
	if (e.code () == __EYESINFRONTOFPEN)
		setMovementToChange (__MOVENTERPENPINKY);

	// After die, the center of the pen has been achieved back...
	if (e.code () == __ENTERINGPEN_COMPLETED)
	{
		_initialize = true;
		setMovementToChange (__MOVPENPINKY);
	}

	// See Blinky explanation...
}

// ---
QGAMES::Position Inky::targetPosition () const
{
	// The target of inky is twice the position of the pacman plus 
	// the distance between blinky and pacman...
	Blinky* blinky = NULL;
	for (PacmanMonsters::const_iterator i = _collegues.begin ();
			i != _collegues.end () && ! blinky; i++)
		if ((*i) -> whoAreYou () == PacmanArtist::_BLINKY)
			blinky = (Blinky*) (*i);

	return (2 * ((_pacman -> position () + 
		__BD ((2 * (__TM _map) -> tileWidth ())) * _pacman -> orientation ()) - 
			blinky -> position ()));
}

// ---
void Inky::initialize ()
{
	PacmanMonster::initialize ();

	setCurrentMovement (__MOVPENINKY);
	setOrientation (General::_e._down);
	if (_map != NULL)
		setPosition (((MapMaze*) _map) -> centerTilePenPosition () + 
			(tilesAtBeginning () * __BD ((MapMaze*) _map) -> tileWidth ()) * General::_e._right);
	// The first movement should be in the pen...
	// so, the first position is in the pen...

	_aspects [PacmanArtist::_DUP][0] = __INKYUPMINFRAME; 
	_aspects [PacmanArtist::_DUP][1] = __INKYUPMAXFRAME;
	_aspects [PacmanArtist::_DLEFT][0] = __INKYLEFTMINFRAME; 
	_aspects [PacmanArtist::_DLEFT][1] = __INKYLEFTMAXFRAME;
	_aspects [PacmanArtist::_DDOWN][0] = __INKYDOWNMINFRAME; 
	_aspects [PacmanArtist::_DDOWN][1] = __INKYDOWNMAXFRAME;
	_aspects [PacmanArtist::_DRIGHT][0] = __INKYRIGHTMINFRAME; 
	_aspects [PacmanArtist::_DRIGHT][1] = __INKYRIGHTMAXFRAME;

	_aspects [4][0] = __MONSTERSCAREDMINFRAME;
	_aspects [4][1] = __MONSTERSCAREDMAXFRAME;
	_aspects [5][0] = __MONSTERSEMISCAREDMINFRAME;
	_aspects [5][1] = __MONSTERSEMISCAREDMAXFRAME;
	_aspects [6][0] = __MONSTERDIEMINFRAME;
	_aspects [6][1] = __MONSTERDIEMAXFRAME;
}

// ---
void Inky::processEvent (const QGAMES::Event& e)
{
	PacmanMonster::processEvent (e);

	// After sometime into the pen, the mosnter starts to exit it...
	if (e.code () == __INTHEPEN_MOVCOMPLETED)
		setMovementToChange (__MOVEXITPENINKY);

	// After exit the pen, the movement across the maze starts...
	if (e.code () == __EXITINGPEN_COMPLETED)
	{
		setMovementToChange (__MOVMAZEINKY);
		setMode (_SCATTER);
	}

	// When die, an the center of the tile has been achieved
	if (e.code () == __EYESINFRONTOFPEN)
		setMovementToChange (__MOVENTERPENINKY);

	// After die, the center of the pen has been achieved back...
	if (e.code () == __ENTERINGPEN_COMPLETED)
	{
		_initialize = true;
		setMovementToChange (__MOVPENINKY);
	}

	// See Blinky explanation...
}

// ---
QGAMES::Position Clyde::targetPosition () const
{
	// The target of clyde depends on the distance to pacman...
	// If it is 8 tiles or greater, then goes for pacman, otherwhise it is the scatter position...
	return (((_pacman -> position ().distanceTo (position ()) / 
		(__TM _map) -> tileWidth ()) > __BD 8) ? _pacman -> position () : scatterPosition ());
}

// ---
void Clyde::initialize ()
{
	PacmanMonster::initialize ();

	setCurrentMovement (__MOVPENCLYDE);
	setOrientation (General::_e._up);
	if (_map != NULL)
		setPosition (((MapMaze*) _map) -> centerTilePenPosition () + 
			(tilesAtBeginning () * __BD ((MapMaze*) _map) -> tileWidth ()) * General::_e._right);
	// The first movement should be in the pen...
	// so, the first position is in the pen...

	_aspects [PacmanArtist::_DUP][0] = __CLYDEUPMINFRAME; 
	_aspects [PacmanArtist::_DUP][1] = __CLYDEUPMAXFRAME;
	_aspects [PacmanArtist::_DLEFT][0] = __CLYDELEFTMINFRAME; 
	_aspects [PacmanArtist::_DLEFT][1] = __CLYDELEFTMAXFRAME;
	_aspects [PacmanArtist::_DDOWN][0] = __CLYDEDOWNMINFRAME; 
	_aspects [PacmanArtist::_DDOWN][1] = __CLYDEDOWNMAXFRAME;
	_aspects [PacmanArtist::_DRIGHT][0] = __CLYDERIGHTMINFRAME; 
	_aspects [PacmanArtist::_DRIGHT][1] = __CLYDERIGHTMAXFRAME;

	_aspects [4][0] = __MONSTERSCAREDMINFRAME;
	_aspects [4][1] = __MONSTERSCAREDMAXFRAME;
	_aspects [5][0] = __MONSTERSEMISCAREDMINFRAME;
	_aspects [5][1] = __MONSTERSEMISCAREDMAXFRAME;
	_aspects [6][0] = __MONSTERDIEMINFRAME;
	_aspects [6][1] = __MONSTERDIEMAXFRAME;
}

// ---
void Clyde::processEvent (const QGAMES::Event& e)
{
	PacmanMonster::processEvent (e);

	// AFter sometime into the pen, the monster starts to exit it...
	if (e.code () == __INTHEPEN_MOVCOMPLETED)
		setMovementToChange (__MOVEXITPENCLYDE);

	// After exit the pen, the movement across the maze starts...
	if (e.code () == __EXITINGPEN_COMPLETED)
	{
		setMovementToChange (__MOVMAZECLYDE);
		setMode (_SCATTER);
	}

	// When die, an the center of the tile has been achieved
	if (e.code () == __EYESINFRONTOFPEN)
		setMovementToChange (__MOVENTERPENCLYDE);

	// After die, the center of the pen has been achieved back...
	if (e.code () == __ENTERINGPEN_COMPLETED)
	{
		_initialize = true;
		setMovementToChange (__MOVPENCLYDE);
	}

	// See Blinky explanation...
}

// ---
QGAMES::Entity* ScoreDigit::clone ()
{
	ScoreDigit* result = new ScoreDigit (_id);
	return (result);
}

// ---
ScorePoints::ScorePoints () 
	: CompositeEntity (__ENTITYPOINTS, QGAMES::Entities ()),
	  _score (0),
	  _pendingToAdd (),
	  _adding (false),
	  _valueBeingAdded (0),
	  _counterAdded (0)
{
	// Nothing else to do...
}

// ---
ScorePoints::~ScorePoints ()
{
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete (*i).second;
}

// ---
QGAMES::Entity* ScorePoints::clone ()
{
	ScorePoints* result = new ScorePoints ();
	result -> initialize ();
	return (result);
}

// ---
void ScorePoints::add (int n)
{
	_score += n;
	_pendingToAdd.push_back (n);
}

// ---
void ScorePoints::initialize ()
{
	// Clear the previous scores...
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete (*i).second;
	_entities.clear ();

	// Init the control internal variables...
	_score = 0;
	_pendingToAdd.clear ();
	_adding = false;
	_valueBeingAdded = 0;
	_counterAdded = 0;

	// Adds the digits...
	_entities.insert (QGAMES::Entities::value_type (__ENTITYPOINTS + 1,
		new ScoreDigit (__ENTITYPOINTS + 1)));
	_entities.insert (QGAMES::Entities::value_type (__ENTITYPOINTS + 2,
		new ScoreDigit (__ENTITYPOINTS + 2)));
	_entities.insert (QGAMES::Entities::value_type (__ENTITYPOINTS + 3,
		new ScoreDigit (__ENTITYPOINTS + 3)));
	_entities.insert (QGAMES::Entities::value_type (__ENTITYPOINTS + 4,
		new ScoreDigit (__ENTITYPOINTS + 4)));
	_entities.insert (QGAMES::Entities::value_type (__ENTITYPOINTS + 5,
		new ScoreDigit (__ENTITYPOINTS + 5)));

	// Init the digits...
	// Place them in their right position and with the right aspect (a 0)
	int counter = 0;
	QGAMES::Forms frms;
	QGAMES::Form* frm = QGAMES::ArcadeGame::game () -> form (__NUMBERSFORM);
	frms.insert (QGAMES::Forms::value_type (frm -> id (), frm));
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		(*i).second -> setPosition (QGAMES::Position (__BD 42 + (frm -> frameWidth () * counter++),
			__BD (__SCREENHEIGHT__ - frm -> frameHeight () - 10), __BD 0));
		(*i).second -> setForms (frms);
		(*i).second -> setCurrentForm (frm -> id ());
		(*i).second -> setCurrentAspect (0);
	}
}

// ---
void ScorePoints::updatePositions ()
{
	if (!_adding)
	{
		if (!_pendingToAdd.empty ())
		{
			_valueBeingAdded = _pendingToAdd.back ();
			_counterAdded = 0;
			_pendingToAdd.pop_back ();
			_adding = (_valueBeingAdded != 0); // Only greater than 0 is allowed...
		}
	}
	else
	{
		bool carry = false;
		int entityNumber = __ENTITYPOINTS + 5;
		do
		{
			carry = false;
			QGAMES::Entity* e = (*_entities.find (entityNumber)).second;
			if (e -> currentAspect () == __NINE)
			{
				e -> setCurrentAspect (__CERO);
				carry = true;
				entityNumber--;
			}
			else
				e -> setCurrentAspect (e -> currentAspect () + 1);
		} 
		while (carry);

		if (++_counterAdded == _valueBeingAdded)
			_adding = false; // Next?
	}
}

// ---
QGAMES::Entity* ScoreLive::clone ()
{
	ScoreLive* result = new ScoreLive (_id);
	return (result);
}

// ---
ScoreLives::ScoreLives ()
	: CompositeEntity (__ENTITYLIVES, QGAMES::Entities ()),
	  _lives (0),
	  _maxLives (__MAXNUMBEROFLIVES__)
{
}

// ---
ScoreLives::~ScoreLives ()
{
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete (*i).second;
}

// ---
QGAMES::Entity* ScoreLives::clone ()
{
	ScoreLives* result = new ScoreLives ();
	result -> initialize ();
	return (result);
}

// ---
void ScoreLives::setLives (int l)
{
	_lives = (l > _maxLives) ? _maxLives : l;

	QGAMES::Entities::const_reverse_iterator j = _entities.rbegin ();
	for (int i = _maxLives; i > _lives; i --, j++)
		(*j).second -> setCurrentAspect (__NONEFRAME);
}

// ---
void ScoreLives::initialize ()
{
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
		delete (*i).second;

	_lives = _maxLives;

	for (int j = 0; j < _maxLives; j++)
		_entities.insert (QGAMES::Entities::value_type (__ENTITYLIVES + j + 1,
			new ScoreLive (__ENTITYPOINTS + j + 1)));

	int counter = 0;
	QGAMES::Forms frms;
	QGAMES::Form* frm = QGAMES::ArcadeGame::game () -> form (__SPRITESFORM);
	frms.insert (QGAMES::Forms::value_type (frm -> id (), frm));
	for (QGAMES::Entities::iterator i = _entities.begin ();
			i != _entities.end (); i++)
	{
		(*i).second -> setPosition (QGAMES::Vector (__BD 
			(__SCREENWIDTH__ - (frm -> frameWidth () * (1 + counter++)) + 32),
				__BD (__SCREENHEIGHT__ - frm -> frameHeight () - 10), __BD 0));
		(*i).second -> setForms (frms);
		(*i).second -> setCurrentForm (frm -> id ());
		(*i).second -> setCurrentAspect (__PACMANRIGHTMAXFRAME);
	}
}
