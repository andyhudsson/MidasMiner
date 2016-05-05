// KingGame.cpp : Defines the entry point for the console application.
//


#include "Engine.h"


using namespace std;
using namespace King;

int main(int argc, char * args[])
{
	try
	{
		bool stop = false;
		SDL_Event e;

		std::unique_ptr<Engine> engine(new Engine);
		engine->CreateStage();

		for (;;)
		{
			while (SDL_PollEvent(&e))
			{
				switch (e.type) {

				case SDL_MOUSEBUTTONDOWN:

					if (engine->GetStatus() == Engine::SLEEPING)
						engine->DragStart(e.button.x, e.button.y);
					break;

				case SDL_MOUSEBUTTONUP:

					if (engine->GetStatus() == Engine::SLEEPING)
						engine->DragFinish(e.button.x, e.button.y);
					break;

				case SDL_QUIT:

					stop = true;
					break;
				}
			}

			engine->Update();
			engine->Render();

			if (stop) break;
		}

	}


	catch (std::exception &exc)
	{
		std::cout << "Exception thrown." << exc.what() << std::endl;
		cin.get();

		return -1;
	}
	catch (...)
	{
		std::cout << "Unknown exception occured." << std::endl;
		cin.get();

		return -1;
	}

	return 0;
}