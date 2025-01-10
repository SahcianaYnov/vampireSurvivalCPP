#include <SFML/Graphics.hpp>

#include "playerEntity.hpp"
#include "enemyEntity.hpp"

#include <iostream>

using namespace playerent;
using namespace enemyent;
using namespace movcomp;

int main() {
	// EN GI NE

	// W I N D O W  &  S E T T I N G S
	Transform windowEngine;
	windowEngine.position = { 0,0 };
	windowEngine.size = { 800, 600 };

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow windowRender(
		sf::VideoMode(windowEngine.size.x, windowEngine.size.y),
		"Vampire Survival",
		sf::Style::Default,
		settings);
	windowRender.setVerticalSyncEnabled(true);


	// P L A Y E R
	sf::Sprite playerSprite;
	sf::Texture playerTexture;
	Player playerEngine;

	playerTexture.loadFromFile("assets/player.png");
	playerSprite.setTexture(playerTexture);

	playerEngine.PlayerTransform.position = {windowEngine.size.x / 2, windowEngine.size.y / 2};

	playerSprite.setPosition(playerEngine.PlayerTransform.position.x, playerEngine.PlayerTransform.position.y);

	// B U L L E T
	sf::CircleShape bulletRender(50.f);
	Enemy bulletEngine;

	bulletEngine.EnemyTransform.position = {windowEngine.size.x,0};

	sf::Vector2f sfVec = *reinterpret_cast<sf::Vector2f*>(&bulletEngine.EnemyTransform.position);

	bulletRender.setPosition(sfVec);

	// E VE NT
	while (windowRender.isOpen()) {
		sf::Event event;
		while (windowRender.pollEvent(event)) {
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				windowRender.close();
		}

		Motion direction;
		float speed = 500.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			direction = {
				.directionNormalized = {0.f,-1.f},
				.speed = speed
			};
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			direction = {
				.directionNormalized = {0.f,1.f},
				.speed = speed
			};

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			direction = {
				.directionNormalized = {-1.f,0.f},
				.speed = speed
			};
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			direction = {
				.directionNormalized = {1.f,0.f},
				.speed = speed
			};
		}else
		{
			direction = {
				.directionNormalized = {0.f, 0.f},
				.speed = 0.f
			};
		}
		update_position(playerEngine.PlayerTransform, direction, 0.016f);

		//REN DER
		windowRender.clear(sf::Color::Black);

		// P L A Y E R
		playerSprite.setTextureRect(sf::IntRect(0, 0, 64,64));

		//std::cout << playerEngine.PlayerTransform.position.x << playerEngine.PlayerTransform.position.y << std::endl;

		playerSprite.setPosition(playerEngine.PlayerTransform.position.x, playerEngine.PlayerTransform.position.y);

		windowRender.draw(playerSprite);

		// B U L L E T
		bulletRender.setFillColor(sf::Color(100, 250, 50));

		bulletEngine.EnemyMotion.directionNormalized = {-1.f,0.f};

		bulletEngine.EnemyMotion.speed = 5.0f;

		bulletRender.move(bulletEngine.EnemyMotion.directionNormalized.x * bulletEngine.EnemyMotion.speed, 
			bulletEngine.EnemyMotion.directionNormalized.y);

		windowRender.draw(bulletRender);


		/* Camera follow player
		sf::View view(sf::Vector2f(350.f, 300.f), sf::Vector2f(300.f,
			200.f));
		window.setView(view);
		view.setCenter(playerPosition);
		*/

		// Vec2 ou 2float, if contains bullet = not outside
		// for each bullet -> if !contains -> bullet.destroy();
		//window.getViewport().contains(bulletPosition);

		windowRender.display();
	}

	return 0;
}