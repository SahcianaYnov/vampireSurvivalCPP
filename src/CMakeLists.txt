add_library(vampireSurvival 
STATIC ${CMAKE_CURRENT_SOURCE_DIR}/systems/movementComponent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/systems/spawnComponent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/entities/playerEntity.cpp
${CMAKE_CURRENT_SOURCE_DIR}/entities/enemyEntity.cpp)

target_include_directories(vampireSurvival 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/entities/)

add_executable(main_test_exe ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)

target_link_libraries(main_test_exe 
PRIVATE GTest::GTest GTest::Main
PUBLIC vampireSurvival)

add_executable(main_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(main_exe 
PUBLIC ${SFML_LIBRARIES}
vampireSurvival)