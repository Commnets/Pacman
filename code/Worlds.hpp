/** \addtogroup Pacman */
/*@{*/

#ifndef __WORLDS_HPP__
#define __WORLDS_HPP__

#include <Arcade/world.hpp>
#include "Defs.hpp"

/** The world where pacaman game happens. */
class WorldPacman : public QGAMES::World
{
	public:
	WorldPacman (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p);

	QGAMES::Position monsterPenPosition () const;
	QGAMES::Position initialPacmanPosition () const;
	void showFruit (int nf);

	virtual void initialize ();

	virtual void processEvent (const QGAMES::Event& e);

	private:
	// Define a set of important buoys..
	/** Define a buoy to finihses the maze. */
	class ToCleanMazeBuoy : public QGAMES::Buoy
	{
		public:
		ToCleanMazeBuoy ()
			: QGAMES::Buoy (__MAZECLEANBOUYID, __BD 0)
							{ }

		virtual void* treatFor (QGAMES::Element* e);
	};
};

#endif
  
// End of the file
/*@}*/
