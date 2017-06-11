/** \addtogroup Pacman */
/*@{*/

#ifndef __ENTITIES_HPP__
#define __ENTITIES_HPP__

#include <Common/entity.hpp>
#include <Arcade/artist.hpp>
#include "Defs.hpp"

#include <vector>
#include <list>

class Pacman;
class Blinky;
class Pinky;
class Inky;
class Clyde;
class PacmanMonster;
typedef std::list <PacmanMonster*> PacmanMonsters;

/** Any pacman element. */
class PacmanArtist : public QGAMES::ArtistInATiledMap
{
	public:
	enum Who 
	{
		_PACMAN = 0,
		_BLINKY = 1,
		_PINKY = 2,
		_INKY = 3,
		_CLYDE = 4
	};

	/** The can be in two different states:
		Alive or died. The implication in each of artists is different. */
	enum State
	{
		_ALIVE = 0,
		_DIE = 1
	};

	PacmanArtist (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Who whoAreYou () const = 0;
	/** How many portions of tiles from the center is the artist located at the beginning? */
	virtual QGAMES::bdata tilesAtBeginning () const
								{ return (__BD 0); } // none by default...
	State state () const
							{ return (_state); }
	virtual void setState (State s);
	virtual void setSpeed (int s);
	virtual bool continueMoving () 
							{ return (true); }
	virtual QGAMES::Vector nextMovementAtLimit (const QGAMES::Vector& d)
							{ return (d); }

	virtual void updatePositions ();

	protected:
	enum MovingTo 
	{
		_DUP = 0,
		_DLEFT = 1,
		_DDOWN = 2,
		_DRIGHT = 3
	}; // Just to help in the aspect selection...

	virtual void confirmFinalAspect (MovingTo d);
	void setMovementToChange (int m)
							{ _changeMovement = true; _movementToChange = m; }

	std::vector <std::vector <int>> _aspects;
	State _state;
	
	// Implementation
	int _counter;
	bool _changeMovement;
	int _movementToChange;
	bool _initialize;
//	QGAMES::Position _lastPosition;
};

/** Any monster. 
	They are also pacman artists. */
class PacmanMonster : public PacmanArtist
{
	public:
	enum Mode
	{
		_WAITING = 0,
		_SCATTER = 1,
		_CHASE = 2
	};

	PacmanMonster (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	void setCollegues (const PacmanMonsters& m)
							{ _collegues = m; }
	virtual void setState (State s);
	Mode mode () const
							{ return (_mode); }
	void setMode (Mode m); 
	/** Which is the position to point when the monster is not targeting pacman? 
		The answer deppends on the monster. */
	virtual QGAMES::Position scatterPosition () const = 0;
	virtual void setMyTarget (Pacman* p);
	/** Which is the position to point when the monster is targeting pacman?.
		The answer again dppends on the monster. */
	virtual QGAMES::Position targetPosition () const = 0;
	/** Points when the monster is eaten by pacman. */
	virtual int pointsWhenEaten () const = 0;
	virtual bool continueMoving ();
	virtual QGAMES::Vector nextMovementAtLimit (const QGAMES::Vector& d);

	virtual void initialize ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	virtual void whenCollisionWith (const QGAMES::Tiles& t, 
		QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l = NULL);

	protected:
	QGAMES::Vector nextMove ();
	/** There is a basic way to define the aspect of a monster,
		but in the case of the monster this way can be different depending on the state 
		of the monster itself (died or alive...) */
	virtual void confirmFinalAspect (MovingTo d);
	/** The monster is always "deciding" between scatter pacman or target it... 
		The time in each situation depends i.e on the game level. */
	void setChaseOrScatter ();

	protected:
	Pacman* _pacman;
	Mode _mode;
	PacmanMonsters _collegues;

	// Implementation
	int _counterScatter;
	int _counterChase;
	int _counterCycle;
	int _counterPanic;
	QGAMES::bdata _lastSpeed;
	QGAMES::Position _whereToGo;
	std::vector <QGAMES::Vector> _orientations;
	std::vector <int> _sizes;
};

/** Finally: Pacman. */
class Pacman : public PacmanArtist
{
	public:
	Pacman (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: PacmanArtist (__ENTITYPACMAN, f, d)
							{ }

	virtual Entity* clone ()
							{ return (new Pacman (_forms, _data)); }
	virtual Who whoAreYou () const
							{ return (_PACMAN); }

	virtual void initialize ();

	virtual void whenCollisionWith (const QGAMES::Tiles& t, 
		QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l = NULL);
};

/** Blinky: The red monster. */
class Blinky : public PacmanMonster
{
	public:
	Blinky (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: PacmanMonster (__ENTITYBLINKY, f, d)
							{ }

	virtual Entity* clone ()
							{ return (new Blinky (_forms, _data)); }
	virtual Who whoAreYou () const
							{ return (_BLINKY); }
	virtual QGAMES::Position scatterPosition () const
							{ return (QGAMES::Position (__BD 1000, __BD -1000, __BD 0)); }
	virtual QGAMES::Position targetPosition () const;
	virtual QGAMES::bdata tilesAtBeginning () const
							{ return (__BD -1.5); }
	virtual int pointsWhenEaten () const
							{ return (__BLINKYEATENPOINTS); }

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

/** Pinky: The pink monster. */
class Pinky : public PacmanMonster
{
	public:
	Pinky (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: PacmanMonster (__ENTITYPINKY, f, d)
							{ }

	virtual Entity* clone ()
							{ return (new Pinky (_forms, _data)); }
	virtual Who whoAreYou () const
							{ return (_PINKY); }
	virtual QGAMES::Position scatterPosition () const
							{ return (QGAMES::Position (__BD 0, __BD -1000, __BD 0)); }
	virtual QGAMES::Position targetPosition () const;
	virtual QGAMES::bdata tilesAtBeginning () const
							{ return (__BD -0.5); }
	virtual int pointsWhenEaten () const
							{ return (__PINKYEATENPOINTS); }

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

/** Inky: The orange monster. */
class Inky : public PacmanMonster
{
	public:
	Inky (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: PacmanMonster (__ENTITYINKY, f, d)
							{ }

	virtual Entity* clone ()
							{ return (new Inky (_forms, _data)); }
	virtual Who whoAreYou () const
							{ return (_INKY); }
	virtual QGAMES::Position scatterPosition () const
							{ return (QGAMES::Position (__BD 1000, __BD 1000, __BD 0)); }
	virtual QGAMES::Position targetPosition () const;
	virtual QGAMES::bdata tilesAtBeginning () const
							{ return (__BD 0.5); }
	virtual int pointsWhenEaten () const
							{ return (__INKYEATENPOINTS); }

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

/** And clyde: The cyan monster. */ 
class Clyde : public PacmanMonster
{
	public:
	Clyde (const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: PacmanMonster (__ENTITYCLYDE, f, d)
							{ }

	virtual Entity* clone ()
							{ return (new Clyde (_forms, _data)); }
	virtual Who whoAreYou () const
							{ return (_CLYDE); }
	virtual QGAMES::Position scatterPosition () const
							{ return (QGAMES::Position (__BD 0, __BD 1000, __BD 0)); }
	virtual QGAMES::Position targetPosition () const;
	virtual QGAMES::bdata tilesAtBeginning () const
							{ return (__BD 1.5); }
	virtual int pointsWhenEaten () const
							{ return (__CLYDEEATENPOINTS); }

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

class ScoreDigit : public QGAMES::ScoreObject
{
	public:
	ScoreDigit (int cId)
		: ScoreObject (cId)
							{ }

	virtual Entity* clone ();
};

class ScorePoints : public QGAMES::CompositeEntity
{
	public:
	ScorePoints ();
	~ScorePoints ();

	virtual Entity* clone ();

	int score () const
							{ return (_score); }

	virtual void add (int n);

	virtual void initialize ();
	virtual void updatePositions ();

	private:
	int _score;

	// Implementation
	std::list <int> _pendingToAdd;
	bool _adding;
	int _valueBeingAdded;
	int _counterAdded;
};

class ScoreLive : public QGAMES::ScoreObject
{
	public:
	ScoreLive (int cId)
		: ScoreObject (cId)
							{ }

	virtual Entity* clone ();
};

class ScoreLives : public QGAMES::CompositeEntity
{
	public:
	ScoreLives ();
	~ScoreLives ();

	virtual Entity* clone ();

	int lives () const
							{ return (_lives); }
	void setLives (int l);

	virtual void initialize ();

	private:
	int _lives;
	int _maxLives;
};

#endif
  
// End of the file
/*@}*/