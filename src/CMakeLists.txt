add_library(vampireSurvival STATIC "movementComponent.cpp")

target_include_directories(vampireSurvival PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main_test_exe main_test.cpp)

target_link_libraries(main_test_exe 
PRIVATE GTest::GTest GTest::Main
PUBLIC vampireSurvival)

add_executable(main_exe main.cpp)

target_link_libraries(main_exe 
PUBLIC sfml::sfml
PUBLIC vampireSurvival)


