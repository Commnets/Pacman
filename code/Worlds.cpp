#include "Worlds.hpp"
#include "Scenes.hpp"
#include <Arcade/arcadegame.hpp>
#include <Arcade/artist.hpp>

// ---
QGAMES::Position WorldPacman::monsterPenPosition () const
{
	return (((ScenePacman*) _activeScene) -> monsterPenPosition ());
}

// ---
QGAMES::Position WorldPacman::initialPacmanPosition () const
{
	return (((ScenePacman*) _activeScene) -> initialPacmanPosition ());
}

// ---
void WorldPacman::showFruit (int nf)
{
	((ScenePacman*) _activeScene) -> showFruit (nf);
}

// ---
void WorldPacman::initialize ()
{
	// The initial scene...
	setScene (__SCENEPACMAN);
	
	// Doing as follows, the entity is observed by the game...
	QGAMES::Artist* e = __AT (__AGM QGAMES::ArcadeGame::game ()) -> artist (__ENTITYPACMAN);
	QGAMES::Artist* b = __AT (__AGM QGAMES::ArcadeGame::game ()) -> artist (__ENTITYBLINKY);
	QGAMES::Artist* p = __AT (__AGM QGAMES::ArcadeGame::game ()) -> artist (__ENTITYPINKY);
	QGAMES::Artist* i = __AT (__AGM QGAMES::ArcadeGame::game ()) -> artist (__ENTITYINKY);
	QGAMES::Artist* c = __AT (__AGM QGAMES::ArcadeGame::game ()) -> artist (__ENTITYCLYDE);
	
	// The entity is going to be part of the scene...
	// They are not added if the character is already part of the scene...
	_activeScene -> addCharacter (e); 
	_activeScene -> addCharacter (b);
	_activeScene -> addCharacter (p);
	_activeScene -> addCharacter (i);
	_activeScene -> addCharacter (c);
	
	// The world is then initialized, and all scenes, world, and entities with it...	
	QGAMES::World::initialize ();

	// The map where entity moves in the active map...
	// If the map is already part of the scene, nothing happens...
	QGAMES::Map* m = _activeScene -> activeMap ();
	e -> setMap (m);
	b -> setMap (m);
	p -> setMap (m);
	i -> setMap (m);
	c -> setMap (m);
}

// ---
void WorldPacman::processEvent (const QGAMES::Event& e)
{
	QGAMES::World::processEvent (e);
	if (e.code () == __MAZECLEAN)
		notify (e);
}
