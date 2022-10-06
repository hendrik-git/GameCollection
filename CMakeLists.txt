#--------------------------------------------------------------------------------------------------
# Building the executable
#--------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(
	GameCollection
	VERSION 0.1.0
    DESCRIPTION "A collection of games"
	LANGUAGES CXX
)

# tell cmake to look into the local /cmake subdirectory to look for modules (.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# update submodules, get the latest git commit
include(git_commands)

#--------------------------------------------------------------------------------------------------
# Set up dependencies
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
# Building the executable
#--------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp app/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_17)
set_target_properties(GameCollectionApp PROPERTIES 
	CXX_EXTENSIONS OFF
)


#--------------------------------------------------------------------------------------------------
# Testing
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
# Graphviz
#--------------------------------------------------------------------------------------------------
include(graphviz)