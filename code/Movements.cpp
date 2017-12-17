#include "Movements.hpp"
#include "Entities.hpp"
#include "Maps.hpp"
#include "General.hpp"

// ---
PacmanEntityMovement::PacmanEntityMovement (int id, std::map <int, double> v)
		: QGAMES::Movement (id, v),
		  _speed (__BASESPEED),
		  _changedSpeed (-1),
		  _accelerated ( 1),
		  _counterTSpeed (0)
{
	_counterMSpeed[0] = _counterMSpeed[1] = 0;
}

// ---
void PacmanEntityMovement::accelerate (int t) 
{
	_accelerated = (t < 0) ? 0 : t;
}

// ---
void PacmanEntityMovement::setSpeed (int s) 
{
	_speed = s;
	if (_speed == 0)
		_speed = __BASESPEED;
	_changedSpeed = _speed; 
}

// ---
void PacmanEntityMovement::initialize ()
{
	// The speed is the variable 0, or the number changed during the game...
	_speed = (_changedSpeed == -1) ? (int) variable (0) : _changedSpeed;
	_accelerated = 1; // No acceleration...
	if (_speed == 0)
		_speed = __BASESPEED; // If the result is 0, then the minimum speed allowed is the base one...
}

// ---
void PacmanEntityMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	// The ways the pacman entity moves, depends 
	// whether the speed is greater or less than the base speed...
	int vel = _speed * _accelerated;

	// If greater...
	if (vel >= __BASESPEED)
	{
		// Moves x times points the speed...
		// As it is possible to move more than one pixel in this loop
		// It is necessary to control the entity can move more than one
		// or it must stop because it reaches
		bool contMove = true;
		for (_counterTSpeed = (int) (vel / __BASESPEED); 
			_counterTSpeed > 0 && contMove; _counterTSpeed--)
		{
			moveOne (d, a, e); // Any time, it moves only one...
			if (!((PacmanArtist*) e) -> continueMoving ())
				contMove = false;
		}

		// Has it to continue moving...
		if (!contMove)
			return; // no...

		// If something rests...
		// Takes into account two decimals...
		// When the monster passes throught this point
		// it can only move one pixel more maximum...
		if ((vel % __BASESPEED) != 0)
		{
			if (_counterMSpeed[0]++ == (int) ((__BASESPEED / (vel % __BASESPEED))))
			{
				_counterMSpeed[0] = 0;
				if ((__BASESPEED % (vel % __BASESPEED)) != 0)
				{
					if (_counterMSpeed[1]++ == 
						((vel % __BASESPEED) / (__BASESPEED % (vel % __BASESPEED))))
						_counterMSpeed[1] = 0;
					else
						moveOne (d, a, e);
				}
			}
			else
				moveOne (d, a, e); // Any time, it moves only one...
		}
	}

	// If less...
	// In all the situations the monsters moves here
	// it moves only one pixel...
	else
	{
		if (_counterMSpeed[0]++ == (int) ((__BASESPEED / (vel % __BASESPEED))))
		{
			_counterMSpeed[0] = 0;
			moveOne (d, a, e); // Any time it moves only one...
			if ((__BASESPEED % (vel % __BASESPEED)) != 0)
			{
				if (_counterMSpeed[1]++ < 
					((vel % __BASESPEED) / (__BASESPEED % (vel % __BASESPEED))))
					moveOne (d, a, e); // Any time, it moves only one...
				else
					_counterMSpeed[1] = 0;
			}
		}
	}
}

// ---
MovementIntoThePen::MovementIntoThePen (int id, std::map <int, double> v)
		: PacmanEntityMovement (id, v),
		  _direction (1),
		  _counter (0),
		  _distance (0),
		  _firstMove (true),
		  _currentPosition (0),
		  _initialPosition (QGAMES::Position::_cero)
{
	// Nothing else to do...
}

// ---
QGAMES::Movement* MovementIntoThePen::clone ()
{
	QGAMES::Movement* result = new MovementIntoThePen (_id, _variables);
	result -> initialize ();
	return (result);
}

// ---
QGAMES::Vector MovementIntoThePen::direction () const
{
	return (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));
}

// ---
void MovementIntoThePen::initialize ()
{
	PacmanEntityMovement::initialize ();
	_firstMove = true;
	int maxLoops = (int) variable (1);
	_counter = (maxLoops == 0) ? (rand () % 100) : (rand () % maxLoops);
}

// ---
void MovementIntoThePen::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	// In this type of movement, the parameters are not relevant...
	// The movement is executed taken only into account the internal variables
	// of the movement...

	if (_firstMove)
	{
		_firstMove = false;
		// It only works with artists...
		_distance = (__TM (__AT e) -> map ()) -> tileHeight ();
		_initialPosition = e -> position ();
		_direction = (int) e -> orientation ().posY ();
		_currentPosition = 0;
	}

	if ((_currentPosition <= (-_distance / 2)) || 
		(_currentPosition >= (_distance / 2)))
		_direction = -_direction;
	_currentPosition += _direction;
	e -> setOrientation (QGAMES::Vector (__BD 0, __BD _direction, __BD 0));
	e -> setPosition (_initialPosition + (__BD _currentPosition * General::_e._down));

	// When the position pass throught the 0, a loop has been completed
	// and that situation is notified...
	if (_currentPosition == __BD 0 && _direction == 1)
	{
		notify (QGAMES::Event (__INTHEPEN_MOVCYCLECOMPLETED, this));
		if (_counter-- <= 0)
			notify (QGAMES::Event (__INTHEPEN_MOVCOMPLETED, this));
	}
}

// ---
MovementExitingThePen::MovementExitingThePen (int id, std::map <int, double> v)
		: PacmanEntityMovement (id, v),
		  _direction (0),
		  _firstMove (true),
		  _initialPosition (QGAMES::Position::_cero),
		  _firstTargetPosition (QGAMES::Position::_cero),
		  _finalTargetPosition (QGAMES::Position::_cero)
{
	// Nothing else to do...
}

// ---
QGAMES::Movement* MovementExitingThePen::clone ()
{
	QGAMES::Movement* result = new MovementExitingThePen (_id, _variables);
	result -> initialize ();
	return (result);
}

// ---
QGAMES::Vector MovementExitingThePen::direction () const
{
	QGAMES::Vector result = QGAMES::Vector::_cero;
	if (_direction == 0) // Left
		result = General::_e._left;
	if (_direction == 1) // Down
		result = General::_e._down;
	if (_direction == 2) // Right
		result = General::_e._right;
	if (_direction == 3) // Up
		result = General::_e._up;
	if (_direction == 4) // Going out
		result = General::_e._up;
	return (result);
}

// ---
void MovementExitingThePen::initialize ()
{
	PacmanEntityMovement::initialize ();
	_firstMove = true;
}

// ---
void MovementExitingThePen::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	// In this type of movement, the parameters are not relevant...
	// The movement is executed taken only into account the internal variables
	// of the movement...

	if (_firstMove)
	{
		_firstMove = false;
		// It only works with artists...
		_initialPosition = e -> position ();
		_firstTargetPosition = ((MapMaze*) (__AT e) -> map ()) -> centerTilePenPosition ();
		_finalTargetPosition = _firstTargetPosition + // Two tiles up...
			__BD ((__TM (__AT e) -> map ()) -> tileHeight () * 2) * General::_e._up;
		_direction = 0;
	}
	
	// Left?
	QGAMES::Vector o = QGAMES::Vector::_cero;
	if (_direction == 0) 
	{
		if (e -> position ().posX () <= _firstTargetPosition.posX ())
			_direction++;
		else
			o += General::_e._left;
	}

	// Down?
	if (_direction == 1)
	{
		if (e -> position ().posY () >= _firstTargetPosition.posY ())
			_direction++;
		else
			o += General::_e._down;
	}

	// Right?
	if (_direction == 2)
	{
		if (e -> position ().posX () >= _firstTargetPosition.posX ())
			_direction++;
		else
			o += General::_e._right;
	}

	// Up?
	if (_direction == 3)
	{
		if (e -> position ().posY () <= _firstTargetPosition.posY ())
			_direction++;
		else
			o += General::_e._up;
	}

	// Going up?
	if (_direction == 4)
	{
		if (e -> position ().posY () <= _finalTargetPosition.posY ())
			notify (QGAMES::Event (__EXITINGPEN_COMPLETED, this)); // The end if the movement...
		else
			o += General::_e._up;
	}

	e -> setOrientation (o);
	e -> setPosition (e -> position () + o);
	// Adjust processes...

	if (_direction == 0 && e -> position ().posX () < _firstTargetPosition.posX ()) // adjust left?
		e -> setPosition (_firstTargetPosition);
	if (_direction == 1 && e -> position ().posY () > _firstTargetPosition.posY ()) // adjust down?
		e -> setPosition (_firstTargetPosition);
	if (_direction == 2 && e -> position ().posX () > _firstTargetPosition.posX ()) // adjust right?
		e -> setPosition (_firstTargetPosition);
	if (_direction == 3 && e -> position ().posY () < _firstTargetPosition.posY ()) // adjust up?
		e -> setPosition (_firstTargetPosition);
	if (_direction == 4 && e -> position ().posY () < _finalTargetPosition.posY ()) // adjust going up?
		e -> setPosition (_finalTargetPosition);
}

// ---
MovementInTheMaze::MovementInTheMaze (int id, std::map <int, double> v)
		: PacmanEntityMovement (id, v),
		  _moving (false),
		  _directionMovement (QGAMES::Vector::_cero),
		  _potentialMovement (QGAMES::Vector::_cero),
		  _numberSteps (0)
{
	// Nothing else to do...
}

// ---
QGAMES::Movement* MovementInTheMaze::clone ()
{
	QGAMES::Movement* result = new MovementInTheMaze (_id, _variables); 
	result -> initialize (); 
	return (result); 
}

// ---
void MovementInTheMaze::initialize ()
{
	PacmanEntityMovement::initialize ();

	_moving = false;
	_directionMovement = QGAMES::Vector::_cero;
	_potentialMovement = QGAMES::Vector::_cero;
	_numberSteps = 0;
}

// ---
void MovementInTheMaze::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	_potentialMovement = d;
	QGAMES::TiledMap* m = __TM (((Pacman*) e) -> map ());
	if (_moving)
	{
		// Which is the limit...(depends on the direction of the movement)
		int limit = (_directionMovement == General::_e._left || 
			_directionMovement == General::_e._right) ? m -> tileWidth () : m -> tileHeight ();

		// Advance...
		e -> setPosition (e -> position () + _directionMovement);
		// Orientation...
		e -> setOrientation (_directionMovement);

		// Determinates whether it has or not to change the movement...
		// It only happens when the limit has be achieved,
		// And the limit is the tile width! (or height in the vertical movements)
		// When the limit is achieved a new potential movement can be modified!
		if (++_numberSteps == limit)
		{
			_numberSteps = 0;
			_potentialMovement = ((PacmanArtist*) e) -> nextMovementAtLimit (_potentialMovement);
			changeMovementTo (_potentialMovement, e);
			if (_moving && ((PacmanArtist*) e) -> continueMoving ())
				move (_potentialMovement, QGAMES::Vector::_cero, e);
		}
	}
	else
	{
		_potentialMovement = ((PacmanArtist*) e) -> nextMovementAtLimit (_potentialMovement);
		changeMovementTo (_potentialMovement, e);
		if (_moving && ((PacmanArtist*) e) -> continueMoving ())
			move (_potentialMovement, QGAMES::Vector::_cero, e);
	}
}

// ---
void MovementInTheMaze::changeMovementTo (const QGAMES::Vector& d, QGAMES::Entity* e)
{
	QGAMES::TiledMap* m = __TM ((Pacman*) e) -> map ();
	if (d == General::_e._right || d == General::_e._left || 
		d == General::_e._up || d == General::_e._down)
	{
		QGAMES::Tile* t = m -> tileAt (e -> position () + 
			((QGAMES::bdata) m -> tileWidth () * d));
		
		_numberSteps = 0;
		if (t && t -> type () != __TILEMAZE)
		{
			_moving = true; // It can move...
			_directionMovement = d; 
		}
		else
		{
			_moving = false; // It can't move
			_directionMovement = QGAMES::Vector::_cero;
		}
	}
}

// ---
MovementEnteringThePen::MovementEnteringThePen (int id, std::map <int, double> v)
		: PacmanEntityMovement (id, v),
		  _direction (0),
		  _firstMove (true),
		  _initialPosition (QGAMES::Position::_cero),
		  _firstTargetPosition (QGAMES::Position::_cero),
		  _finalTargetPosition (QGAMES::Position::_cero)
{
	// Nothing else to do...
}

// ---
QGAMES::Movement* MovementEnteringThePen::clone ()
{
	QGAMES::Movement* result = new MovementEnteringThePen (_id, _variables);
	result -> initialize ();
	return (result);
}

// ---
QGAMES::Vector MovementEnteringThePen::direction () const
{
	QGAMES::Vector result = QGAMES::Vector::_cero;
	if (_direction == 0) // Going down
		result = General::_e._down;
	if (_direction == 1) // Left
		result = General::_e._left;
	if (_direction == 2) // Down
		result = General::_e._down;
	if (_direction == 3) // Right
		result = General::_e._right;
	if (_direction == 4) // Up
		result = General::_e._up;;
	return (result);
}

// ---
void MovementEnteringThePen::initialize ()
{
	PacmanEntityMovement::initialize ();
	_firstMove = true;
}

// ---
void MovementEnteringThePen::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	// In this type of movement, the parameters are not relevant...
	// The movement is executed taken only into account the internal variables
	// of the movement...
	if (_firstMove)
	{
		_firstMove = false;
		// It only works with artists...
		_initialPosition = e -> position ();
		_firstTargetPosition = ((MapMaze*) (__AT e) -> map ()) -> centerTilePenPosition ();
		_finalTargetPosition = _firstTargetPosition + 
			(__BD ((MapMaze*) (__AT e) -> map ()) -> tileWidth () * 
				((PacmanArtist*) e) -> tilesAtBeginning () *General::_e._right);
		_direction = 0; // Going down first...
	}

	// Going down?
	QGAMES::Vector o = QGAMES::Vector::_cero;
	if (_direction == 0) 
	{
		if (e -> position ().posY () >= _firstTargetPosition.posY ())
			_direction++;
		else
			o += General::_e._down;
	}

	// Left?
	if (_direction == 1)
	{
		if (e -> position ().posX () <= _finalTargetPosition.posX ())
			_direction++;
		else
			o += General::_e._left;
	}

	// Down?
	if (_direction == 2)
	{
		if (e -> position ().posY () >= _finalTargetPosition.posY ())
			_direction++;
		else
			o += General::_e._down;
	}

	// Right?
	if (_direction == 3)
	{
		if (e -> position ().posX () >= _finalTargetPosition.posX ())
			_direction++;
		else
			o += General::_e._right;
	}

	// Up?
	if (_direction == 4)
	{
		if (e -> position ().posY () <= _finalTargetPosition.posY ())
			notify (QGAMES::Event (__ENTERINGPEN_COMPLETED, this)); // The end if the movement...
		else
			o += General::_e._up;
	}

	e -> setOrientation (o);
	e -> setPosition (e -> position () + o);
	// Adjust processes...

	if (_direction == 0 && e -> position ().posX () > _firstTargetPosition.posX ()) // adjust going down?
		e -> setPosition (_firstTargetPosition);
	if (_direction == 1 && e -> position ().posX () < _finalTargetPosition.posX ()) // adjust left?
		e -> setPosition (_finalTargetPosition);
	if (_direction == 2 && e -> position ().posY () > _finalTargetPosition.posY ()) // adjust down?
		e -> setPosition (_finalTargetPosition);
	if (_direction == 3 && e -> position ().posX () > _finalTargetPosition.posX ()) // adjust right?
		e -> setPosition (_finalTargetPosition);
	if (_direction == 4 && e -> position ().posY () < _finalTargetPosition.posY ()) // adjust up?
		e -> setPosition (_finalTargetPosition);
}
