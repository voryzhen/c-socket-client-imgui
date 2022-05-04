cmake_minimum_required(VERSION 3.23)

project( hello )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/SDL2_win/include )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/SDL2_win/lib/x64 )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable( hello main.cpp )
target_link_libraries( hello SDL2 SDL2main )

install( TARGETS hello RUNTIME DESTINATION app )

set( CPACK_GENERATOR NSIS )
set( CPACK_PACKAGE_NAME "SimpleGUICPPProgram" )
set( CPACK_PACKAGE_VENDOR "CMake.org" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple GUI CPP Program" )

set( CPACK_PACKAGE_VERSION "1.0.0" )
set( CPACK_PACKAGE_VERSION_MAJOR "1" )
set( CPACK_PACKAGE_VERSION_MINOR "0" )
set( CPACK_PACKAGE_VERSION_PATCH "0" )

set( CPACK_PACKAGE_INSTALL_DIRECTORY "CPack Component Example" )

SET( CPACK_NSIS_MODIFY_PATH ON )

INCLUDE( CPack )
