/** \addtogroup Pacman */
/*@{*/

#ifndef __TILES_HPP__
#define __TILES_HPP__

#include <Arcade/tile.hpp>
#include "Tiles.hpp"
#include "Defs.hpp"

/** Tile defining the background of the game. */
class TileBackground : public QGAMES::Tile
{
	public:
	TileBackground (int id, QGAMES::Form* f, int nf,
		const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __TILEBACKGROUND, p)
							{ }
};

/** Tile defining the limits of the maze. */
class TileMazeBorder : public QGAMES::Tile
{
	public:
	TileMazeBorder (int id, QGAMES::Form* f, int nf,
		const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __TILEMAZE, p)
							{ }
};

/** Tile defining something to eat in the game. */
class TileBall : public QGAMES::Tile
{
	public:
	TileBall (int id, QGAMES::Form* f, int nf,  
		const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __TILEBALL, p)
							{ }
};

/** Tile defining part of the portal connecting two. */
class TileGate : public QGAMES::Tile
{
	public:
	TileGate (int id, QGAMES::Form* f, int nf,  
		const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __TILEGATE, p)
							{ }
};

/** Tile defining the fruit to eat in the game. */
class TileFruit : public QGAMES::Tile
{
	public:
	TileFruit (int id, QGAMES::Form* f, int nf,  
		const QGAMES::TileProperties& p = QGAMES::TileProperties ())
			: QGAMES::Tile (id, f, nf, __TILEFRUIT, p)
							{ }
};

#endif
  
// End of the file
/*@}*/
