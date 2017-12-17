/** \addtogroup Pacman */
/*@{*/

#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__

#include <Arcade/arcadeinclude.hpp>

/** General basic variables of the game. */
class General
{
	public:
	General ();

	public:
	static General _e;

	QGAMES::Vector _left, _right, _up, _down;
	std::vector <std::vector <QGAMES::bdata>> _variables;
};

#endif
  
// End of the file
/*@}*/