#ifndef EXCHANGER_H
#define EXCHANGER_H

#include <SDL.h>
#include <memory>

#include "Asset.h"

const int ASSET_WIDTH = 35;
const int ASSET_HEIGHT = 35;

const int MOVE_STEP = 1;

class Exchanger
{
	std::shared_ptr <Asset>  _leftPtr;
	std::shared_ptr <Asset>  _rightPtr;

	SDL_Rect _leftRect;
	SDL_Rect _rightRect;

	bool _finished;
	bool _rollback;

	int _mode;

public:

	enum Mode {

		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Exchanger(	std::shared_ptr <Asset> leftPtr,
				std::shared_ptr <Asset> rightPtr);

	~Exchanger();

	void SetMode(Mode mode);

	void Reverse();

	void Update();
	bool IsRollBack();
	bool Done();


};

#endif