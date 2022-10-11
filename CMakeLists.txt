# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(
    GameCollection
    VERSION 0.1.0
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

# -------------------------------------------------------------------------------------------------
# Declare all dependencies
# -------------------------------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED) # only download missing dependencies

# cli11 is a command line parser library
fetchcontent_declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.3.0
)

# The Simple and Fast Multimedia Library is used for graphics, audio, windows and input see also
# https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php
fetchcontent_declare(
    SFML GIT_REPOSITORY https://github.com/SFML/SFML.git # GIT_TAG 2.5.1 # use the latest
                                                         # experimental sfml
)

# Catch2 is a unit test library
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.1.0
)

# -------------------------------------------------------------------------------------------------
# Make all dependencies available
# -------------------------------------------------------------------------------------------------
fetchcontent_makeavailable(cli11 SFML Catch2)
set_target_properties(CLI11 PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-audio PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-graphics PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-main PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-network PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-system PROPERTIES FOLDER Dependencies)
set_target_properties(sfml-window PROPERTIES FOLDER Dependencies)
set_target_properties(Catch2 PROPERTIES FOLDER Dependencies)
set_target_properties(Catch2WithMain PROPERTIES FOLDER Dependencies)

# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp app/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_17)
set_target_properties(GameCollectionApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(GameCollectionApp PRIVATE CLI11::CLI11 SFML::Graphics)

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32)
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

# -------------------------------------------------------------------------------------------------
# Tools
# -------------------------------------------------------------------------------------------------
include(graphviz)
include(cmake-format)
include(cmake-lint)
