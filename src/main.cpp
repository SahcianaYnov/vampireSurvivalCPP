#include <SFML/Graphics.hpp>
#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "type.hpp"
#include "systems/bullet_system.hpp"
#include <components/animationComponent.hpp>
#include <iostream> // Pour les logs de débogage

#include "components/render.hpp"
#include "systems/enemy_system.hpp"
#include "systems/spawnComponent.hpp"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Etape 1 - Initialisation des managers
    auto& componentManager = ecs::ComponentManager::singleton();
    auto& entityManager = ecs::EntityManager::singleton();
    auto& systemManager = ecs::SystemManager::singleton();

    // Etape 2 - Enregistrement des composants des managers
    componentManager.register_component<Transform>();
    componentManager.register_component<Motion>();
    componentManager.register_component<movcomp::Transform>();
    componentManager.register_component<movcomp::Motion>();
    componentManager.register_component<sf::CircleShape>();
    componentManager.register_component<Direction>();
    componentManager.register_component<SpriteRender>();

    // Etape 3 - Creation des entites avec leurs signatures
    ecs::Signature signatureWindow;
    ecs::Entity windowEntity = entityManager.create_entity();

    signatureWindow.set(componentManager.get_component_type<Transform>(), true);
    entityManager.set_entity_signature(windowEntity, signatureWindow);
    systemManager.update_entity_signature(windowEntity, signatureWindow);

    // Etape 4 - Creation des systemes avec leurs signatures
    auto bulletSystem = std::make_shared<BulletSystem>();
    ecs::Signature bulletSig;
    bulletSig.set(componentManager.get_component_type<Transform>(), true);
    bulletSig.set(componentManager.get_component_type<Motion>(), true);
    bulletSig.set(componentManager.get_component_type<sf::CircleShape>(), true);
    systemManager.register_system("BulletSystem", bulletSystem, bulletSig);

    auto enemySystem = std::make_shared<EnemySystem>();
    ecs::Signature enemySig;
    enemySig.set(componentManager.get_component_type<Transform>(), true);
    enemySig.set(componentManager.get_component_type<Motion>(), true);
    enemySig.set(componentManager.get_component_type<Direction>(), true);
    enemySig.set(componentManager.get_component_type<SpriteRender>(), true);
    systemManager.register_system("EnemySystem", enemySystem, enemySig);

    // Etape 5 - Configurer la fenetre de jeu (antialiasing dimensions titre vsync)
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    Transform windowTransformData{
        .position = {.x = 0, .y = 0},
        .size = {.x = 800, .y = 600},
    };

    sf::RenderWindow windowRender(
        sf::VideoMode(windowTransformData.size.x, windowTransformData.size.y),
        "Vampire Survival",
        sf::Style::Default,
        settings);

    windowRender.setVerticalSyncEnabled(true);

    // Etape 6 - Associer le composant transform que l on a creer (dimensions) a l entite fenetre de jeu
    componentManager.add_component<Transform>(windowEntity, std::move(windowTransformData));

    // Etape 7 - Joueur
    movcomp::Transform playerTransformData{
        .position = {.x = windowTransformData.size.x / 2, .y = windowTransformData.size.y / 2}
    };
    SpriteRender playerAnimation;
    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    movcomp::Motion playerMotionData{};
    ecs::Entity playerEntity = entityManager.create_entity();
    ecs::Signature signaturePlayer;

    playerTexture.loadFromFile("assets/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(playerTransformData.position.x, playerTransformData.position.y);

    // Association des composants a l entite joueur
    componentManager.add_component<movcomp::Transform>(playerEntity, std::move(playerTransformData));
    componentManager.add_component<movcomp::Motion>(playerEntity, std::move(playerMotionData));

    // Enregistrement de l entite et de sa signature
    signaturePlayer.set(componentManager.get_component_type<movcomp::Transform>(), true);
    signaturePlayer.set(componentManager.get_component_type<movcomp::Motion>(), true);

    entityManager.set_entity_signature(playerEntity, signaturePlayer);
    systemManager.update_entity_signature(playerEntity, signaturePlayer);

    // Etape 8 - Bullets
    // BULLETS
    Transform bulletsTransformData;
    Motion bulletsMotionData{};
    Vector2 directionNormalized{};
    sf::CircleShape circleRender(50.f);

    // TIMER
    sf::Clock spawnClock;
    float spawnInterval = 2.0f;
    float elapsedTime = 0.f;

    // Etape 9 - Enemies
    sf::Texture enemyTexture;
    Transform enemyTransformData;
    Motion enemyMotionData{};
    enemyTexture.loadFromFile("assets/player.png");

    // EVENT LOOP
    while (windowRender.isOpen()) {
        float speed = 500.f;
        sf::Event event{};

        while (windowRender.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                windowRender.close();
        }

        auto& playerMotion = componentManager.get_component<movcomp::Motion>(playerEntity);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            playerMotion.directionNormalized = { 0.f, -1.f };
            playerMotion.speed = speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            playerMotion.directionNormalized = { 0.f, 1.f };
            playerMotion.speed = speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            playerMotion.directionNormalized = { -1.f, 0.f };
            playerMotion.speed = speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerMotion.directionNormalized = { 1.f, 0.f };
            playerMotion.speed = speed;
        }
        else {
            playerMotion.directionNormalized = { 0.f, 0.f };
            playerMotion.speed = 0.f;
        }

        // Mettre à jour la position du joueur
        auto& playerTransform = componentManager.get_component<movcomp::Transform>(playerEntity);
        update_position(playerTransform, playerMotion, 0.016f);

        Vector2 convertedPlayerPosition = { .x = playerTransform.position.x, .y = playerTransform.position.y };
        Transform convertedPlayerTransformData = { .position = convertedPlayerPosition };

        // SPAWN
        elapsedTime += spawnClock.restart().asSeconds();
        if (elapsedTime >= spawnInterval) {
            bulletsTransformData.position = spacomp::randomizePosition(windowTransformData, directionNormalized);

            bulletsMotionData.direction = directionNormalized;
            bulletsMotionData.acceleration = { 100.f, 100.f };

            bulletSystem->create_bullet(bulletsTransformData, bulletsMotionData);

            enemyTransformData = enemySystem->randomizePosition(windowTransformData, convertedPlayerPosition);

            enemySystem->create_enemy(enemyTexture, windowTransformData, enemyTransformData);

            elapsedTime = 0.f;
        }

        bulletSystem->update(0.016f);
        enemySystem->handle_movements(0.016f, convertedPlayerPosition);

        bulletSystem->checkCollisions(windowTransformData, convertedPlayerTransformData);
        enemySystem->checkCollisions(windowTransformData, convertedPlayerTransformData);

        // RENDER
        windowRender.clear(sf::Color::Black);

        // PLAYER
        playerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        playerSprite.setPosition(playerTransform.position.x, playerTransform.position.y);
        windowRender.draw(playerSprite);

        // Bullet
        bulletSystem->render_bullet(windowRender);

        // Enemy
        enemySystem->render_enemy(windowRender);

        windowRender.display();
    }

    return 0;
}
