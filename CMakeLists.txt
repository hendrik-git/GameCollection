# -------------------------------------------------------------------------------------------------
# GameCollection Project
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(
    GameCollection
    VERSION 0.3.0
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

# Collect compiler flags in a variable to reuse for the following targets. Setting the compiler
# flags globally will also cause them to be used for dependencies. This may result in the
# dependencies breaking the build with warnings (with /WX or -Werror)
if(MSVC)
    set(ADDITIONAL_COMPILER_FLAGS /MP /utf-8 /fastfail /W4 /WX)
else()
    set(ADDITIONAL_COMPILER_FLAGS -Werror -Wall)
endif()
mark_as_advanced(ADDITIONAL_COMPILER_FLAGS)

# -------------------------------------------------------------------------------------------------
# Declare all dependencies
# -------------------------------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED) # only download missing dependencies

# it is recommended to use commit SHA as argument to the GIT_TAG instead of the branch name or tag
# because it leads to a cleaner commit history and is faster to check

# fmt is a lightweight and fast formatting library
fetchcontent_declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50 # v9.1.0 [2022.08.27]
)


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
fetchcontent_makeavailable(fmt cli11 SFML Catch2)
setfolderinvs(fmt Dependencies)
setfolderinvs(CLI11 Dependencies)
setfolderinvs(sfml-audio Dependencies)
setfolderinvs(sfml-graphics Dependencies)
setfolderinvs(sfml-main Dependencies)
setfolderinvs(sfml-network Dependencies)
setfolderinvs(sfml-network Dependencies)
setfolderinvs(sfml-system Dependencies)
setfolderinvs(sfml-window Dependencies)
setfolderinvs(Catch2 Dependencies)
setfolderinvs(Catch2WithMain Dependencies)

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
target_compile_features(GameMath PUBLIC cxx_std_20)
target_compile_options(GameMath PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# -------------------------------------------------------------------------------------------------
# Building the GameEngine
# -------------------------------------------------------------------------------------------------
add_library(
    GameEngine OBJECT
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Action.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Animation.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Asset.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Component.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Entity.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/EntityManager.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/GameEngine.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Scene.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneAsteroids.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneMainMenu.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/System.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Utility.hpp
    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Action.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Animation.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Asset.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Component.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/GameEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneAsteroids.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneMainMenu.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/System.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Utility.cpp
)
target_include_directories(GameEngine PUBLIC src/GameEngine/include)
target_link_libraries(GameEngine PUBLIC GameMath SFML::Graphics fmt::fmt)
target_compile_features(GameEngine PUBLIC cxx_std_20)
target_compile_options(GameEngine PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp app/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_20)
set_target_properties(GameCollectionApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(GameCollectionApp PRIVATE CLI11::CLI11 GameEngine)
# target_compile_options(GameCollectionApp PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

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

# https://cmake.org/cmake/help/v3.9/module/FindDoxygen.html
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
set(DOXYGEN_GENERATE_HTML YES)
doxygen_add_docs(
    doxygen
    ${CMAKE_SOURCE_DIR}/src
    COMMENT "Create HTML documentation"
)
set_target_properties(doxygen PROPERTIES FOLDER Tools)

# set the active project to the executable in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameCollectionApp)
