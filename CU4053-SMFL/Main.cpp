// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.

#include <iostream>
#include "Level.h"
#include "Menu.h"
#include "TileEditor.h"
#include "Framework/TileManager.h"
#include "Framework/GameState.h"
#include "Framework/World.h"
#define WINDOWWIDTH 1080
#define WINDOWHEIGHT 720	

void windowProcess(sf::RenderWindow* window, Input* in, Level* level)
{
	// Handle window events.
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			level->adjustViewToWindowSize(event.size.width, event.size.height);
			break;
		case sf::Event::KeyPressed:
			// update input class
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::DOWN);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::DOWN);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::UP);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::UP);
			}
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main()
{
	//Create the window
	sf::RenderWindow window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "CU4053");
	sf::View view;
	sf::Vector2f viewSize = sf::Vector2f(window.getSize().x, window.getSize().y);
	view.setSize(viewSize.x, viewSize.y);
	view.setCenter(viewSize.x / 2, viewSize.y / 2);
	// Initialise input and level objects.
	Input input;
	World world;
	world.setGravity(sf::Vector2f(0, 980.f));
	
	TileManager tileManager;
	GameState gameState;
	
	Level level(&window, &input, &gameState,&view,&world,&tileManager);
	Menu menu(&window, &input, &gameState);
	TileEditor tileEditor(&window, &input, &gameState, &view, &world, &tileManager);
	
	
	gameState.setCurrentState(State::MENU);

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;
	float FrameRate = 60.f;
	// Game Loop
	while (window.isOpen())
	{
		//Process window events
		windowProcess(&window, &input, &level);

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		if (deltaTime > 1.0f / FrameRate)
		{
			deltaTime = 1.0f / FrameRate;
		}

		switch (gameState.getCurrentState())
		{
		case(State::MENU):
			menu.handleInput(deltaTime);
			menu.update(deltaTime);
			menu.render();

			break;
		case(State::LEVEL):
			world.UpdatePhysics(deltaTime);
			level.handleInput(deltaTime);
			level.update(deltaTime);
			level.render();
			break;
		case(State::TILEEDITOR):
			world.UpdatePhysics(deltaTime);
			tileEditor.handleInput(deltaTime);
			tileEditor.update(deltaTime);
			tileEditor.render();
			break;
		}
		// Update input class, handle pressed keys
		// Must be done last.
		//Perform world physics update before level 
		input.update();
	}

}

