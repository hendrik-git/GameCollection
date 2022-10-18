# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(
    GameCollection
    VERSION 0.2.0
    DESCRIPTION "A collection of games"
    LANGUAGES CXX
)

# select whether to build and link dynamic or static
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# tell cmake to look into the local /cmake subdirectory to look for modules (.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# use folders to group targets in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# update submodules, get the latest git commit
include(git_commands)

# load custom functions and macros
include(macros)

# -------------------------------------------------------------------------------------------------
# Declare all dependencies
# -------------------------------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED) # only download missing dependencies

# it is recommended to use commit SHA as argument to the GIT_TAG instead of the branch name or tag
# because it leads to a cleaner commit history and is faster to check

# cli11 is a command line parser library
fetchcontent_declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG a66ae4145779c56dc0f9f98a631656417dd77de8 # v2.3.0 [2022.10.15]
)

# The Simple and Fast Multimedia Library is used for graphics, audio, windows and input see also
# https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php
fetchcontent_declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 8f54a3e97c35476012613a8a41b7d2234d7ce38e # latest [2022.10.15]
)

# Catch2 is a unit test library
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG 97c48e0c343d26d50764fafdc90b1e630fbd10ce # v3.1.0 [2022.10.15]
)

# -------------------------------------------------------------------------------------------------
# Make all dependencies available
# -------------------------------------------------------------------------------------------------
fetchcontent_makeavailable(cli11 SFML Catch2)
set_vs_folder(CLI11 Dependencies)
set_vs_folder(sfml-audio Dependencies)
set_vs_folder(sfml-graphics Dependencies)
set_vs_folder(sfml-main Dependencies)
set_vs_folder(sfml-network Dependencies)
set_vs_folder(sfml-network Dependencies)
set_vs_folder(sfml-system Dependencies)
set_vs_folder(sfml-window Dependencies)
set_vs_folder(Catch2 Dependencies)
set_vs_folder(Catch2WithMain Dependencies)

# -------------------------------------------------------------------------------------------------
# Building the GameMath library
# -------------------------------------------------------------------------------------------------
add_library(
    GameMath OBJECT
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_SOURCE_DIR}/src/GameMath/include/GameMath/Vec2.hpp
    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/GameMath/Vec2.cpp
)
target_include_directories(GameMath PUBLIC src/GameMath/include)
target_compile_features(GameMath PUBLIC cxx_std_17)

# -------------------------------------------------------------------------------------------------
# Building the GameEngine
# -------------------------------------------------------------------------------------------------
add_library(
    GameEngine OBJECT
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Component.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Entity.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/EntityManager.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/GameEngine.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/System.hpp
    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Component.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/GameEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/System.cpp
)
target_include_directories(GameEngine PUBLIC src/GameEngine/include)
target_link_libraries(GameEngine PUBLIC GameMath SFML::Graphics)
target_compile_features(GameEngine PUBLIC cxx_std_17)

# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp app/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_17)
set_target_properties(GameCollectionApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(GameCollectionApp PRIVATE CLI11::CLI11 GameEngine)

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET GameCollectionApp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:GameCollectionApp>
                $<TARGET_FILE_DIR:GameCollectionApp>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

# -------------------------------------------------------------------------------------------------
# Testing
# -------------------------------------------------------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

# -------------------------------------------------------------------------------------------------
# Tools
# -------------------------------------------------------------------------------------------------
include(graphviz)
include(cmake-format)
include(cmake-lint)

# set the active project to the executable in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameCollectionApp)
