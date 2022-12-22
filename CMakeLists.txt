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

# tell cmake to look into the local /cmake subdirectory to look for modules (.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(git_commands) # update submodules, get the latest git commit
include(macros) # load custom functions and macros

option(BUILD_SHARED_LIBS "Build using shared libraries" ON) # to build and link dynamic or static
option(USE_PROFILING "Enable PROFILING makros for chrome tracing" OFF)

# set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Collect compiler flags in a variable to reuse for the following targets. Setting the compiler
# flags globally will also cause them to be used for dependencies. This may result in the
# dependencies breaking the build with warnings (with /WX or -Werror)
if(MSVC)
    set(ADDITIONAL_COMPILER_FLAGS /MP /fastfail /W4 /WX) # /utf-8 
else()
    set(ADDITIONAL_COMPILER_FLAGS -Werror -Wall)
endif()
mark_as_advanced(ADDITIONAL_COMPILER_FLAGS)

# -------------------------------------------------------------------------------------------------
# Declare all dependencies
# -------------------------------------------------------------------------------------------------
# package manager build on top of fetch_content, see https://github.com/cpm-cmake/CPM.cmake
include(CPM) 
CPMAddPackage("gh:fmtlib/fmt#a33701196adfad74917046096bf5a2aa0ab0bb50") # v9.1.0 [2022.08.27]
setfolderinvs(fmt Dependencies)

CPMAddPackage("gh:CLIUtils/CLI11#a66ae4145779c56dc0f9f98a631656417dd77de8") # v2.3.0 [2022.10.15]
setfolderinvs(CLI11 Dependencies)

CPMAddPackage("gh:SFML/SFML#8f54a3e97c35476012613a8a41b7d2234d7ce38e") # latest [2022.10.15]
setfolderinvs(sfml-audio Dependencies)
setfolderinvs(sfml-graphics Dependencies)
setfolderinvs(sfml-main Dependencies)
setfolderinvs(sfml-network Dependencies)
setfolderinvs(sfml-system Dependencies)
setfolderinvs(sfml-window Dependencies)

CPMAddPackage("gh:catchorg/Catch2#97c48e0c343d26d50764fafdc90b1e630fbd10ce") # v3.1.0 [2022.10.15]
setfolderinvs(Catch2 Dependencies)
setfolderinvs(Catch2WithMain Dependencies)

# -------------------------------------------------------------------------------------------------
# Building the targets
# -------------------------------------------------------------------------------------------------
add_subdirectory(src/CodeHelpers)
add_subdirectory(src/GameEngine)
add_subdirectory(src/GameMath)
add_subdirectory(src/GameScenes)
add_subdirectory(app)

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
# include(documentation)
include(poxy)

# -------------------------------------------------------------------------------------------------
# Visual Studio settings
# -------------------------------------------------------------------------------------------------
# set the active project to the executable in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameCollectionApp)
# set the working directory to the /bin directory
set_target_properties(GameCollectionApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
# neatly put all targets in their folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)