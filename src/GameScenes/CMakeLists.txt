# -------------------------------------------------------------------------------------------------
# Building the GameScenes library
# -------------------------------------------------------------------------------------------------
add_library(
    GameScenes STATIC
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/Asteroids.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/Scene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/SceneAsteroids.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/SceneMainMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/SceneParticleGallery.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/SceneShaderGallery.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/SceneSplines.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameScenes/ShadowCasting.hpp

    # necessary source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Asteroids.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneAsteroids.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneMainMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneParticleGallery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneShaderGallery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneSplines.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShadowCasting.cpp
)
target_link_libraries(GameScenes PUBLIC CodeHelpers GameEngine GameSystems)
target_include_directories(GameScenes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(GameScenes PUBLIC cxx_std_20)
target_compile_options(GameScenes PRIVATE ${ADDITIONAL_COMPILER_FLAGS})