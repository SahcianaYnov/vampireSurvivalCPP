#include <SFML/Graphics.hpp>

#include "playerEntity.hpp"
#include "enemyEntity.hpp"

using namespace playerent;
using namespace enemyent;
using namespace movcomp;

int main() {
	movcomp::Transform windowEngine;
	windowEngine.position = { 0,0 };
	windowEngine.size = { 800, 600 };

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow windowRender(
		sf::VideoMode(windowEngine.size.x, windowEngine.size.y),
		"Vampire Survival",
		sf::Style::Default,
		settings);

	sf::CircleShape bulletRender(50.f);

	enemyent::Enemy bulletEngine;

	bulletEngine.EnemyPosition = {windowEngine.size.x,0};

	sf::Vector2f sfVec = *reinterpret_cast<sf::Vector2f*>(&bulletEngine.EnemyPosition);

	bulletRender.setPosition(sfVec);

	while (windowRender.isOpen()) {
		sf::Event event;
		while (windowRender.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				windowRender.close();

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					windowRender.close();
					break;
					/*
					 *case sf::Keyboard::Z:
					 *bullet.move(0.f, -10.f);
					 *break;
					 *case sf::Keyboard::S:
					 *bullet.move(0.f, 10.f);
					 *break;
					 *case sf::Keyboard::Q:
					 *bullet.move(-10.f, 0.f);
					 *break;
					 *case sf::Keyboard::D:
					 *bullet.move(10.f, 0.f);
					 *break;
					 */
				
				}
			}
		}
		windowRender.clear(sf::Color::Black);
		windowRender.setVerticalSyncEnabled(true);

		// Bullet render
		bulletRender.setFillColor(sf::Color(100, 250, 50));
		windowRender.draw(bulletRender);

		bulletEngine.EnemyMotion.directionNormalized = {-1.f,0.f};

		bulletEngine.EnemyMotion.speed = 5.0f;

		bulletRender.move(bulletEngine.EnemyMotion.directionNormalized.x * bulletEngine.EnemyMotion.speed, 
			bulletEngine.EnemyMotion.directionNormalized.y);

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