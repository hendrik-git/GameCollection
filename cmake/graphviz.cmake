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