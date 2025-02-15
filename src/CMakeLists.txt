add_library(vampireSurvival 
STATIC ${CMAKE_CURRENT_SOURCE_DIR}/systems/movementComponent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/systems/spawnComponent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/entities/playerEntity.cpp
${CMAKE_CURRENT_SOURCE_DIR}/entities/enemyEntity.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/components/animationComponent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/systems/bullet_system.cpp
${CMAKE_CURRENT_SOURCE_DIR}/systems/render_system.cpp
)

add_library(ecs 
STATIC ${CMAKE_CURRENT_SOURCE_DIR}/ecs/entity_manager.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ecs/system_manager.cpp
)

target_include_directories(vampireSurvival 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/entities/
${CMAKE_CURRENT_SOURCE_DIR}/components/)

target_include_directories(ecs 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/internal/)

add_executable(main_test_exe ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)

target_link_libraries(main_test_exe 
PRIVATE GTest::GTest GTest::Main
PUBLIC vampireSurvival
ecs)

add_executable(main_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(main_exe 
PUBLIC ${SFML_LIBRARIES}
vampireSurvival
ecs)