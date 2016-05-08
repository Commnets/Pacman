/** \addtogroup Pacman */
/*@{*/

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <Arcade/arcadegame.hpp>
#include "GameImplementation.hpp"
#include "Entities.hpp"

/** The pacman game. */
class PacmanGame : public QGAMES::ArcadeGame
{
	public:
	typedef std::multimap <int, std::string, std::less <int>> ScoreList;

	/** Different states the pacman game can be on.
		Normal, the usual situation,
		Frightening, when the pacman eats a super ball, and the monsters run away
		This last situation lasts only few seconds. */
	enum Mode
	{
		_NORMAL = 0,
		_FRIGHTENING = 1,
		_SEMIFRIGHTENING = 2
	};

	PacmanGame ();

	void processEvent (const QGAMES::Event& e);

	virtual void addScoreObjects ();
	virtual void removeScoreObjects ();
	void initScore ();

	int score () const
							{ return (_points -> score ()); }
	void initScore () const
							{ _points ->initialize (); }
	ScoreList scores () const
							{ return (_scores); }
	void addScore (const std::string& p, int s);
	int lives () const
							{ return (_lives -> lives ()); }
	void setLives (int l)
							{ _lives -> setLives (l); }
	int initLives ()
							{ _lives -> initialize (); }
	Mode mode () const
							{ return (_mode); }
	void setMode (Mode m);
	int level () const
							{ return (_level); }
	void setLevel (int l); 

	/** To set whether the game is or not controlled by joystick. */
	void setJoystick (bool j);

	QGAMES::Position monsterPenPosition () const;
	QGAMES::Position initialPacmanPosition () const;

	private:
	virtual QGAMES::FormBuilder* createFormBuilder ();
	virtual QGAMES::ObjectBuilder* createObjectBuilder ();
	virtual QGAMES::EntityBuilder* createEntityBuilder ();
	virtual QGAMES::MovementBuilder* createMovementBuilder ();
	virtual QGAMES::SoundBuilder* createSoundBuilder ();
	virtual QGAMES::Timer* createTimer ();
	virtual QGAMES::Screens createScreens ();
	virtual QGAMES::WorldBuilder* createWorldBuilder ();
	virtual QGAMES::MapBuilder* createMapBuilder ();

	virtual void initialize ();
	virtual void inEveryLoop ();
	virtual void updatePositions ();
	virtual void detectCollisions ();
	virtual void finalize (); 

	void ballEated (QGAMES::Tile* t);
	void superBallEated (QGAMES::Tile* t);
	void mazeClean (QGAMES::Map* m);
	void monsterEaten (PacmanMonster* m);
	void eatenByAMonster (PacmanMonster* m);
	void moveArtistToPortal (int p, PacmanArtist* a);
	void fruitEaten (QGAMES::Tile* t);

	private:
	ScorePoints* _points;
	ScoreLives* _lives;
	Mode _mode;
	int _level;
	ScoreList _scores;

	// Implementation
	Pacman* _pacman;
	Blinky* _blinky;
	Pinky* _pinky;
	Inky* _inky;
	Clyde* _clyde;
	PacmanMonsters _monsters;

	// Several counters & situations...
	int _counterFrigthening;
	bool _gameStopped;
	int _counterStopped;
	int _counterToFruit;
	int _counterFruitScreen;
	int _secondForFruit;
	bool _fruitShown;
};

#endif
  
// End of the file
/*@}*/