/** \addtogroup Pacman */
/*@{*/

#ifndef __MOVEMENTS_HPP__
#define __MOVEMENTS_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class represents any movement of the game. 
	Any movement into the maze should be diver by a class inhereting this one. */
class PacmanEntityMovement : public QGAMES::Movement
{
	public:
	PacmanEntityMovement (int id, std::map <int, double> v);

	bool isAccelrated () const
							{ return (_accelerated > 1); }
	bool isDecelerate () const
							{ return (_accelerated < 1 && _accelerated > 0); }
	bool isStopped () const
							{ return (_accelerated == __BD 0); }
	// Below 1 the function is to reduce the speed..., and equal to 0 is to stop...
	// The quantity might be greater or equal than 0
	void accelerate (int t);
	int speed () const
							{ return (_speed); }
	void setSpeed (int s) ;

	virtual QGAMES::Vector acceleration () const
							{ return (QGAMES::Vector::_cero); }

	virtual void initialize ();

	/** The move method has been programming to call moveOne
		the number of times needed as defined per the speed value. */
	virtual void move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);

	protected:
	// This method must be overloaded in each of the different kinds of movement...
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e) = 0;

	protected:
	int _speed;
	int _changedSpeed;
	int _accelerated;

	// Implementation
	int _counterTSpeed;
	int _counterMSpeed[2];
};

/** It represents the monster's movement into the pen. */
class MovementIntoThePen : public PacmanEntityMovement
{
	public:
	MovementIntoThePen (int id, std::map <int, double> v);

	virtual Movement* clone () const;

	virtual QGAMES::Vector direction () const;
	virtual QGAMES::Vector acceleration () const
							{ return (QGAMES::Vector::_cero); }

	virtual void initialize ();

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);

	private:
	int _direction;
	int _counter;

	// Implementation
	int _distance;
	bool _firstMove;
	int _currentPosition;
	QGAMES::Position _initialPosition;
};

/** It represents the movement of the monsters while exit the pen. */
class MovementExitingThePen : public PacmanEntityMovement
{
	public:
	MovementExitingThePen (int id, std::map <int, double> v);

	virtual Movement* clone () const;

	virtual QGAMES::Vector direction () const;

	virtual void initialize ();

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);

	private:
	int _direction;

	// Implementation
	bool _firstMove;
	QGAMES::Position _initialPosition;
	QGAMES::Position _firstTargetPosition;
	QGAMES::Position _finalTargetPosition;
};

/** It represents the movement of an pacman artist in the maze.
	Taking so into account its limits. */
class MovementInTheMaze : public PacmanEntityMovement
{
	public:
	MovementInTheMaze (int id, std::map <int, double> v);

	virtual Movement* clone () const;
	virtual QGAMES::Vector direction () const
							{ return (QGAMES::Vector::_cero); }

	bool isMoving () const
							{ return (_moving); }

	virtual void initialize ();

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);
	void changeMovementTo (const QGAMES::Vector& d, QGAMES::Entity* e);

	private:
	// Implementation
	bool _moving;
	QGAMES::Vector _directionMovement;
	QGAMES::Vector _potentialMovement;
	int _numberSteps;
};

/** It represents the movement of the monsters while enter the pen. */
class MovementEnteringThePen : public PacmanEntityMovement
{
	public:
	MovementEnteringThePen (int id, std::map <int, double> v);

	virtual Movement* clone () const;

	virtual QGAMES::Vector direction () const;

	virtual void initialize ();

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);

	private:
	int _direction;

	// Implementation
	bool _firstMove;
	QGAMES::Position _initialPosition;
	QGAMES::Position _firstTargetPosition;
	QGAMES::Position _finalTargetPosition;
};

#endif
  
// End of the file
/*@}*/
