/** \addtogroup Pacman */
/*@{*/

#ifndef __TILELAYERS_HPP__
#define __TILELAYERS_HPP__

#include <Arcade/layer.hpp>
#include "Defs.hpp"

/** A layer representing the maze, and the thing in it ready to be eaten. */
class TileLayerMaze : public QGAMES::TileLayer2D
{
	public:
	TileLayerMaze (int id, const std::string& n, const QGAMES::Tiles& t, 
		QGAMES::Map* m, QGAMES::TileLayer2D::Orientation o,
		const QGAMES::LayerProperties p = QGAMES::LayerProperties ())
		: QGAMES::TileLayer2D (id, n, t, m, o, p),
		  _blink (false)
							{ }

	/** To know the number of ball still ready to be eaten. */
	int numberBalls ();
	/** To "blink" the limits of the maze.
		This is one of the effects happenning at the end or a round, when the
		maze has been totally clean. */
	void blink (bool t);
	bool isBlink () const
							{ return (_blink); }
	
	/** To init the maze, it is: to set up back the balls to be eaten. */
	virtual void initialize ();

	private:
	// Implementation...
	bool _blink;
};

/** A layer representing the background of the game. */
class TileLayerBackground : public QGAMES::TileLayer2D
{
	public:
	TileLayerBackground (int id, const std::string& n, const QGAMES::Tiles& t, 
		QGAMES::Map* m, QGAMES::TileLayer2D::Orientation o,
		const QGAMES::LayerProperties p = QGAMES::LayerProperties ())
		: QGAMES::TileLayer2D (id, n, t, m, o, p),
		  _pos (0),
		  _direction (1)
							{ _position = QGAMES::Position::_cero; }

	private:
	int _count;
	int _pos;
	int _direction;
};

#endif
  
// End of the file
/*@}*/