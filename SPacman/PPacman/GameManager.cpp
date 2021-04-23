#include "GameManager.h"

using namespace std;

GameManager::GameManager() {
	gWindow = nullptr;
	gRenderer = nullptr;
	gScreenSurface = nullptr;
	gPacmanTexture = nullptr;
	gMonedaTexture = nullptr;
	pacmanTextura = nullptr;
	juego_en_ejecucion = true;
}
	
int GameManager::onExecute() {
	srand(time(NULL));
	if (onInit() == false) {
		return -1;
	}
	pacman = new Pacman(pacmanTextura, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT, 5);
	cout << pacman->getIdObjeto() << endl;
	actoresJuego.push_back(new Fantasma(fantasma1Texture, 0, 0, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT, 5));
	actoresJuego.push_back(new Fantasma(fantasma2Texture, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT, 5));
	actoresJuego.push_back(new Fantasma("Clyde.bmp", 220, 220, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT, 5));

	actoresJuego.push_back(new Fruta(gRenderer, gFrutasTextures, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT));

	int posx = 0;
	for (int i = 0; i < 10; i++)
	{
		posx = i * 50;
		//monedas.push_back(new Moneda(gRenderer, gMonedaTexture, posx, 100, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT));
		actoresJuego.push_back(new Moneda(gRenderer, gMonedaTexture, posx, 100, 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT));
	}

	for (int i = 0; i < 4; i++)
	{
		actoresJuego.push_back(new Moneda(gRenderer, gSuperMonedaTexture, 50 + (i * 50), 50 + (i * 50), 25, 25, SCREEN_WIDTH, SCREEN_HEIGHT));
	}

	for (int i = 0; i < actoresJuego.size(); i++) {
		cout << actoresJuego[i]->getIdObjeto() << endl;
	}

	srand(time(NULL));


	SDL_Event Event;

	while (juego_en_ejecucion) {
		while (SDL_PollEvent(&Event)) {
			onEvent(&Event);
			pacman->handleEvent(Event);
		}
		// Mover Pacman
		pacman->move();

		for (int i = 0; i < actoresJuego.size(); i++) {
			actoresJuego[i]->move();
			actoresJuego[i]->mostrar();
		}
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Update screen

		onLoop();
		onRender();
		SDL_RenderPresent(gRenderer);
	}

	onCleanup();

	return 0;
}

bool GameManager::onInit() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Pacman USFX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{

			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}


			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);

			gPacmanTexture = loadTexture("pacmansito.bmp");
			if (gPacmanTexture == NULL)
			{
				cout << "Fallo en la carga de la textura" << endl;
				success = false;
			}
			Texture::renderer = gRenderer;
			//Fantasmas
			pacmanTextura = new Texture();
			pacmanTextura->loadFromImage("pacmansito.bmp");

			fantasma1Texture = new Texture();
			fantasma1Texture->loadFromImage("Fantasma1.bmp");
			fantasma2Texture = new Texture();
			fantasma2Texture->loadFromImage("Fantasma2.bmp");

			//Frutitas
			gFrutasTextures.push_back(loadTexture("Fruta1.png"));
			gFrutasTextures.push_back(loadTexture("Fruta2.png"));
			gFrutasTextures.push_back(loadTexture("Fruta3.png"));
			gFrutasTextures.push_back(loadTexture("Fruta4.png"));

			//Monedas
			gMonedaTexture = loadTexture("monedas.bmp");
			if (gMonedaTexture == NULL)
			{
				cout << "Fallo en la carga de la textura" << endl;
				success = false;
			}

			gSuperMonedaTexture = loadTexture("supermoneda.bmp");
			if (gSuperMonedaTexture == NULL)
			{
				cout << "Fallo en la carga de la textura" << endl;
				success = false;
			}
		}
	}

	return success;
};

void GameManager::onEvent(SDL_Event* Event) {
	if (Event->type == SDL_QUIT) {
		juego_en_ejecucion = false;
	}
};
void GameManager::onLoop() {};
void GameManager::onRender() {
	pacman->update();
	pacman->render();
	for (int i = 0; i < actoresJuego.size(); i++) {
		actoresJuego[i]->update();
	}
	for (int i = 0; i < actoresJuego.size(); i++) {
		actoresJuego[i]->render();
	}
};

void GameManager::onCleanup() {
	SDL_FreeSurface(gScreenSurface);

	SDL_Quit();
};


SDL_Texture* GameManager::loadTexture(string path)
{
	// Textura final generada
	SDL_Texture* newTexture = nullptr;

	// Carga una imagen de una ruta especifica
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "No se pudo cargarla imagen, SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		// Crea una textura a partir de una superficie de pixeles
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "No se pudo crear la textura, SDL Error: " << SDL_GetError() << endl;
		}

		// Libera la superficie cargada
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}