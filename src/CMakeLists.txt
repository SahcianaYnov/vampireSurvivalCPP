# Ajoutez la biblioth�que ecs
add_library(ecs SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs/entity_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs/system_manager.cpp)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ecs)

# Ajoutez la biblioth�que vampireSurvival
add_library(vampireSurvival STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/movementComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/spawnComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entities/playerEntity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/bullet_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/enemy_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/components/animationComponent.cpp)

target_include_directories(vampireSurvival PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/entities/
    ${CMAKE_CURRENT_SOURCE_DIR}/components/)

# Liez vampireSurvival � ecs
target_link_libraries(vampireSurvival PUBLIC ecs sfml::sfml)

# Ajoutez l'ex�cutable main_test_exe
add_executable(main_test_exe ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)

target_link_libraries(main_test_exe
    PRIVATE GTest::GTest GTest::Main
    PUBLIC vampireSurvival ecs)

# Ajoutez l'ex�cutable main_exe
add_executable(main_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(main_exe
    PUBLIC ${SFML_LIBRARIES}
    vampireSurvival ecs)

# Ajoutez l'ex�cutable app
add_executable(app
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/enemy_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/render_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/bullet_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC sfml::sfml ecs)
