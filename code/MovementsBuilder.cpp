#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* MovementsBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{ 
	QGAMES::Movement* result = NULL;
	switch (def._id)
	{
		case __MOVPENBLINKY: case __MOVPENPINKY: case __MOVPENINKY: case __MOVPENCLYDE: 
			result = new MovementIntoThePen (def._id, def._variables);
			break;

		case __MOVEXITPENBLINKY: case __MOVEXITPENPINKY: case __MOVEXITPENINKY: case __MOVEXITPENCLYDE:
			result = new MovementExitingThePen (def._id, def._variables);
			break;

		case __MOVMAZEPACMAN:
		case __MOVMAZEBLINKY: case __MOVMAZEPINKY: case __MOVMAZEINKY: case __MOVMAZECLYDE:
			result = new MovementInTheMaze (def._id, def._variables);
			break;

		case __MOVENTERPENBLINKY: case __MOVENTERPENPINKY: case __MOVENTERPENINKY: case __MOVENTERPENCLYDE:
			result = new MovementEnteringThePen (def._id, def._variables);
			break;

		default:
			exit (1);
			break;
	}

	return (result);
}
