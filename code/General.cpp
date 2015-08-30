#include "General.hpp"
#include "Defs.hpp"

General General::_e = General ();

// ---
General::General ()
	: _left (__BD -1, __BD 0, __BD 0),
	  _right (__BD 1, __BD 0, __BD 0),
	  _up (__BD 0, __BD -1, __BD 0),
	  _down (__BD 0, __BD 1, __BD 0)
{
	// Set up the number of levelsss...
	// Let's figure out the max levcel is always greater than 21!
	_variables.resize (__MAXNUMBEROFLEVELS);
	for (int i = 0; i < __MAXNUMBEROFLEVELS; i++)
		_variables [i].resize (15);

	QGAMES::bdata data [21][15] =
	{ 
		{ __BD 1,	__BD 100,	__BD 0.8,	__BD 0.71,	__BD 0.75,	__BD 0.40,	__BD 20,	__BD 0.8,	__BD 10,	__BD 0.85,	__BD 0.9,	__BD 0.79,	__BD 0.5,	__BD 6,	__BD 5 },
		{ __BD 2,	__BD 300,	__BD 0.9,	__BD 0.79,	__BD 0.85,	__BD 0.45,	__BD 30,	__BD 0.9,	__BD 15,	__BD 0.95,	__BD 0.95,	__BD 0.83,	__BD 0.55,	__BD 5,	__BD 5 },
		{ __BD 3,	__BD 500,	__BD 0.9,	__BD 0.79,	__BD 0.85,	__BD 0.45,	__BD 40,	__BD 0.9,	__BD 20,	__BD 0.95,	__BD 0.95,	__BD 0.83,	__BD 0.55,	__BD 4,	__BD 5 },
		{ __BD 4,	__BD 500,	__BD 0.9,	__BD 0.79,	__BD 0.85,	__BD 0.45,	__BD 40,	__BD 0.9,	__BD 20,	__BD 0.95,	__BD 0.95,	__BD 0.83,	__BD 0.55,	__BD 3,	__BD 5 },
		{ __BD 5,	__BD 700,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 40,	__BD 1,		__BD 20,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 2,	__BD 5 },
		{ __BD 6,	__BD 700,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 50,	__BD 1,		__BD 25,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 5,	__BD 5 },
		{ __BD 7,	__BD 1000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 50,	__BD 1,		__BD 25,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 2,	__BD 5 },
		{ __BD 8,	__BD 1000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 50,	__BD 1,		__BD 25,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 2,	__BD 5 },
		{ __BD 10,	__BD 2000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 60,	__BD 1,		__BD 30,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 11,	__BD 2000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 60,	__BD 1,		__BD 30,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 5,	__BD 5 },
		{ __BD 1,	__BD 3000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 60,	__BD 1,		__BD 30,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 2,	__BD 5 },
		{ __BD 2,	__BD 3000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 80,	__BD 1,		__BD 40,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 3,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 80,	__BD 1,		__BD 40,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 4,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 80,	__BD 1,		__BD 40,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 3,	__BD 5 },
		{ __BD 5,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 100,	__BD 1,		__BD 50,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 6,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 100,	__BD 1,		__BD 50,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 7,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 100,	__BD 1,		__BD 50,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 8,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 100,	__BD 1,		__BD 50,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 10,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 120,	__BD 1,		__BD 60,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 11,	__BD 5000,	__BD 1,		__BD 0.87,	__BD 0.95,	__BD 0.5,	__BD 120,	__BD 1,		__BD 60,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 },
		{ __BD 1,	__BD 5000,	__BD 0.9,	__BD 0.79,	__BD 0.95,	__BD 0.5,	__BD 120,	__BD 1,		__BD 60,	__BD 1.05,	__BD 1,		__BD 0.87,	__BD 0.6,	__BD 1,	__BD 3 }
	};


	// Time to init the variables
	// The table has been published as part of the pacmand dossier
	// at: http://home.comcast.net/~jpittman2/pacman/pacmandossier.html
	// published by: Jamey Pittman
	// The name of the variables can be find Defs.hpp
	for (int i = 0; i < ((__MAXNUMBEROFLEVELS >= 21) ? 21 : __MAXNUMBEROFLEVELS); i++)
		_variables [i] = std::vector <QGAMES::bdata> (data [i], data [i] + 15);
	for (int i = 22; i < __MAXNUMBEROFLEVELS; i++)
		_variables [i] = std::vector <QGAMES::bdata> (data [21], data [21] + 15);
	// From level 21 of the levels has the same behaviour...
}
