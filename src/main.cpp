#include <SFML/Graphics.hpp>
#include "playerEntity.hpp"
#include "enemyEntity.hpp"
#include "animationComponent.hpp"
#include <systems/spawnComponent.hpp>

#include "ecs/type.hpp"
#include <ecs/component_manager.hpp>

#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"
#include <systems/bullet_system.hpp>

using namespace playerent;
using namespace enemyent;
using namespace movcomp;

int main() {
    srand(static_cast<unsigned int>(time(0)));

    auto& componentManager = ecs::ComponentManager::singleton();
    auto& systemManager = ecs::SystemManager::singleton();

    componentManager.register_component<Motion>();
    componentManager.register_component<Transform>();
    componentManager.register_component<Player>();
    componentManager.register_component<Enemy>();

    auto bulletSystem = std::make_shared<BulletSystem>();
    ecs::Signature bulletSig;
    bulletSig.set(componentManager.get_component_type<Transform>(), true);
    bulletSig.set(componentManager.get_component_type<Motion>(), true);
    bulletSig.set(componentManager.get_component_type<sf::CircleShape>(), true);
    systemManager.register_system("BulletSystem", bulletSystem, bulletSig);


    // W I N D O W  &  S E T T I N G S

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    Transform windowTransformData{
        .position = {0, 0},
        .size = {800, 600},
    };

    ecs::Entity windowEntity = ecs::EntityManager::singleton().create_entity();

    sf::RenderWindow windowRender(
        sf::VideoMode(windowTransformData.size.x, windowTransformData.size.y),
        "Vampire Survival",
        sf::Style::Default,
        settings);
    windowRender.setVerticalSyncEnabled(true);

    ecs::ComponentManager::singleton().add_component<Transform>(windowEntity, std::move(windowTransformData));

    ecs::Signature signatureWindowTransform;
    signatureWindowTransform.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    ecs::EntityManager::singleton().set_entity_signature(windowEntity, signatureWindowTransform);
    ecs::SystemManager::singleton().update_entity_signature(windowEntity, signatureWindowTransform);

    // P L A Y E R
    Transform playerTransformData{
        .position = {windowTransformData.size.x / 2, windowTransformData.size.y / 2}
    };

    animcomp::Animation animation;
    sf::Sprite playerSprite = animation.sprite;
    sf::Texture playerTexture;

    playerTexture.loadFromFile("assets/player.png");
    playerSprite.setTexture(playerTexture);

    Motion playerMotionData;
    float speed = 500.f;

    playerSprite.setPosition(playerTransformData.position.x, playerTransformData.position.y);

    ecs::Entity playerEntity = ecs::EntityManager::singleton().create_entity();

    ecs::ComponentManager::singleton().add_component<Transform>(playerEntity, std::move(playerTransformData));
    ecs::ComponentManager::singleton().add_component<Motion>(playerEntity, std::move(playerMotionData));

    ecs::Signature signaturePlayerTransform;
    signaturePlayerTransform.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    ecs::EntityManager::singleton().set_entity_signature(playerEntity, signaturePlayerTransform);
    ecs::SystemManager::singleton().update_entity_signature(playerEntity, signaturePlayerTransform);

    ecs::Signature signaturePlayerMotion;
    signaturePlayerMotion.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
    ecs::EntityManager::singleton().set_entity_signature(playerEntity, signaturePlayerMotion);
    ecs::SystemManager::singleton().update_entity_signature(playerEntity, signaturePlayerMotion);

    // B U L L E T S
    Transform bulletsTransformData;
    Motion bulletsMotionData;
    Vec2 directionNormalized;
    sf::CircleShape circleRender(50.f);

    // T I M E R
    sf::Clock spawnClock;
    float spawnInterval = 1.0f;
    float elapsedTime = 0.f;

    // S P A W N
    elapsedTime += spawnClock.restart().asSeconds();
    if (elapsedTime >= spawnInterval) {
        ecs::Entity bulletEntity = ecs::EntityManager::singleton().create_entity();

        ecs::ComponentManager::singleton().add_component<Transform>(bulletEntity, std::move(bulletsTransformData));
        ecs::ComponentManager::singleton().add_component<Motion>(bulletEntity, std::move(bulletsMotionData));
        ecs::ComponentManager::singleton().add_component<sf::CircleShape>(bulletEntity, std::move(circleRender));

        ecs::Signature signatureBulletTransform;
        signatureBulletTransform.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
        ecs::EntityManager::singleton().set_entity_signature(bulletEntity, signatureBulletTransform);
        ecs::SystemManager::singleton().update_entity_signature(bulletEntity, signatureBulletTransform);

        ecs::Signature signatureBulletMotion;
        signatureBulletMotion.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
        ecs::EntityManager::singleton().set_entity_signature(bulletEntity, signatureBulletMotion);
        ecs::SystemManager::singleton().update_entity_signature(bulletEntity, signatureBulletMotion);

        ecs::Signature signatureCircleRender;
        signatureCircleRender.set(ecs::ComponentManager::singleton().get_component_type<sf::CircleShape>(), true);
        ecs::EntityManager::singleton().set_entity_signature(bulletEntity, signatureCircleRender);
        ecs::SystemManager::singleton().update_entity_signature(bulletEntity, signatureCircleRender);

        bulletsTransformData.position = spacomp::randomizePosition(windowTransformData, directionNormalized);

        if (bulletsTransformData.position.x >= 0 && bulletsTransformData.position.x <= windowTransformData.size.x &&
            bulletsTransformData.position.y >= 0 && bulletsTransformData.position.y <= windowTransformData.size.y) {

            bulletsMotionData.directionNormalized = directionNormalized;
            bulletsMotionData.speed = 50.0f;
        }

        elapsedTime = 0.f;
    }

    // E VE NT
    while (windowRender.isOpen()) {
        sf::Event event;
        while (windowRender.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                windowRender.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            playerMotionData = { {0.f, -1.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            playerMotionData = { {0.f, 1.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            playerMotionData = { {-1.f, 0.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerMotionData = { {1.f, 0.f}, speed };
        }
        else {
            playerMotionData = { {0.f, 0.f}, 0.f };
        }

        update_position(playerTransformData, playerMotionData, 0.016f);

    	//bulletSystem->updateBulletPositions(0.016f);

        //bulletSystem->deleteBullet(windowTransformData, playerTransformData);

        // REN DER
        windowRender.clear(sf::Color::Black);

        // P L A Y E R
        playerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        playerSprite.setPosition(playerTransformData.position.x, playerTransformData.position.y);

        windowRender.draw(playerSprite);

        // B U L L E T S
        circleRender.setFillColor(sf::Color(100, 250, 50));
        circleRender.setPosition(bulletsTransformData.position.x, bulletsTransformData.position.y);

        windowRender.draw(circleRender);

        windowRender.display();
    }

    return 0;
}
