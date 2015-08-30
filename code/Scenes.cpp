#include "Scenes.hpp"
#include "Maps.hpp"

// ---
QGAMES::Position ScenePacman::monsterPenPosition () const
{
	return (((MapMaze*) _activeMap) -> monsterPenPosition ());
}

// ---
QGAMES::Position ScenePacman::initialPacmanPosition () const
{
	return (((MapMaze*) _activeMap) -> initialPacmanPosition ());
}

// ---
void ScenePacman::showFruit (int nf)
{
	((MapMaze*) _activeMap) -> showFruit (nf);
}

// ---
void ScenePacman::initialize ()
{
	setMap (__MAPMAZEA);
	QGAMES::Scene::initialize ();
}

// ---
void ScenePacman::processEvent (const QGAMES::Event& e)
{
	QGAMES::Scene::processEvent (e);
	if (e.code () == __MAZECLEAN)
		notify (e);
}
