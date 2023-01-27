# example integration taken from https://perfetto.dev/docs/instrumentation/tracing-sdk

find_package(Threads)

add_library(perfetto STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/perfetto/perfetto.h
    ${CMAKE_CURRENT_SOURCE_DIR}/perfetto.cc
)

add_library(perfetto::perfetto ALIAS perfetto)

target_include_directories(perfetto 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE   ${CMAKE_CURRENT_SOURCE_DIR}/include/perfetto
)

if (WIN32)
    # The perfetto library contains many symbols, so it needs the big object format.
    target_compile_options(perfetto PRIVATE "/bigobj")
    # Disable legacy features in windows.h.
    #target_compile_definitions(perfetto PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    # On Windows we should link to WinSock2.
    target_link_libraries(perfetto PUBLIC ws2_32 ${CMAKE_THREAD_LIBS_INIT})
endif (WIN32)

# Enable standards-compliant mode when using the Visual Studio compiler.
if (MSVC)
    target_compile_options(perfetto PRIVATE "/W0" "/permissive-")
endif (MSVC)

SetFolderInVS(perfetto Dependencies)