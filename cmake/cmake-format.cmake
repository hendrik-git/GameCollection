# -------------------------------------------------------------------------------------------------
# cmake formatter
# -------------------------------------------------------------------------------------------------
find_program(EXE_CMAKEFORMAT "cmake-format")
if(NOT EXE_CMAKEFORMAT)
    message(STATUS "cmake-format executable not found! ${EXE_CMAKEFORMAT}")
    message(STATUS "-- Download with pip install cmakelang")
else()
    message(STATUS "cmake-format executable found! Adding target.")

    # find all cmake files to be formated
    file(GLOB_RECURSE CMAKE_FILE_LIST CMakeLists.txt *.cmake)

    add_custom_target(
        cmake-format ALL
        COMMAND cmake-format -c ${CMAKE_SOURCE_DIR}/tools/cmake-format.py -i
                ${CMAKE_FILE_LIST}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Formating CMake files in place, check git for differences"
    )
    set_target_properties(cmake-format PROPERTIES FOLDER Tools)
endif()
