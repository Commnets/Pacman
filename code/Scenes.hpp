/** \addtogroup Pacman */
/*@{*/

#ifndef __SCENES_HPP__
#define __SCENES_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** The scene where pacman game happens. */
class ScenePacman : public QGAMES::Scene
{
	public:
	ScenePacman (const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn,
		const QGAMES::SceneProperties& p = QGAMES::SceneProperties (),
		const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
		: QGAMES::Scene (__SCENEPACMAN, m, cn, p, ePL)
							{ }

	QGAMES::Position monsterPenPosition () const;
	QGAMES::Position initialPacmanPosition () const;
	void showFruit (int nf);
	
	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

#endif
  
// End of the file
/*@}*/