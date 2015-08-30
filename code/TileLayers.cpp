#include "TileLayers.hpp"

// ---
int TileLayerMaze::numberBalls ()
{
	int result = 0;
	for (QGAMES::Tiles::const_iterator i = _tiles.begin (); i != _tiles.end (); i++)
		if ((*i) -> type () == __TILEBALL && 
				((*i) -> numberFrame () == __BALLFRAME || (*i) -> numberFrame () == __SUPERBALLFRAME))
			result++;
	return (result);
}

// ---
void TileLayerMaze::blink (bool t)
{
	// Hightlight the maze...or not...
	int m = (t ? (_blink ? 0 : 1) : (_blink ? -1 : 0));
	_blink = t;
	for (QGAMES::Tiles::const_iterator i = _tiles.begin (); i != _tiles.end (); i++)
		if ((*i) -> type () == __TILEMAZE)
			(*i) -> setForm ((*i) -> form (), (*i) -> numberFrame () + m * __BLINKADD);
}

// ---
void TileLayerMaze::initialize ()
{
	// All the ball are set up to their initial states...
	blink (false);
	for (QGAMES::Tiles::const_iterator i = _tiles.begin (); i != _tiles.end (); i++)
	{
		if ((*i) -> type () == __TILEBALL)
		{
			if ((*i) -> numberFrame () == __BALLEATEDFRAME)
				(*i) -> setForm ((*i) -> form (), __BALLFRAME);
			else
			if ((*i) -> numberFrame () == __SUPERBALLEATEDFRAME)
				(*i) -> setForm ((*i) -> form (), __SUPERBALLFRAME);
		}
	}
}
