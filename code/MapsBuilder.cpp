#include "MapsBuilder.hpp"
#include "Maps.hpp"
#include "Tiles.hpp"
#include "TileLayers.hpp"
#include "Defs.hpp"

// ---
QGAMES::Tile* MapsBuilderAddsOn::createTile (int id, QGAMES::Form* form, int nf, 
		const QGAMES::TileProperties& p)
{
	QGAMES::Tile* result = NULL;
	
	// Tiles from the maze form...
	if (form -> id () == __MAZEFORM)
	{
		if (nf == __BALLFRAME || nf == __SUPERBALLFRAME)
			result = new TileBall (id, form, nf, p);
		else
		if (nf == __BACKGROUNDFRAME)
			result = new TileBackground (id, form, nf, p);
		else
		if (nf == __PORTALGATE1 || nf == __PORTALGATE2)
			result = new TileGate (id, form, nf, p);
		else
			result = new TileMazeBorder (id, form, nf, p);
	}

	// Tiles from the fruit form...
	if (form -> id () == __FRUITSFORM)
		result = new TileFruit (id, form, nf, p);

	return (result);
}

// ---
QGAMES::TileLayer* MapsBuilderAddsOn::createTileLayer (int id, const std::string& n, 
		const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer2D::Orientation o,
		const QGAMES::LayerProperties& p)
{
	QGAMES::TileLayer* result = NULL;

	if (n == std::string (__BACKGROUNDLAYER))
		result = new TileLayerBackground (id, n, t, m, o, p);
	else
		result = new TileLayerMaze (id, n, t, m, o, p);

	return (result);
}

// ---
QGAMES::ObjectLayer* MapsBuilderAddsOn::createObjectLayer (int id, const std::string& n, const QGAMES::Objects& o, 
		QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	QGAMES::ObjectLayer* result = NULL;
	return (result);
}

// ---
QGAMES::ImageLayer* MapsBuilderAddsOn::createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
		QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	QGAMES::ImageLayer* result = NULL;
	return (result);
}

// ---
QGAMES::Map* MapsBuilderAddsOn::createMapObject (int id, const QGAMES::Layers& l, int w, int h, int tW, int tH,
		const QGAMES::MapProperties& p)
{
	QGAMES::Map* result = NULL;
	switch (id)
	{
		case __MAPMAZEA:
			result = new MapMazeA (id, l, w, h, tW, tH, p);
			break;

		default:
			exit (1);
			break;
	};

	return (result);
}
