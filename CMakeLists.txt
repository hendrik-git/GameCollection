cmake_minimum_required(VERSION 3.21)

project(
	GameCollection
	VERSION 0.1.0
        DESCRIPTION "A collection of games"
	LANGUAGES CXX
)

add_executable(GameCollectionApp app/main.cpp)