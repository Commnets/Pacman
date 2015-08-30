#ifndef __MAPS_HPP__
#define __MAPS_HPP__

#include "TileLayers.hpp"
#include <Arcade/map.hpp>

/** The generic class for a map representing the maze. */
class MapMaze : public QGAMES::TiledMap
{
	public:
	MapMaze (int c, const QGAMES::Layers& l, int w, int h, int tW, int tH,
		const QGAMES::MapProperties& p = QGAMES::MapProperties ())
		: QGAMES::TiledMap (c, l, w, h, tW, tH, p),
		_relevantLayers (),
		_blink (false)
							{ }

	/** To know the number of balls still ready to be eaten. */
	int numberBalls ();
	/** To know te center of the monster pen. 
		It must represent the center center of the monster pen. */
	virtual QGAMES::Position monsterPenPosition () const = 0;
	/** To know the til in the center of the pen.
		The tile from the monsters will exit the pen. */
	virtual QGAMES::Position centerTilePenPosition () const = 0;
	/** To know the position where the pacman starts the round. 
		It must represent the upper left corner of the tile where the pacman appears first. */
	virtual QGAMES::Position initialPacmanPosition () const = 0;
	/** To know the place for the fruit. */
	virtual QGAMES::Position fruitPosition () const = 0;
	/** To blink the limists of the maze. */
	void blink (bool t);
	bool isBlink () const
							{ return (_blink); }
	void showFruit (int nf);

	/** The relevant tile layer is the maze layer. */
	virtual QGAMES::Layers relevantLayers ();

	virtual void initialize ();
	/** When all balls have been eaten, an event indicating that situation is thrown. */
	virtual void updatePositions ();

	private:
	// Implementation
	QGAMES::Layers _relevantLayers;
	bool _blink;
};

/** The system is well prepared to have several mazes.
	This is the most classical one. */
class MapMazeA : public MapMaze
{
	public:
	MapMazeA (int c, const QGAMES::Layers& l, int w, int h, int tW, int tH,
		const QGAMES::MapProperties& p = QGAMES::MapProperties ())
		: MapMaze (c, l, w, h, tW, tH, p)
							{ }
	virtual QGAMES::Position monsterPenPosition () const;
	virtual QGAMES::Position centerTilePenPosition () const;
	virtual QGAMES::Position initialPacmanPosition () const;
	virtual QGAMES::Position fruitPosition () const;
};

#endif