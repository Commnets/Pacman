#include "Maps.hpp"
#include "Defs.hpp"

// ---
int MapMaze::numberBalls ()
{
	return (((TileLayerMaze* ) layer (std::string (__MAZELAYER))) -> numberBalls ());
}

// ---
void MapMaze::blink (bool t)
{
	_blink = t;
	((TileLayerMaze* ) layer (std::string (__MAZELAYER))) -> blink (_blink);
}

// ---
void MapMaze::showFruit (int nf)
{
	TileLayerMaze* l = ((TileLayerMaze* ) layer (std::string (__MAZELAYER)));
	QGAMES::Tile* t = l -> tileAt (initialPacmanPosition ());
	t -> setForm (t -> form (), nf); // Sets the fruit...
}

// ---
QGAMES::Layers MapMaze::relevantLayers ()
{
	if (_relevantLayers.empty ())
		_relevantLayers = QGAMES::Map::relevantLayers ();
	return (_relevantLayers);
}

// ---
void MapMaze::initialize ()
{
	_activeLayer = (TileLayerMaze* ) layer (std::string (__MAZELAYER));
	_activeLayer -> initialize ();

	QGAMES::Map::initialize ();
}

// ---
void MapMaze::updatePositions ()
{
	// When there is no more ball active...
	// and the map is clean the situation is notified to the game...
	if (numberBalls () == 0)
		notify (QGAMES::Event (__MAZECLEAN, this));
}

// ---
QGAMES::Position MapMazeA::monsterPenPosition () const
{
	// The middle...
	return (QGAMES::Position (__BD ((10 * _tileWidth) + (_tileWidth / 2)), 
		__BD ((10 * _tileHeight) + _tileHeight / 2), __BD 0));
}

// ---
QGAMES::Position MapMazeA::centerTilePenPosition () const
{
	return (QGAMES::Position (__BD (10 * _tileWidth), __BD (10 * _tileHeight), __BD 0));
}

// ---
QGAMES::Position MapMazeA::initialPacmanPosition () const
{
	return (QGAMES::Position (__BD (10 * _tileWidth), __BD (12 * _tileHeight), __BD 0));
}

// ---
QGAMES::Position MapMazeA::fruitPosition () const
{
	return (QGAMES::Position (__BD (10 * _tileWidth), __BD (12 * _tileHeight), __BD 0));
}

