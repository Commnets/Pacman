/** \addtogroup Pacman */
/*@{*/

#ifndef __DEFS__
#define __DEFS__

// Main parameters of the screen game...
#define __GAMESNAME__					"ICF Pacman Game"
#define __SCREENXPOS__					400
#define __SCREENYPOS__					50
#define __SCREENWIDTH__					608
#define __SCREENHEIGHT__				704
#define __MAXNUMBEROFLIVES__			3
#define __PLAYSTOROUND__				3
#define __REDCOLOR						QGAMES::Color (255,0,0,255)
#define __GRAYCOLOR						QGAMES::Color (100,100,100,255)
#define __MAXSCORES						10

// The game states:
#define __GAMESTATELOADING				0
#define __GAMESTATELOADINGNAME			"0"
#define __GAMESTATEINITIAL				1
#define __GAMESTATEINITIALNAME			"1"
#define __GAMESTATEPRELUDE				2
#define __GAMESTATEPRELUDENAME			"2"
#define __GAMESTATEPRELUDESHORT			20
#define __GAMESTATEPRELUDESHORTNAME		"20"
#define __GAMESTATEPLAYING				3
#define __GAMESTATEPLAYINGNAME			"3"
#define __GAMESTATEMAZECLEAN			4
#define __GAMESTATEMAZECLEANNAME		"4"
#define __GAMESTATEPACMANDIES			5
#define __GAMESTATEPACMANDIESNAME		"5"
#define __GAMESTATEROUNDEND				6
#define __GAMESTATEROUNDENDNAME			"6"
#define __GAMESTATEEND					7
#define __GAMESTATEENDNAME				"7"
#define __GAMESTATEINTROLETTERS			8
#define __GAMESTATEINTROLETTERSNAME		"8"
#define __GAMESTATESEESCORES			9
#define __GAMESTATESEESCORESNAME		"9"
#define __NUMBEROPTIONS					5
#define __MAXNUMBEROFDIFFICULTYLEVELS	3

// The worlds of the game:
#define __WORLDPACMAN					1
// The scenes of the game
#define __SCENEPACMAN					11

// The maps of the scenes:
#define __MAPMAZEA						111
// The name of the layers:
#define __BACKGROUNDLAYER				"Background"
#define __MAZELAYER						"Maze"
// The types of tiles:
#define __TILEBACKGROUND				1
#define __TILEMAZE						2
#define __TILEBALL						3
#define __TILEGATE						4
#define __TILEFRUIT						5

// The entities are of the game:
// The pacman entity, and the rest...
#define __ENTITYPACMAN					1
#define __ENTITYBLINKY					2
#define __ENTITYPINKY					3
#define __ENTITYINKY					4
#define __ENTITYCLYDE					5
#define __ENTITYPOINTS					6
#define __ENTITYLIVES					7

// The forms...
// The main form with eveything...
#define __BACKGROUNDINITIALFORM			10
#define __COMMTYLOGOFORM				11
#define __PACMANICOFORM					12
#define __SPRITESFORM					1
#define __NUMBERSFORM					2
#define __GIANTPACMANFORM__				3
#define __READYFORM						104
#define __LETTERSFORM					105
#define __MAZEFORM						100000
#define __FRUITSFORM					100001

// The main number of frames...
#define __BACKGROUNDFRAME				11 // It referes to the form with the maze parts!
#define __BALLFRAME						29
#define __SUPERBALLFRAME				27
#define __FRUITFRAMEEMPTY				9
#define __BALLEATEDFRAME				30
#define __SUPERBALLEATEDFRAME			40
#define __PORTALGATE1					31
#define __PORTALGATE2					41
#define __CERO							0
#define __ONE							1
#define __TWO							2
#define __THREE							3
#define __FOUR							4
#define __FIVE							5
#define __SIX							6
#define __SEVEN							7
#define __EIGHT							8
#define __NINE							9
#define __BLINKADD						50

// The number aspects of the pacman, and the monsters...
// Pacman
#define __PACMANUPMINFRAME				52
#define __PACMANUPMAXFRAME				53
#define __PACMANLEFTMINFRAME			38
#define __PACMANLEFTMAXFRAME			39
#define __PACMANDOWNMINFRAME			24
#define __PACMANDOWNMAXFRAME			25
#define __PACMANRIGHTMINFRAME			10
#define __PACMANRIGHTMAXFRAME			11
#define __GIANTPACMANRIGHTMINFRAME		0
#define __GIANTPACMANRIGHTMAXFRAME		1
// Blinky
#define __BLINKYUPMINFRAME				42
#define __BLINKYUPMAXFRAME				43
#define __BLINKYLEFTMINFRAME			28
#define __BLINKYLEFTMAXFRAME			29
#define __BLINKYDOWNMINFRAME			14
#define __BLINKYDOWNMAXFRAME			15
#define __BLINKYRIGHTMINFRAME			0
#define __BLINKYRIGHTMAXFRAME			1
// Pinky
#define __PINKYUPMINFRAME				44
#define __PINKYUPMAXFRAME				45
#define __PINKYLEFTMINFRAME				30
#define __PINKYLEFTMAXFRAME				31
#define __PINKYDOWNMINFRAME				16
#define __PINKYDOWNMAXFRAME				17
#define __PINKYRIGHTMINFRAME			2
#define __PINKYRIGHTMAXFRAME			3
// Inky
#define __INKYUPMINFRAME				46
#define __INKYUPMAXFRAME				47
#define __INKYLEFTMINFRAME				32
#define __INKYLEFTMAXFRAME				33
#define __INKYDOWNMINFRAME				18
#define __INKYDOWNMAXFRAME				19
#define __INKYRIGHTMINFRAME				4
#define __INKYRIGHTMAXFRAME				5
// Clyde
#define __CLYDEUPMINFRAME				48
#define __CLYDEUPMAXFRAME				49
#define __CLYDELEFTMINFRAME				34
#define __CLYDELEFTMAXFRAME				35
#define __CLYDEDOWNMINFRAME				20
#define __CLYDEDOWNMAXFRAME				21
#define __CLYDERIGHTMINFRAME			6
#define __CLYDERIGHTMAXFRAME			7
// Scared
#define __MONSTERSCAREDMINFRAME			12
#define __MONSTERSCAREDMAXFRAME			13
// Semiscared
#define __MONSTERSEMISCAREDMINFRAME		26
#define __MONSTERSEMISCAREDMAXFRAME		27
// Died
#define __MONSTERDIEMINFRAME			40
#define __MONSTERDIEMAXFRAME			41
// None
#define __NONEFRAME						54

// The movements of the game:
// The pacman's movement and the rest...
#define __MOVMAZEPACMAN					1
#define __MOVPENBLINKY					20
#define __MOVEXITPENBLINKY				21
#define __MOVMAZEBLINKY					22
#define __MOVENTERPENBLINKY				23
#define __MOVPENPINKY					30
#define __MOVEXITPENPINKY				31
#define __MOVMAZEPINKY					32
#define __MOVENTERPENPINKY				33
#define __MOVPENINKY					40
#define __MOVEXITPENINKY				41
#define __MOVMAZEINKY					42
#define __MOVENTERPENINKY				43
#define __MOVPENCLYDE					50
#define __MOVEXITPENCLYDE				51
#define __MOVMAZECLYDE					52
#define __MOVENTERPENCLYDE				53

// The sounds of the game:
// Pacman sounds
#define __SOUNDBEGINNING				1
#define __SOUNDCHOMP					2
#define __SOUNDDEATH					3
#define __SOUNDEATFRUIT					4
#define __SOUNDEATGHOST					5
#define __SOUNDEXTRAPAC					6
#define __SOUNDINTERMISSION				7
#define __SOUNDSIREN					8

// Points when the monsters are eaten...
#define __BLINKYEATENPOINTS				1600
#define __PINKYEATENPOINTS				1200
#define __INKYEATENPOINTS				1000
#define __CLYDEEATENPOINTS				800

// The keys...
#define __PACMANLEFT					QGAMES::KeyCode::QGAMES_A
#define __PACMANRIGHT					QGAMES::KeyCode::QGAMES_S
#define __PACMANUP						QGAMES::KeyCode::QGAMES_P
#define __PACMANDOWN					QGAMES::KeyCode::QGAMES_L

// The events
// When a normal ball has been eaten...
#define __BALLEATED						900
// When a super ball ha sbeen eated (the ghosts become blue)
#define __SUPERBALLEATED				901
// When a maze has been clean...
#define __MAZECLEAN						902
// When the total loops in the pen have been completed...
#define __INTHEPEN_MOVCOMPLETED			903
// When a loop in the pen has been completed
#define __INTHEPEN_MOVCYCLECOMPLETED	904
// When the movement to exit the pen has been completed
#define __EXITINGPEN_COMPLETED			905
// When the movement to enter the pen has been completed
#define __ENTERINGPEN_COMPLETED			906
// When the monster has died and its eyes has achieved the center of the tile...
#define __EYESINFRONTOFPEN				907
// Enter the portal 1
#define __ENTERPORTAL1					908
// Enter the portal 2
#define __ENTERPORTAL2					909
// The fruit has been eaten
#define __FRUITEATEN					910

// The buoys...
#define __MAZECLEANBOUYID				123

// Points when eating something...
#define __POINTSBALLEATED				5
#define __POINTSSUPERBALLEATED			10

// Name of the variables of the game...
#define __MAXNUMBEROFLEVELS				256
#define __VARBONUSSYMBOL				0
#define __VARBONUSPOINTS				1
#define __VARPACMANSPEED				2
#define __VARPACMANDOTSSPEED			3
#define __VARGHOSTSPEED					4
#define __VARGHOSTTUNNELSPEED			5
#define __VARELROY1DOTSLEFT				6
#define __VARELROY1SPEED				7
#define __VARELROY2DOTSLEFT				8
#define __VARELROY2SPEED				9
#define __VARFRIGHTPACMANSPEED			10
#define __VARFRIGTHDOTSPACMANSPEED		11
#define __VARFRIGHTGHOSTSPEED			12
#define __VARFRIGHTTIME					13
#define __VARNUMBERFLASHES				14

// Base of the speed...
#define __BASESPEED						10
#define __PERCENTSPEED					2

#endif
  
// End of the file
/*@}*/