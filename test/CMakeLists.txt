# -------------------------------------------------------------------------------------------------
# Setup
# -------------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameEngine library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/System.cpp
)
target_link_libraries(Test_GameEngine PRIVATE GameEngine Catch2::Catch2WithMain)
SetFolderInVS(Test_GameEngine Test)

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameMath library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameMath
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMath/Vec2.cpp
)
target_link_libraries(Test_GameMath PRIVATE GameMath Catch2::Catch2WithMain)
SetFolderInVS(Test_GameMath Test)