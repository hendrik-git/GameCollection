# -------------------------------------------------------------------------------------------------
# Setup
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
# Unit tests for the GameMath library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameMath
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMath/Vec2.cpp
)
target_link_libraries(Test_GameMath PRIVATE GameMath Catch2::Catch2WithMain)
set_VS_folder(Test_GameMath Test)