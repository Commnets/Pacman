/** \addtogroup Pacman */
/*@{*/

#ifndef __MAPSBUILDER_HPP__
#define __MAPSBUILDER_HPP__

#include <Arcade/mapbuilder.hpp>

/** The maps builder. */
class MapsBuilderAddsOn : public QGAMES::TMXMapBuilderAddsOn
{
	public:
	MapsBuilderAddsOn (QGAMES::Sprite2DBuilder* sB)
		: QGAMES::TMXMapBuilderAddsOn (sB)
							{ }

	protected:
	virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
		const QGAMES::TileProperties& p = QGAMES::TileProperties ());
	virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
		const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer2D::Orientation o, 
		const QGAMES::LayerProperties& p = QGAMES::LayerProperties ());
	virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
		QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties());
	virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
		QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ());
	virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int tW, int tH,
		const QGAMES::MapProperties& p = QGAMES::MapProperties ());
};

#endif
  
// End of the file
/*@}*/