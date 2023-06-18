# -------------------------------------------------------------------------------------------------
# Building the GameEngine
# -------------------------------------------------------------------------------------------------
add_library(
    GameEngine STATIC
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Action.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Animation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Asset.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Collision.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Component.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Entity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/EntityManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/GameEngine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/ParticleManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/System.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GameEngine/Utility.hpp
    # necessary source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Action.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Animation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility.cpp
)
target_include_directories(GameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(GameEngine PUBLIC CodeHelpers GameScenes GameMath SFML::Graphics SFML::Audio fmt::fmt)
target_compile_features(GameEngine PUBLIC cxx_std_20)
target_compile_options(GameEngine PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# OpenAL needs to be linked dynamically due to licensing.
# Unfortunately SFML does not propagate the given dll as runtime dependency yet.
# Therefore the following command will copy the necessary dll to the correct directory.
if(WIN32)
    add_custom_command(
        TARGET GameEngine COMMENT "Copy OpenAL DLL" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${FETCHCONTENT_BASE_DIR}/sfml-src/extlibs/bin/x64/openal32.dll 
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/OpenAl32.dll
    )
endif()