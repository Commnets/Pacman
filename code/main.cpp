#include "stdafx.h"
#include "Game.hpp"

int _tmain (int argc, _TCHAR* argv[])
{
	PacmanGame game;
	game.setLinkDrawToFrameRate (true); 
	// The normal thing is to draw at the same pace than the frame rate...
	game.setPreLoad (true); 
	// The normal thing is to preload all componenets...
	game.exec ();
	return (0);
}