
#include "Exchanger.h"


Exchanger::Exchanger(	std::shared_ptr <Asset> leftPtr, 
						std::shared_ptr <Asset> rightPtr) 
										: 
										_leftPtr(leftPtr), 
										_rightPtr(rightPtr),
										_leftRect(rightPtr->GetRectPos()),
										_rightRect(leftPtr->GetRectPos()),
										_finished(false),
										_rollback(false)
{}

Exchanger::~Exchanger()
{}


bool Exchanger::Done()
{
	return _finished;
}

bool Exchanger::IsRollBack()
{
	return _rollback;
}

void Exchanger::Reverse()
{
	_rollback = true;
	_finished = false;

	std::swap(	_leftPtr, 
				_rightPtr);

	_leftRect = _rightPtr->GetRectPos();
	_rightRect = _leftPtr->GetRectPos();
}

void Exchanger::SetMode(Mode mode)
{ 
	_mode = mode;
}

void Exchanger::Update()
{
	if (!_finished)
	{
		SDL_Rect leftRect = { 0, 0, ASSET_WIDTH, ASSET_HEIGHT }, 
				 rightRect = { 0, 0, ASSET_WIDTH, ASSET_HEIGHT };

		switch (_mode)
		{

		case Exchanger::UP:

			leftRect.x = _leftPtr->GetRectPos().x,
				leftRect.y = _leftPtr->GetRectPos().y - MOVE_STEP;

			rightRect.x = _rightPtr->GetRectPos().x,
				rightRect.y = _rightPtr->GetRectPos().y + MOVE_STEP;

			break;

		case Exchanger::DOWN:

			leftRect.x = _leftPtr->GetRectPos().x,
				leftRect.y = _leftPtr->GetRectPos().y + MOVE_STEP;

			rightRect.x = _rightPtr->GetRectPos().x,
				rightRect.y = _rightPtr->GetRectPos().y - MOVE_STEP;

			break;

		case Exchanger::LEFT:

			leftRect.x = _leftPtr->GetRectPos().x - MOVE_STEP;
			leftRect.y = _leftPtr->GetRectPos().y;
	 

			rightRect.x = _rightPtr->GetRectPos().x + MOVE_STEP;
			rightRect.y = _rightPtr->GetRectPos().y;

			break;

		case Exchanger::RIGHT:

			leftRect.x = _leftPtr->GetRectPos().x + MOVE_STEP,
			leftRect.y = _leftPtr->GetRectPos().y;

			rightRect.x = _rightPtr->GetRectPos().x - MOVE_STEP,
			rightRect.y = _rightPtr->GetRectPos().y;

			break;
		}

		_leftPtr->SetRectPos(leftRect);
		_rightPtr->SetRectPos(rightRect);

		if (_leftPtr->GetRectPos().x == _leftRect.x
			&& 
			_leftPtr->GetRectPos().y == _leftRect.y)
			_finished = true;			
	}
}