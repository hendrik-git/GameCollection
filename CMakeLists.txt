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
find_program(DOT_EXE "dot")
if(NOT DOT_EXE)
    message(STATUS "Graphviz executable not found! ${DOT_EXE}")
	message(STATUS "-- Download from https://graphviz.org/download/")
	message(STATUS "-- or install with sudo apt install graphviz")
else()
	message(STATUS "Graphviz executable found! Adding target.")
	add_custom_target(graphviz ALL
    	COMMAND ${CMAKE_COMMAND} "--graphviz=GameCollectionApp.dot" .
    	COMMAND dot -Tpng GameCollectionApp.dot -o ../docs/GameCollectionApp.png
    	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endif()