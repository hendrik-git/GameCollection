# -------------------------------------------------------------------------------------------------
# GameCollection Project
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)
project(
    GameCollection
    VERSION 0.5.0
    DESCRIPTION "A collection of games"
    LANGUAGES CXX
)

# only works for Makefile Generators and Ninja Generators
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# tell cmake to look into the local /cmake subdirectory to look for modules (.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(git_commands) # update submodules, get the latest git commit
include(macros) # load custom functions and macros
include(ccache)

if(PROJECT_IS_TOP_LEVEL)
  include(sanitizers)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries"                ON) # to build and link dynamic or static
option(BUILD_DOCS        "Add targets for building docs"              OFF) # to build documentaion
option(USE_PROFILING     "Enable PROFILING makros for chrome tracing" OFF)

# set the output directories
if(MSVC)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

# Collect compiler flags in a variable to reuse for the following targets. Setting the compiler
# flags globally will also cause them to be used for dependencies. This may result in the
# dependencies breaking the build with warnings (with /WX or -Werror)
if(MSVC)
    set(ADDITIONAL_COMPILER_FLAGS /MP /W4 ) # /utf-8  /WX /fastfail 
else()
    #set(ADDITIONAL_COMPILER_FLAGS -Werror -Wall)
endif()

mark_as_advanced(ADDITIONAL_COMPILER_FLAGS)

# -------------------------------------------------------------------------------------------------
# Download and enable all dependencies
# -------------------------------------------------------------------------------------------------
# package manager build on top of fetch_content, see https://github.com/cpm-cmake/CPM.cmake
include(get_cpm)

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.4.2
)
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

# CPMAddPackage("gh:ArthurSonzogni/FTXUI@3.0.0") # v3.0.0 [2023.04.02]
# setfolderinvs(component Dependencies/ftxui)
# setfolderinvs(dom Dependencies/ftxui)
# setfolderinvs(screen Dependencies/ftxui)

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 11.0.2
)
setfolderinvs(fmt Dependencies)

CPMAddPackage(
    NAME entt
    GITHUB_REPOSITORY skypjack/entt
    GIT_TAG v3.14.0
)

CPMAddPackage(
    NAME quill
    GITHUB_REPOSITORY odygrd/quill
    GIT_TAG v7.5.0  # Specify the version you need
    OPTIONS 
        "QUILL_FMT_EXTERNAL ON"
        "CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON"
)
setfolderinvs(quill Dependencies)


# -------------------------------------------------------------------------------------------------
# Building the targets
# -------------------------------------------------------------------------------------------------
if(USE_PROFILING)
    add_subdirectory(extern/perfetto)
endif()
add_subdirectory(extern/tomlplusplus)

add_subdirectory(src/CodeHelpers)
add_subdirectory(src/GameEngine)
add_subdirectory(src/GameMath)
add_subdirectory(src/GameScenes)
add_subdirectory(src/GameSystems)

add_subdirectory(app/GameCollectionApp)
# add_subdirectory(app/RemoteControlApp)

# -------------------------------------------------------------------------------------------------
# Testing
# -------------------------------------------------------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()    
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
include(cpp_check)

if(BUILD_DOCS)
    include(documentation)
    include(poxy)
endif()

# -------------------------------------------------------------------------------------------------
# Visual Studio settings
# -------------------------------------------------------------------------------------------------
# set the active project to the executable in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameCollectionApp)
# set the working directory to the /bin directory
set_target_properties(GameCollectionApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
# neatly put all targets in their folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)