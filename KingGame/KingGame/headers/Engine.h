#ifndef ENGINE_H
#define ENGINE_H

#include <ctime>
#include <memory>
#include <vector>
#include <deque>
#include <cmath>

#include <SDL_ttf.h>

#include "Asset.h"
#include "Exchanger.h"
#include "Timer.h"
#include "TextRenderer.h"

using namespace std;

namespace King {

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	const int ADD_X = 350;
	const int ADD_Y = 100;

	const int ASSETS_ROWS = 8;
	const int ASSETS_COLS = 8;

	// Duration of game session in seconds
	const int STAGE_DURATION = 60;

	// Space between assets
	const int ASSET_SEPARATOR = 10;

	// Dividors to calculate position of asset
	const int COL_DIV = ASSET_WIDTH + ASSET_SEPARATOR;
	const int ROW_DIV = ASSET_HEIGHT + ASSET_SEPARATOR;

	// Console name
	const char WINDOW_CAPTION [] = "KING GAME ASSIGNMENT. SOFTWARE DEVELOPER TEST.";

	// Score, set per hit of adjacent sequence of assets of the same color
	const enum Score{

		HIT3 = 100,
		HIT4 = 200,
		HIT5 = 500,
		HIT6 = 1000,
		HIT7 = 3000,
		HIT8 = 5000
	};

	const int COLORS_NUMBER = 5;
	const int MAX_COLOR_PATH = 20;

	const enum assetsColors{

		RED=0,
		PURPLE,
		BLUE,
		YELLOW,
		GREEN
	};

	// Pathes to colors from assetsColors,
	// So the order should be aligned if changed assetsColors or COLOR_PATHES.
	const char COLOR_PATHES [COLORS_NUMBER] [MAX_COLOR_PATH] =
	{
		{ "assets/Red.png" },
		{ "assets/Purple.png" },
		{ "assets/Blue.png" },
		{ "assets/Yellow.png" },
		{ "assets/Green.png" },
	};

	const char BACKGROUND_PATH [] = "assets/BackGround.jpg";

	class Engine
	{
		static const int BUF_LEN = 9;

		int _width;
		int _height;

		int _duration;

		int _status;

		int _fromColumn;
		int _fromRow;
		int _toColumn;
		int _toRow;

		bool _dragging;
		bool _toRefill;
		bool _toRefillRoof;

		size_t _score;
		char _scoreStr[BUF_LEN];

		std::unique_ptr <Timer> _timer;
		std::unique_ptr <Object> _backGround;

		std::vector <std::pair <int, int> > _assetSequences;
		std::vector <std::pair <int, int> > _shiftedAssets;

		std::vector <std::shared_ptr <Asset>> _newAssets;

		std::deque  <std::shared_ptr <Asset>> _assetsPool;

		std::vector <vector <std::shared_ptr <Asset>>> _assets;

		std::unique_ptr <Exchanger> _exchanger;

		std::unique_ptr <TextRenderer> _scoreLabel;
		std::unique_ptr <TextRenderer> _scoreValue;
		std::unique_ptr <TextRenderer> _timeLeft;

		SDL_Renderer * _renderer;
		SDL_Window   * _window;

		

	public:

		// Engine's statuses
		static enum Status {

			STARTING,
			SLEEPING,
			SWAPPING,
			RESTORING,
			PROCESSING,
			PROCESS_ENDED,
			FINISH
		};

 		Engine();

		~Engine();

		void CreateStage();

		void DragFinish(int x, int y);

		void DragStart(int x, int y);

		int GetStatus() const;

		void Render();

		void SetStatus(int status);

		void Update();

	private:

		void AddScore(size_t hit);

		void AssetsPoolShuffle();

		void CheckColumnSequences();

		void CoverUpperRow();

		void ExchangeAssets(int fromColumn, int fromRow,
							int toColumn, int toRow);

		void CheckRowSequences();

		void FixGaps();

		std::pair <int, int> GetColAndRowByCoord(int x, int y);

		const char * GetPathByColor(int color) const;

		bool IsSequence(int col, int row);

		void PopSequences();

		void ProcesShiftedAssets();

		int SetAssetsColor();

		void ShiftAssetsDown(std::vector <std::shared_ptr <Asset> > &beMoved);
		




	
	};
}
#endif
