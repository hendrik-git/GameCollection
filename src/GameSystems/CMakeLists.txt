# -------------------------------------------------------------------------------------------------
# Building the GameSystems library
# -------------------------------------------------------------------------------------------------
add_library(
    GameSystems STATIC
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameSystems/Score.hpp
    # necessary source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Score.cpp
)
target_include_directories(GameSystems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(GameSystems PUBLIC cxx_std_20)
target_compile_options(GameSystems PRIVATE ${ADDITIONAL_COMPILER_FLAGS})
target_link_libraries(GameSystems PUBLIC fmt::fmt tomlplusplus CodeHelpers)