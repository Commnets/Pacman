#include "EntitiesBuilder.hpp"
#include "Entities.hpp"
#include "Defs.hpp"

// ---
QGAMES::Entity* EntitiesBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	QGAMES::Entity* result = NULL;
	switch (def._id)
	{
		case __ENTITYPACMAN :
			result = new Pacman ();
			break;

		case __ENTITYBLINKY :
			result = new Blinky ();
			break;

		case __ENTITYPINKY :
			result = new Pinky ();
			break;

		case __ENTITYINKY :
			result = new Inky ();
			break;

		case __ENTITYCLYDE :
			result = new Clyde ();
			break;

		default:
			exit (1);
			break;
	}

	return (result);
}
