#include "InputHandler.hpp"
#include "GameStates.hpp"
#include "Entities.hpp"
#include "General.hpp"
#include "Defs.hpp"

// ---
void InputHandler::onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt)
{
	// Take into account that if the joystick is not moving
	// this method will not be called, so there must be an exit option programmed
	// to stop the main character when the joystick is not being moved!

	// Only one joystick available...
	if (dt -> _numberJoystick == 0) 
	{
		// ...when the user is playing...
		if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
		{
			// If the device to control the character is not joystick when playing, nothing to do here...
			// Keyboard and joystick can not be active at the same time when playing
			// The reason is, no action in any of them generates a no movement of the main character
			// This method is call after processing any event, including keyboard events
			// So, if the joystick is being used to move the character, and a movement is generated
			// when the system enters this method, and the keyboard is not in use, the movement finishes...
			if (!_joystick)
				return; 

			// Notice that the movemnt is always incremental respect the previous one...
			// The joysticks points a position in the space. When move to right (i.e) 
			// the position starts to move towrds max position in the X. 
			// When the joystick is released, the position moves back to 0
			// If the joystick is maintained at the right, the position doesn't change.
			// This behaviour is used to determinate the direction of the main character
			// When is is the right and bigger than a fouth of the max position the position is 
			// incremented in one, and reduce in one if it isn't.
			// The x axis is the number 0
			if (dt -> _numberAxis == 0) 
			{
				_mY = 0;
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX < 1) ? 1 : 0;
					else
						_mX -= (_mX > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX > -1) ? -1 : 0;
					else
						_mX -= (_mX < 0) ? -1 : 0;
				}
			}
			else
			// The y axis is the number 1
			if (dt -> _numberAxis == 1) 
			{
				_mX = 0;
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY < 1) ? 1 : 0;
					else
						_mY -= (_mY > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY > -1) ? -1 : 0;
					else
						_mY -= (_mY < 0) ? -1 : 0;
				}
			}

			// Moves pacman...
			// x axis?
			if (_mX > 0) pacmanRight (); 
			else if (_mX < 0) pacmanLeft ();
			// y axis?
			if (_mY > 0) pacmanDown (); 
			else if (_mY < 0) pacmanUp ();
		}
		else
		// ...when he is in the initial selection of the difficulty
		if (_game -> activeState () -> type () == __GAMESTATEINITIAL)
		{
			if (dt -> _numberAxis == 1) // Moving in the y axis to select...
			{
				// The system will move focus to the next or previous position
				// Just if is not already changing! (see declaration of GameStateInitial class).
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ < 1) ? 1 : 0;
					else
						_dJ -= (_dJ > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ > -1) ? -1 : 0;
					else
						_dJ -= (_dJ < 0) ? -1 : 0;
				}
			}

			// Move to the option depending on the axis movement...
			GameStateInitial* st = (GameStateInitial*) _game -> activeState ();
			if (_dJ == 1) st -> nextOption ();
			else if (_dJ == -1) st -> previousOption ();
		}
		else
		// ...when he is introducing the initial after finalizing the game...
		if (_game -> activeState () -> type () == __GAMESTATEINTROLETTERS)
		{
			if (dt -> _numberAxis == 0) // Moving in the x axis to select...
			{
				// The system will move focus to the next or previous position
				// Just if is not already changing! (see declaration of GameStateInitial class).
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ < 1) ? 1 : 0;
					else
						_dJ -= (_dJ > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ > -1) ? -1 : 0;
					else
						_dJ -= (_dJ < 0) ? -1 : 0;
				}
			}

			// Move to the option depending on the axis movement...
			GameStateIntroLetters* st = (GameStateIntroLetters*) _game -> activeState ();
			if (_dJ == 1) st -> nextLetter ();
			else if (_dJ == -1) st -> previousLetter ();
		}
	}
}

// ---
void InputHandler::onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt)
{
	// Only one joystick can be active at the same time,
	// and the different actions make only sense when the button is releasd
	if (dt -> _numberJoystick == 0)
	{
		if (!dt -> _on)
		{
			if (_game -> activeState () -> type () == __GAMESTATEINITIAL && 
					dt -> _numberButton == 0)
				((GameStateInitial*) _game -> activeState ()) -> optionSelected ();
			else
			if (_game -> activeState () -> type () == __GAMESTATEINTROLETTERS &&
					dt -> _numberButton == 0)
				((GameStateIntroLetters*) _game -> activeState ()) -> letterSelected ();
			else
			if (_game -> activeState () -> type () == __GAMESTATESEESCORES &&
					dt -> _numberButton == 0)
				((GameStateSeeScore*) _game -> activeState ()) -> setWantToExit (true);
		}
	}
}

// ---
void InputHandler::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	int kPressed = -1;
	bool pKey = false;
	if (_lastKey != -1)
	{
		if (!dt -> _on)
		{
			kPressed = _lastKey;
			_lastKey = -1;
			pKey = true;
		}
	}
	else
		_lastKey = dt -> _internalCode;

	if (pKey)
	{
		int gS = _game -> activeState () -> type ();
		switch (gS)
		{
			case __GAMESTATEINITIAL:
				manakeKeyOnSelectionState (kPressed);
				break;

			case __GAMESTATEPRELUDE:
				manakeKeyOnPreludeState (kPressed);
				break;

			case __GAMESTATEPLAYING:
				manakeKeyOnPlayingState (kPressed);
				break;

			case __GAMESTATEINTROLETTERS:
				manageKeyOnIntroLetterState (kPressed);
				break;

			case __GAMESTATESEESCORES:
				manageKeyOnSeeScoreState (kPressed);
				break;

			default:
				break;
		};
	}
}

// ---
void InputHandler::onMouseMovementEvent (QGAMES::MouseMovementEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{
		case __GAMESTATEINITIAL:
			((GameStateInitial*) _game -> activeState ()) -> 
				optionAt (QGAMES::Position (__BD dt -> _x, __BD dt -> _y, __BD 0));
			break;
	}
}

// ---
void InputHandler::onMouseButtonEvent (QGAMES::MouseButtonEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{

		case __GAMESTATEINITIAL:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on) // To select one option...
				((GameStateInitial*) _game -> activeState ()) -> optionSelected ();
			break;

		case __GAMESTATEPLAYING:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on) // To pause / continue the game...
				_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
			break;

		case __GAMESTATESEESCORES:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on) // To return to the rpevious state...
				((GameStateSeeScore*) _game -> activeState ()) -> setWantToExit (true);
			break;

		default:
			break;
	}
}

// ---
void InputHandler::onKeyboardStatus (const std::vector <bool>& kS)
{
	if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
	{
		// If the device to control pacman is the joystick, nothing to do here...
		// Keyboard and joystick can not be active at the same time when playing
		// The reason is, no action in any of them generates a no movement of the pacman
		// This method is call after processing any event, including keyboard events
		// So, if the joystick is being used to move the character, and a movement is generated
		// when the system enters this method, and the keyboard is not in use, the movement finishes...
		if (_joystick)
			return; 

		if (kS [__PACMANRIGHT]) pacmanRight ();
		else if (kS [__PACMANLEFT]) pacmanLeft ();
		else if (kS [__PACMANDOWN]) pacmanDown ();
		else if (kS [__PACMANUP]) pacmanUp ();
		// If no key is pressed the character stops!
	}
}

// ---
void InputHandler::manakeKeyOnSelectionState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_RETURN)
	{
		((GameStateInitial*) _game -> activeState ()) -> optionSelected ();
		game () -> inputHandler () -> clearPendingEvents ();
	}
	else
	if (k == QGAMES::KeyCode::QGAMES_DOWN)
		((GameStateInitial*) _game -> activeState ()) -> nextOption ();
	else
	if (k == QGAMES::KeyCode::QGAMES_UP)
		((GameStateInitial*) _game -> activeState ()) -> previousOption ();
}

// ---
void InputHandler::manakeKeyOnPlayingState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_Z) // To pause/continue the game...
	{
		_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
		if (_game -> isGamePaused ()) QGAMES::SoundSystem::system () -> pause ();
		else QGAMES::SoundSystem::system () -> resume (); // Stops all sounds whilst pause is set!
	}
}

// ---
void InputHandler::manakeKeyOnPreludeState (int k)
{
	// Any key is valid to move the game to the next state...
	((GameStatePrelude*) _game -> activeState ()) -> setWantToExit (true);
}

// ---
void InputHandler::manageKeyOnIntroLetterState (int k)
{
	GameStateIntroLetters* s = (GameStateIntroLetters*) _game -> activeState ();
	if (k == QGAMES::KeyCode::QGAMES_RIGHT) s -> nextLetter ();
	else if (k == QGAMES::KeyCode::QGAMES_LEFT) s -> previousLetter ();
	else if (k == QGAMES::KeyCode::QGAMES_RETURN) s -> letterSelected ();
}

// ---
void InputHandler::manageKeyOnSeeScoreState (int k)
{
	GameStateSeeScore* s = (GameStateSeeScore*) _game -> activeState ();
	if (k == QGAMES::KeyCode::QGAMES_RETURN) s -> setWantToExit (true);
}

// ---
void InputHandler::pacmanLeft ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._left);
}

// ---
void InputHandler::pacmanRight ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._right);
}

// ---
void InputHandler::pacmanUp ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._up);
}

// ---
void InputHandler::pacmanDown ()
{
	(__AT (__AGM _game) -> artist (__ENTITYPACMAN)) -> setMove (General::_e._down);
}
