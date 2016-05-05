
#include "Engine.h"



namespace King {

	Engine::Engine() :_width(WINDOW_WIDTH),
					  _height(WINDOW_HEIGHT),
					  _duration(STAGE_DURATION),
					  _status(STARTING),
					  _fromColumn(-1),
					  _fromRow(-1),
					  _toColumn(-1),
					  _toRow(-1),
					  _dragging(false),
					  _toRefill(false),
					  _toRefillRoof(false),
					  _score(0)
					  
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_Quit();
			throw std::runtime_error("SDL_Init returned error. " + string(SDL_GetError()));
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
			SDL_Quit();
		}

		_window = SDL_CreateWindow(
			WINDOW_CAPTION,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			_width,
			_height,
			SDL_WINDOW_OPENGL);

		if (_window == NULL)
		{
			SDL_Quit();
			throw std::runtime_error("SDL_CreateWindow returned error. " + string(SDL_GetError()));
		}

		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);

		if (_renderer == NULL)
		{
			SDL_DestroyWindow(_window);
			SDL_Quit();
			throw std::runtime_error("SDL_CreateRenderer returned error. " + string(SDL_GetError()));
		}

		_backGround = std::make_unique<Object>(_renderer);
		_timer = std::make_unique<Timer>();

		_scoreLabel = std::make_unique <TextRenderer>();
		_scoreLabel->SetRenderer(_renderer);
		_scoreLabel->LoadMedia("assets/font.ttf", "Score: ", 40, { 255, 0, 255 });

		_scoreStr[0] = '0';
		_scoreStr[1] = '\0';

		_scoreValue = std::make_unique <TextRenderer>();
		_scoreValue->SetRenderer(_renderer);
		_scoreValue->LoadMedia("assets/font.ttf", _scoreStr, 40, { 255, 0, 255 });

		_timeLeft = std::make_unique <TextRenderer>();
		_timeLeft->SetRenderer(_renderer);
		_timeLeft->LoadMedia("assets/font.ttf", _timer->GetTimeStr(), 65, { 255, 0, 255 });
	}

	Engine::~Engine()
	{
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}

	void Engine::AddScore(size_t hit)
	{
		switch (hit)
		{
		case 3:
			_score += HIT3;
			break;

		case 4:
			_score += HIT4;
			break;

		case 5:
			_score += HIT5;
			break;

		case 6:
			_score += HIT6;
			break;

		case 7:
			_score += HIT7;
			break;

		case 8:
			_score += HIT8;
			break;

		default:
			break;
		}

		_itoa_s(_score, _scoreStr, BUF_LEN, 10);
		_scoreValue->LoadFromRenderedText(_scoreStr, { 255, 0, 255 });

		////////////////////////////////////////////////////////////////
	}

	void Engine::AssetsPoolShuffle()
	{
		for (auto i = 0; i < _assetsPool.size(); i+=2)
		{
			if (_assetsPool[i] == NULL) continue;
			swap(_assetsPool[i], _assetsPool[_assetsPool.size() - 1 - (rand() % (_assetsPool.size() - 5))]);
		}
	}


	int Engine::SetAssetsColor()
	{
		static float cnt;
		cnt+=0.5;

		if (cnt > 4) cnt = 0;
			
		return (int)cnt;
	}

	 

	const char * Engine::GetPathByColor(int color) const
	{
		return COLOR_PATHES[color];
	}

	void Engine::CreateStage()
	{
		this->SetStatus(Engine::STARTING);

		std::srand(std::time(0));

		_timer->SetTime(_duration);
		_assets.resize(ASSETS_ROWS);

		_backGround->SetRectPos({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT});
		_backGround->LoadTexture(BACKGROUND_PATH);

		// Crete a pool of assets to speed up.
		_assetsPool.resize(ASSETS_ROWS*ASSETS_COLS);
		auto poolCnt = 0;

		for (auto row = 0; row < ASSETS_ROWS; ++row)
		{
			_assets[row].resize(ASSETS_COLS);

			for (auto col = 0; col < ASSETS_COLS; ++col)
			{
				_assets[row][col] = std::make_shared <Asset> ();

				_assetsPool[poolCnt] = std::make_shared <Asset>();

				_assets[row][col]->SetRectPos({ col * COL_DIV + ADD_X, 
												row * ROW_DIV + ADD_Y, 
												ASSET_WIDTH, 
												ASSET_HEIGHT });

				_assetsPool[poolCnt]->SetRectPos({ 0, 0, ASSET_WIDTH, ASSET_HEIGHT });


				_assets[row][col]->setRow(row);
				_assets[row][col]->setColumn(col);

				_assetsPool[poolCnt]->setRow(0);
				_assetsPool[poolCnt]->setColumn(0);

				_assets[row][col]->setColor(SetAssetsColor());

				_assetsPool[poolCnt]->setColor(rand() % 5);

				_assets[row][col]->SetRenderer(_renderer);
				_assets[row][col]->LoadTexture(GetPathByColor(_assets[row][col]->getColor()));


				_assetsPool[poolCnt]->SetRenderer(_renderer);
				_assetsPool[poolCnt]->LoadTexture(GetPathByColor(_assetsPool[poolCnt]->getColor()));


				++poolCnt;
			}
		}

 
		this->SetStatus(Engine::SLEEPING);
	}


	void Engine::Render()
	{
		 _backGround->Render();

		for (auto row = 0; row < ASSETS_ROWS; ++row)
			for (auto column = 0; column < ASSETS_COLS; ++column)
				_assets[row][column]->Render();

		_scoreLabel->Render(10, 150);

		_scoreValue->Render(125, 150);

		static Uint32 cntTime = STAGE_DURATION - 1;

		// This is to minimize number of calls to LoadFromRenderedText,
		// call only if time is changed.
		if (_timer->GetTime() == cntTime)
		{
			_timeLeft->LoadFromRenderedText(_timer->GetTimeStr(), { 255, 0, 255 });
			--cntTime;
		}

		_timeLeft->Render(90, 440);



		SDL_RenderPresent(_renderer);
	}

	int Engine::GetStatus() const
	{
		return _status;
	}

	void Engine::SetStatus(int status)
	{
		_status = status;
	}

	void Engine::Update()
	{
		switch (this->GetStatus())
		{

		case Engine::STARTING:
			break;

		case Engine::SLEEPING:

			_timer->Update();

			if(
				!_newAssets.empty() ||
				!_assetSequences.empty()
				||
				!_shiftedAssets.empty()
			)
			this->SetStatus(Engine::PROCESSING);

			if (_timer->Finished())
				this->SetStatus(Engine::FINISH);

			break;

		case Engine::SWAPPING:

			_timer->Update();
			_exchanger->Update();

			if (_exchanger->Done())
			{
				ExchangeAssets(_fromColumn, _fromRow, _toColumn, _toRow);

				if (IsSequence(_fromColumn, _fromRow)
					||
					IsSequence(_toColumn, _toRow))
				{
					_exchanger.reset(nullptr);
					_fromColumn = _fromRow = _toColumn = _toRow = -1;
					CheckColumnSequences();
					CheckRowSequences();

					this->SetStatus(Engine::PROCESSING);
				}
				else
				{
					_exchanger->Reverse();
					this->SetStatus(Engine::RESTORING);
				}
			}
			break;

		case Engine::RESTORING:

			_timer->Update();
			_exchanger->Update();

			if (_exchanger->Done())
			{

				ExchangeAssets(_fromColumn, _fromRow, 
							   _toColumn, _toRow);

				_exchanger.reset();

				_fromColumn = _fromRow = _toColumn = _toRow = -1;

				_status = Engine::SLEEPING;
			}

			break;

		case Engine::PROCESSING:

			_timer->Update();

			PopSequences();

			FixGaps();

			ProcesShiftedAssets();

			CoverUpperRow();

			ShiftAssetsDown(_newAssets);


			if (
				_newAssets.empty()
				&&
				_assetSequences.empty()
				&& 
				_shiftedAssets.empty() 
				)
				_status = PROCESS_ENDED;

			break;

		case Engine::PROCESS_ENDED:

			_timer->Update();

			CheckRowSequences();
			CheckColumnSequences();

			_status = Engine::SLEEPING;

			AssetsPoolShuffle();

			break;

		case Engine::FINISH:
			//_timeLeft->LoadFromRenderedText("Time's up!", { 0, 0, 255 });
			break;
		} 
	}

	// returns Asset's column and row by passed coordinates
	std::pair <int,int> Engine::GetColAndRowByCoord(int x, int y)
	{
		return std::make_pair
			(
				(x - ADD_X + ASSET_SEPARATOR) / COL_DIV,
				(y - ADD_Y + ASSET_SEPARATOR) / ROW_DIV
			);
	}

 
	void Engine::DragStart(int x, int y)
	{
		std::pair <int,int> ColAndRow = GetColAndRowByCoord(x, y);

		if (ColAndRow.first >= 0 && 
			ColAndRow.second >= 0 &&
			ColAndRow.first < ASSETS_COLS &&
			ColAndRow.second < ASSETS_ROWS
			)
		{
			_dragging = true;

			_fromColumn = ColAndRow.first;
			_fromRow = ColAndRow.second;
		}
	}


	void Engine::DragFinish(int x, 
							int y)
	{
		if (!_dragging) return;

		std::pair <int, int> ColAndRow = GetColAndRowByCoord(x, y);

		_toRow = ColAndRow.second;
		_toColumn = ColAndRow.first;

		if (_fromRow == _toRow && _fromColumn == _toColumn)
		{

			// Get Up, Down, Left, Right neighbours
			std::vector <std::pair <int, int>> neighbours;
			if (_toRow > 0) 
				neighbours.push_back(make_pair(_toRow - 1, _toColumn)); //Up

			if (_toRow < ASSETS_ROWS - 1) 
				neighbours.push_back(make_pair(_toRow + 1, _toColumn)); //Down

			if (_toColumn > 0)
				neighbours.push_back(make_pair(_toRow, _toColumn - 1)); //Left

			if (_toColumn < ASSETS_COLS - 1)
				neighbours.push_back(make_pair(_toRow, _toColumn + 1)); //Right


			double minDist = DBL_MAX;
			int trgCol = -1, trgRow = -1;

			for (auto i = 0; i < neighbours.size(); ++i)
			{
				SDL_Rect rct = _assets[neighbours[i].first][neighbours[i].second]->GetRectPos();

				// look for minimal (Euclidean distance) for assets around for exchange
				double dst = sqrt(pow(x - rct.x-10, 2) + pow(y - rct.y-10, 2));

				if (minDist > dst)
				{
					minDist = dst;
					trgRow = neighbours[i].first;
					trgCol = neighbours[i].second;
				}
			}

			_toRow = trgRow;
			_toColumn = trgCol;
		}

		if ((_fromRow != _toRow)  && (_fromColumn != _toColumn))
		{
			_dragging = false;
			return;
		}

		if (_toRow >= 0 && 
			_toColumn >= 0
			&&
			_toRow < ASSETS_ROWS
			&&
			 _toColumn < ASSETS_COLS)
		{
			int colDiff = _toColumn - _fromColumn;
				
			int direction = 0;

			if (colDiff > 0)
			{
				direction = Exchanger::RIGHT;
				_toColumn = _fromColumn + 1;
			}

			if (colDiff < 0)
			{
				direction = Exchanger::LEFT;
				_toColumn = _fromColumn - 1;
			}

			int rowDiff = _toRow - _fromRow;

			if (rowDiff > 0)
			{
				direction = Exchanger::DOWN;
				_toRow = _fromRow + 1;
			}

			if (rowDiff < 0)
			{
				direction = Exchanger::UP;
				_toRow = _fromRow - 1;
			}

			_exchanger = make_unique <Exchanger>(
					_assets[_fromRow][_fromColumn], 
					_assets[_toRow][_toColumn]
					);

			_exchanger->SetMode((Exchanger::Mode)direction);

			_dragging = false;

			this->SetStatus(Engine::SWAPPING);
		}
		else
		{
			_fromColumn = _fromRow = _toColumn = _toRow = -1;
			_dragging = false;
		}
	}

	

	bool Engine::IsSequence(int col, 
							int row)
	{

		auto color = _assets[row][col]->getColor();
		auto cnt = 0;

		auto toLeft = col - 1;
		auto toRight = col + 1;

		while (	toLeft >= 0
					&&
				_assets[row][toLeft]->getColor() == color)
 
		{ --toLeft; ++cnt; }
 
		if (cnt >= 2) return true;

		while (
			toRight < ASSETS_COLS
			&&
			_assets[row][toRight]->getColor() == color
			)
		{ ++toRight; ++cnt; }

		if (cnt >= 2) return true;


		cnt = 0;
		toLeft = row - 1;

		while (
			toLeft >= 0
			&&
			_assets[toLeft][col]->getColor() == color
			)
		{ --toLeft; ++cnt; }

		if (cnt >= 2) return true;

		toRight = row + 1;

		while (
			toRight < ASSETS_ROWS
			&&
			_assets[toRight][col]->getColor() == color
			)
		{ ++toRight; ++cnt; }

		return cnt >= 2;
	}

	void Engine::ExchangeAssets(int fromColumn, 
								int fromRow,
								int toColumn, 
								int toRow)
	{
		_assets[toRow][toColumn]->setColumn(fromColumn);
		_assets[toRow][toColumn]->setRow(fromRow);

		_assets[fromRow][fromColumn]->setColumn(toColumn);
		_assets[fromRow][fromColumn]->setRow(toRow);

		std::swap(_assets[fromRow][fromColumn], _assets[toRow][toColumn]);
	}

	void Engine::PopSequences()
	{
		for (auto i = 0; i < _assetSequences.size(); ++i, _toRefill = true)
		{
			if (_assets[_assetSequences[i].first][_assetSequences[i].second] != NULL)
				_assetsPool.push_back(std::move(_assets[_assetSequences[i].first][_assetSequences[i].second]));

		}
		_assetSequences.clear();
	}

	void Engine::FixGaps()
	{
		if (!_toRefill)
			return;

		_toRefillRoof = true;

		for (auto col = ASSETS_COLS - 1; col >= 0; --col)
		{
			for (auto row = ASSETS_ROWS - 1; row >= 0; --row)
			{
				if (_assets[row][col] == nullptr)
				{
					for (int fix = row - 1; fix >= 0; --fix)
					{
						if (_assets[fix][col] != nullptr)
						{
							_assets[fix][col]->setRow(row);
							_assets[fix][col]->setColumn(col);

							_assets[row][col] = std::move(_assets[fix][col]);

							_assets[fix][col] = nullptr;

							_shiftedAssets.push_back(std::make_pair(row,col));


							break;
						}
					}
				}
			}
		}

		_toRefill = false;
	}



	 

	void Engine::CoverUpperRow()
	{
		if (!_toRefillRoof)
			return;

		

		for (auto	col = 0; 
					col < ASSETS_COLS; 
					++col)
		{
			int poolCnt = 0;

			for (auto	row = 0;
						row < ASSETS_ROWS && _assets[row][col] == nullptr;
						++row)
			{
				while (_assetsPool[poolCnt] == NULL)
						_assetsPool.pop_front();

				_assetsPool[poolCnt]->setColumn(col);
				_assetsPool[poolCnt]->setRow(row);

				_assetsPool[poolCnt]->SetRectPos({ col * ROW_DIV + ADD_X,
					2 * ASSET_HEIGHT,
					ASSET_WIDTH,
					ASSET_HEIGHT
				});

				_assets[row][col] = std::move(_assetsPool[poolCnt]);
				_newAssets.push_back(_assets[row][col]);

				_assetsPool[poolCnt] = NULL;

				++poolCnt;

			}
		}

		

		_toRefillRoof = false;
	}

	void Engine::ProcesShiftedAssets()
	{
		for (auto shift = 0; shift < _shiftedAssets.size(); ++shift)
		{
			int y = ASSET_HEIGHT * _assets[_shiftedAssets[shift].first]
										  [_shiftedAssets[shift].second]->getRow()
				+
				ASSET_SEPARATOR * _assets[_shiftedAssets[shift].first]
										 [_shiftedAssets[shift].second]->getRow()
				+
				ADD_Y;

			_assets[_shiftedAssets[shift].first]
				   [_shiftedAssets[shift].second]->SetRectPos(
			{
				_assets[_shiftedAssets[shift].first]
				[_shiftedAssets[shift].second]->GetRectPos().x,
				_assets[_shiftedAssets[shift].first]
				[_shiftedAssets[shift].second]->GetRectPos().y + MOVE_STEP,
				ASSET_WIDTH,
				ASSET_HEIGHT
			});

			if (_assets[_shiftedAssets[shift].first]
				[_shiftedAssets[shift].second]->GetRectPos().y >= y)
					_shiftedAssets.erase(_shiftedAssets.begin() + shift);
		}


	}

	//void Engine::ShiftAssetsDown(std::vector <std::shared_ptr <Asset> > & beMoved)
	void Engine::ShiftAssetsDown(std::vector <std::shared_ptr <Asset> > & beMoved)
	{
		for (auto shift = 0; shift < beMoved.size(); ++shift)
		{
			int y = ASSET_HEIGHT * beMoved[shift]->getRow()
				+
				ASSET_SEPARATOR * beMoved[shift]->getRow()
				+
				ADD_Y;

			beMoved[shift]->SetRectPos(
			{
				beMoved[shift]->GetRectPos().x,
				beMoved[shift]->GetRectPos().y + MOVE_STEP,
				ASSET_WIDTH,
				ASSET_HEIGHT
			});

			if (beMoved[shift]->GetRectPos().y >= y)
			{
				beMoved[shift].reset();
				beMoved.erase(beMoved.begin() + shift);
			}
		}
		

	}

	void Engine::CheckRowSequences()
	{
		for (auto row = 0; row < ASSETS_ROWS; ++row)
		{
			for (auto col = 0; col < ASSETS_COLS - 2; ++col)
			{
				auto color = _assets[row][col]->getColor();

				if (color == _assets[row][col + 1]->getColor()
					&&
					color == _assets[row][col + 2]->getColor())
				{
					auto hit = 3;
					
					_assetSequences.push_back(make_pair(row, col));
					_assetSequences.push_back(make_pair(row, col + 1));
					_assetSequences.push_back(make_pair(row, col + 2));

					col += 3;

					for (; col < ASSETS_ROWS
						&&
						color == _assets[row][col]->getColor();
						++col)
					{
						_assetSequences.push_back(make_pair(row,col));
						++hit;
					}

					AddScore(hit);


				}
			}
		}
	}

	void Engine::CheckColumnSequences()
	{
		for (auto col = 0; col < ASSETS_COLS; ++col)
		{
			for (auto row = 0; row < ASSETS_ROWS - 2;++row)
			{
				auto color = _assets[row][col]->getColor();

				if (color == _assets[row + 1][col]->getColor()
					&&
					color == _assets[row + 2][col]->getColor())
				{
					auto hit = 3;
					vector <std::shared_ptr <Asset> > assetSeqs;

					_assetSequences.push_back(make_pair(row, col));
					_assetSequences.push_back(make_pair(row + 1, col));
					_assetSequences.push_back(make_pair(row + 2, col));

					row += 3;

					for (; row < ASSETS_ROWS
						&&
						color == _assets[row][col]->getColor();
						++row)
					{
						_assetSequences.push_back(make_pair(row, col));
						++hit;
					}
 
					AddScore(hit);
				}
			}
		}
	}

	 

	};

