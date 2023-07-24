# -------------------------------------------------------------------------------------------------
# Building the GameMath library
# -------------------------------------------------------------------------------------------------
add_library(
    GameMath STATIC
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameMath/Spline.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameMath/Vec2.hpp
    # necessary source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Spline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vec2.cpp
)
target_include_directories(GameMath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(GameMath PUBLIC cxx_std_20)
target_compile_options(GameMath PRIVATE ${ADDITIONAL_COMPILER_FLAGS})