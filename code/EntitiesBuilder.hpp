/** \addtogroup Pacman */
/*@{*/

#ifndef __ENTITIESBUILDER_HPP__
#define __ENTITIESBUILDER_HPP__

#include <Arcade/arcadeinclude.hpp>

/** The entities builder. */
class EntitiesBuilder : public QGAMES::EntityBuilder
{
	public:
	EntitiesBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
		: QGAMES::EntityBuilder (fDef, fB, mB)
							{ }
	
	private:
	QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def);
};

#endif
  
// End of the file
/*@}*/