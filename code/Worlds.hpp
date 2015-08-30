#ifndef __WORLDS_HPP__
#define __WORLDS_HPP__

#include <Arcade/world.hpp>
#include "Defs.hpp"

/** The world where pacaman game happens. */
class WorldPacman : public QGAMES::World
{
	public:
	WorldPacman (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
		: QGAMES::World (__WORLDPACMAN, s, p)
							{ }

	QGAMES::Position monsterPenPosition () const;
	QGAMES::Position initialPacmanPosition () const;
	void showFruit (int nf);

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);
};

#endif
