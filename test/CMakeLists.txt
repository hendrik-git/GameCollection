# -------------------------------------------------------------------------------------------------
# Setup
# -------------------------------------------------------------------------------------------------
include(Catch)

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameEngine library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Audio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/System.cpp
)
target_link_libraries(Test_GameEngine PRIVATE GameEngine GameScenes Catch2::Catch2WithMain)
set_target_properties(Test_GameEngine PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
SetFolderInVS(Test_GameEngine Test)

#add_test(NAME GameEngine COMMAND Test_GameEngine)
catch_discover_tests(Test_GameEngine WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameMath library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameMath
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMath/Vec2.cpp
)
target_link_libraries(Test_GameMath PRIVATE GameMath Catch2::Catch2WithMain)
set_target_properties(Test_GameMath PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
SetFolderInVS(Test_GameMath Test)

#add_test(NAME GameMath COMMAND Test_GameMath)
catch_discover_tests(Test_GameMath WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)