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
# Building the CodeHelpers library
# -------------------------------------------------------------------------------------------------
add_library(
    CodeHelpers OBJECT
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_SOURCE_DIR}/src/CodeHelpers/include/CodeHelpers/Overload.hpp
    ${CMAKE_SOURCE_DIR}/src/CodeHelpers/include/CodeHelpers/Profiler.hpp
    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/CodeHelpers/Overload.cpp
    ${CMAKE_SOURCE_DIR}/src/CodeHelpers/Profiler.cpp
)
target_include_directories(CodeHelpers PUBLIC src/CodeHelpers/include)
target_compile_features(CodeHelpers PUBLIC cxx_std_20)
target_compile_options(CodeHelpers PRIVATE ${ADDITIONAL_COMPILER_FLAGS})
if(USE_PROFILING)
    target_compile_definitions(CodeHelpers PUBLIC PROFILING)
endif()

# -------------------------------------------------------------------------------------------------
# Building the GameScenes library
# -------------------------------------------------------------------------------------------------
add_library(
    GameScenes OBJECT
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_SOURCE_DIR}/src/GameScenes/include/GameScenes/Scene.hpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/include/GameScenes/SceneAsteroids.hpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/include/GameScenes/SceneMainMenu.hpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/include/GameScenes/SceneParticleGallery.hpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/include/GameScenes/SceneShaderGallery.hpp

    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/GameScenes/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/SceneAsteroids.cpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/SceneMainMenu.cpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/SceneParticleGallery.cpp
    ${CMAKE_SOURCE_DIR}/src/GameScenes/SceneShaderGallery.cpp
)
target_link_libraries(GameScenes PUBLIC CodeHelpers GameEngine)
target_include_directories(GameScenes PUBLIC src/GameScenes/include)
target_compile_features(GameScenes PUBLIC cxx_std_20)
target_compile_options(GameScenes PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

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
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Collision.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Component.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Entity.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/EntityManager.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/GameEngine.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/ParticleManager.hpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Scene.hpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneAsteroids.hpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneMainMenu.hpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneParticleGallery.hpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/SceneShaderGallery.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/System.hpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/include/GameEngine/Utility.hpp
    # necessary source files
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Action.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Animation.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Asset.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Collision.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Component.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/GameEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/ParticleManager.cpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/Scene.cpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneAsteroids.cpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneMainMenu.cpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneParticleGallery.cpp
    # ${CMAKE_SOURCE_DIR}/src/GameEngine/SceneShaderGallery.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/System.cpp
    ${CMAKE_SOURCE_DIR}/src/GameEngine/Utility.cpp
)
target_include_directories(GameEngine 
PUBLIC  src/GameEngine/include 
PRIVATE src/GameScenes/include)
target_link_libraries(GameEngine PUBLIC CodeHelpers GameMath SFML::Graphics fmt::fmt)
target_compile_features(GameEngine PUBLIC cxx_std_20)
target_compile_options(GameEngine PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp app/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_20)
set_target_properties(GameCollectionApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(GameCollectionApp PRIVATE CLI11::CLI11 GameScenes GameEngine CodeHelpers)
target_compile_options(GameCollectionApp PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

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