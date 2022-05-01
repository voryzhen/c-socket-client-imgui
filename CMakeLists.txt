project(hello)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/SDL2_win/include )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL2/SDL2_win/lib/x64 )

add_executable( hello main.cpp )
target_link_libraries( hello SDL2 SDL2main )

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

