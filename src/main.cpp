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
    enemySig.set(componentManager.get_component_type<sf::Texture>(), true);
    systemManager.register_system("BulletSystem", enemySystem, enemySig);

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
    SpriteRender enemyAnimation;
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;

    enemyTexture.loadFromFile("assets/player.png");

    // fait crash le code
    //enemySystem->create_enemy(enemyTexture, windowTransformData);

    enemySprite.setTexture(enemyTexture);

    // On doit recup les entites du systeme mais je comprends pas comment vu que le cours ne le montre jamais
    //enemyAnimation.sprite.setPosition();

    // EVENT LOOP
    while (windowRender.isOpen()) {
        float speed = 500.f;
        sf::Event event{};

        while (windowRender.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                windowRender.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            playerMotionData = { .directionNormalized = {.x = 0.f, .y = -1.f}, .speed = speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            playerMotionData = { .directionNormalized = {.x = 0.f, .y = 1.f}, .speed = speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            playerMotionData = { .directionNormalized = {.x = -1.f, .y = 0.f}, .speed = speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerMotionData = { .directionNormalized = {.x = 1.f, .y = 0.f}, .speed = speed };
        }
        else {
            playerMotionData = { .directionNormalized = {.x = 0.f, .y = 0.f}, .speed = 0.f };
        }

        // MAJ de la position du joueur
        auto& playerTransform = componentManager.get_component<movcomp::Transform>(playerEntity);
        auto& playerMotion = componentManager.get_component<movcomp::Motion>(playerEntity);

        Vector2 convertedPlayerPosition = { .x = playerTransformData.position.x, .y = playerTransformData.position.y };
        Transform convertedPlayerTransformData = { .position = convertedPlayerPosition };

        // Bug - playerMotionData n a pas ete update dans la signature pour X raisons
        update_position(playerTransform, playerMotionData, 0.016f);

        // SPAWN - fonctionne pas pour X raisons
        elapsedTime += spawnClock.restart().asSeconds();
        if (elapsedTime >= spawnInterval) {
            bulletsTransformData.position = spacomp::randomizePosition(windowTransformData, directionNormalized);
            bulletSystem->create_bullet(bulletsTransformData, bulletsMotionData);

            //ennemiesTransformData = ennemySystem->randomizePosition(bulletsTransformData, playerTransformData.position);
            //pas compris comment on fait vu que deja mis mon create enemy en haut

            elapsedTime = 0.f;
        }

        bulletSystem->update(0.016f);

        //fait crash pour X raisons
        //bulletSystem->checkCollisions(bulletsTransformData, windowTransformData, convertedPlayerTransformData, circleRender.getRadius());

        //enemySystem->handle_movements(0.016f);
        //enemySystem->checkCollisions(windowTransformData, convertedPlayerTransformData);

        // RENDER
        windowRender.clear(sf::Color::Black);

        // PLAYER
        playerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        playerSprite.setPosition(playerTransform.position.x, playerTransform.position.y);
        windowRender.draw(playerSprite);

        // Bullet
    	bulletSystem->render_bullet(windowRender);

        // Enemy
        enemySprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        // encore une fois, idk
        //enemySprite.setPosition();
        windowRender.draw(enemySprite);

        windowRender.display();
    }

    return 0;
}
